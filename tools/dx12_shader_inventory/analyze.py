from __future__ import annotations

import argparse
import ctypes
import json
import subprocess
import sys
from datetime import datetime
from pathlib import Path

from shader_inventory.content_scan import scan_content
from shader_inventory.internal_programs import load_internal_program_shaders
from shader_inventory.legacy_fingerprints import build_exact_runtime_inventory
from shader_inventory.manifests import load_manifests
from shader_inventory.report import write_inventory
from shader_inventory.runtime_catalog import load_runtime_catalog
from shader_inventory.shader_host import (
    ShaderMetadataHost,
    build_metadata_host,
    failed_shader,
)
from shader_inventory.source_scan import scan_internal_shader_sources


TOOL_ROOT = Path(__file__).resolve().parent
REPOSITORY_ROOT = TOOL_ROOT.parents[1]


def main() -> int:
    arguments = _parse_arguments()
    client_root = arguments.client_root.resolve()
    output_dir = arguments.output.resolve()
    content_roots = (
        [path.resolve() for path in arguments.content_root]
        if arguments.content_root
        else [client_root / "Data"]
    )

    if ctypes.sizeof(ctypes.c_void_p) != 8:
        raise RuntimeError("El host de Shaders.dll requiere Python de 64 bits")

    print("[1/6] Leyendo manifiestos .sha...", flush=True)
    manifests = load_manifests(client_root / "Shaders")
    packages = {Path(item.package.replace("\\", "/")).name.lower() for item in manifests}
    if packages != {"shaders.dll"}:
        raise RuntimeError(f"Paquetes de shader no soportados: {sorted(packages)}")

    print("[2/6] Preparando host aislado de Shaders.dll...", flush=True)
    host_dir = build_metadata_host(
        TOOL_ROOT / "host" / "EngineMetadataStub.vcxproj",
        arguments.cache_root.resolve() / "host",
        client_root / "Bin" / "Shaders.dll",
    )

    print("[3/6] Extrayendo descriptores y variantes de programa...", flush=True)
    host = ShaderMetadataHost(host_dir)
    try:
        shaders = []
        for manifest in manifests:
            try:
                shaders.append(host.extract(manifest))
            except (AttributeError, OSError, RuntimeError) as error:
                print(
                    f"  ADVERTENCIA: {manifest.relative_path}: {error}",
                    flush=True,
                )
                shaders.append(failed_shader(manifest, error))
    finally:
        host.close()

    print("[4/6] Escaneando cada byte del contenido instalado...", flush=True)
    content = scan_content(
        content_roots,
        client_root,
        progress=lambda completed, total, bytes_read: print(
            f"  {completed:,}/{total:,} archivos; {bytes_read:,} bytes",
            flush=True,
        ),
    )

    print("[5/6] Cruzando fuentes internas y cobertura DX12...", flush=True)
    source_inventory = scan_internal_shader_sources(client_root / "src")
    runtime_catalog = load_runtime_catalog(
        client_root
        / "src"
        / "Engine"
        / "Graphics"
        / "DirectX12LegacyShaderFamily.cpp"
    )
    print("[6/6] Reconstruyendo fingerprints y parejas exactas...", flush=True)
    internal_program_shaders = load_internal_program_shaders(
        client_root / "src" / "Engine"
    )
    exact_runtime_inventory = build_exact_runtime_inventory(
        shaders=shaders + internal_program_shaders,
        shader_code_header=client_root
        / "src"
        / "Engine"
        / "Graphics"
        / "ShaderCode.h",
        shader_source_root=client_root / "src" / "Shaders",
        runtime_catalog=runtime_catalog,
    )
    inventory = write_inventory(
        output_dir=output_dir,
        client_root=client_root,
        shaders=shaders,
        content=content,
        source_inventory=source_inventory,
        runtime_catalog=runtime_catalog,
        exact_runtime_inventory=exact_runtime_inventory,
        repository_commit=_git_commit(REPOSITORY_ROOT),
    )

    print(json.dumps(inventory["summary"], ensure_ascii=False, indent=2), flush=True)
    print(f"Informe: {output_dir / 'summary.md'}", flush=True)
    print(f"Inventario: {output_dir / 'inventory.json'}", flush=True)
    return 0 if inventory["summary"]["scan_complete"] else 2


def _parse_arguments() -> argparse.Namespace:
    timestamp = datetime.now().strftime("%Y%m%d-%H%M%S")
    default_cache = REPOSITORY_ROOT / ".itconfig" / "dx12-shader-inventory"
    parser = argparse.ArgumentParser(
        description=(
            "Inventaría offline todo el contenido y las variantes legacy que "
            "debe cubrir el backend DirectX 12."
        )
    )
    parser.add_argument(
        "--client-root",
        type=Path,
        default=REPOSITORY_ROOT / "client",
    )
    parser.add_argument(
        "--content-root",
        type=Path,
        action="append",
        help="Raíz a escanear; puede repetirse. Por defecto usa client/Data.",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=default_cache / timestamp,
    )
    parser.add_argument(
        "--cache-root",
        type=Path,
        default=default_cache,
    )
    return parser.parse_args()


def _git_commit(repository_root: Path) -> str | None:
    result = subprocess.run(
        ["git", "rev-parse", "HEAD"],
        cwd=repository_root,
        check=False,
        capture_output=True,
        text=True,
    )
    return result.stdout.strip() if result.returncode == 0 else None


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, ValueError) as error:
        print(f"ERROR: {error}", file=sys.stderr)
        raise SystemExit(1)
