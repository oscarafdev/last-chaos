from __future__ import annotations

import csv
import json
from collections import Counter, defaultdict
from datetime import datetime, timezone
from pathlib import Path

from .content_scan import canonical_shader_path
from .models import ContentScan, ExtractedShader


def write_inventory(
    output_dir: Path,
    client_root: Path,
    shaders: list[ExtractedShader],
    content: ContentScan,
    source_inventory: dict[str, object],
    runtime_catalog: dict[str, object],
    exact_runtime_inventory: dict[str, object],
    repository_commit: str | None,
) -> dict[str, object]:
    output_dir.mkdir(parents=True, exist_ok=True)
    source_root = output_dir / "sources"
    source_root.mkdir(exist_ok=True)

    manifests = [
        _serialize_shader(shader, source_root)
        for shader in shaders
    ]
    references_by_shader: dict[str, list[str]] = defaultdict(list)
    for reference in content.references:
        references_by_shader[canonical_shader_path(reference.shader_path)].append(
            reference.asset_path
        )

    known_paths = {
        canonical_shader_path(shader.manifest.relative_path): shader
        for shader in shaders
    }
    unknown_references = sorted(
        (
            {
                "shader_path": shader_path,
                "asset_count": len(set(asset_paths)),
                "sample_assets": sorted(set(asset_paths))[:20],
            }
            for shader_path, asset_paths in references_by_shader.items()
            if shader_path not in known_paths
        ),
        key=lambda item: item["shader_path"],
    )

    for manifest in manifests:
        canonical = canonical_shader_path(manifest["manifest"]["relative_path"])
        assets = sorted(set(references_by_shader.get(canonical, [])))
        manifest["asset_reference_count"] = len(assets)
        manifest["sample_assets"] = assets[:20]

    potential_pairs = _potential_pairs(shaders)
    referenced_manifests = sum(
        1 for manifest in manifests if manifest["asset_reference_count"] > 0
    )
    programmable_manifests = sum(
        1
        for shader in shaders
        if shader.vertex_variants or shader.pixel_variants
    )
    inventory = {
        "schema_version": 1,
        "generated_at_utc": datetime.now(timezone.utc).isoformat(),
        "repository_commit": repository_commit,
        "scope": {
            "client_root": str(client_root.resolve()),
            "guarantee": (
                "Se leyeron todos los bytes accesibles de las raíces declaradas, "
                "todos los manifiestos .sha y todas sus variantes exportadas por "
                "Shaders.dll. El digest identifica exactamente este snapshot."
            ),
            "limits": [
                (
                    "Una referencia construida dinámicamente por código no contiene "
                    "necesariamente la cadena .sha dentro de un asset."
                ),
                (
                    "potential_program_pairs conserva el superset por fuente. "
                    "exact_runtime_inventory agrega pesos, estado y bytecode final."
                ),
                (
                    "Los shaders internos creados desde C++ se inventarían por sitio "
                    "de definición, fuera del ABI de Shaders.dll."
                ),
            ],
        },
        "summary": {
            "scan_complete": not content.errors,
            "content_file_count": content.file_count,
            "content_byte_count": content.byte_count,
            "manifest_count": len(shaders),
            "referenced_manifest_count": referenced_manifests,
            "programmable_manifest_count": programmable_manifests,
            "manifest_extraction_error_count": sum(
                1 for shader in shaders if shader.extraction_error
            ),
            "asset_shader_reference_count": len(content.references),
            "unknown_shader_reference_count": len(unknown_references),
            "potential_program_pair_count": len(potential_pairs),
            "dll_vertex_source_count": sum(
                len(shader.vertex_variants) for shader in shaders
            ),
            "dll_pixel_source_count": sum(
                len(shader.pixel_variants) for shader in shaders
            ),
            "unique_dll_vertex_source_count": len(
                {
                    variant.source_sha256
                    for shader in shaders
                    for variant in shader.vertex_variants
                    if variant.source
                }
            ),
            "unique_dll_pixel_source_count": len(
                {
                    variant.source_sha256
                    for shader in shaders
                    for variant in shader.pixel_variants
                    if variant.source
                }
            ),
            "exact_vertex_fingerprint_count": exact_runtime_inventory[
                "summary"
            ]["unique_vertex_fingerprint_count"],
            "exact_pixel_fingerprint_count": exact_runtime_inventory[
                "summary"
            ]["unique_pixel_fingerprint_count"],
            "exact_candidate_pair_count": exact_runtime_inventory[
                "summary"
            ]["candidate_pair_count"],
            "implemented_pair_correlation_count": exact_runtime_inventory[
                "summary"
            ]["implemented_pair_correlation_count"],
            "validated_pair_correlation_count": exact_runtime_inventory[
                "summary"
            ]["validated_pair_correlation_count"],
            "exact_assembly_error_count": exact_runtime_inventory[
                "summary"
            ]["assembly_error_count"],
        },
        "content": {
            "roots": content.roots,
            "snapshot_sha256": content.snapshot_sha256,
            "extension_counts": content.extension_counts,
            "errors": content.errors,
        },
        "manifests": manifests,
        "unknown_shader_references": unknown_references,
        "potential_program_pairs": potential_pairs,
        "internal_shader_sources": source_inventory,
        "dx12_runtime_catalog": runtime_catalog,
        "exact_runtime_inventory": exact_runtime_inventory,
    }

    (output_dir / "inventory.json").write_text(
        json.dumps(inventory, ensure_ascii=False, indent=2),
        encoding="utf-8",
    )
    _write_references_csv(output_dir / "asset-shader-references.csv", content)
    _write_summary(output_dir / "summary.md", inventory)
    return inventory


def _serialize_shader(
    shader: ExtractedShader,
    source_root: Path,
) -> dict[str, object]:
    manifest_name = shader.manifest.path.stem
    vertex_variants = []
    for variant in shader.vertex_variants:
        relative = Path(manifest_name) / f"vp-{variant.index}.asm"
        _write_source_once(source_root / relative, variant.source)
        vertex_variants.append(
            {
                "index": variant.index,
                "source_sha256": variant.source_sha256,
                "source_file": relative.as_posix(),
                "empty": not bool(variant.source),
            }
        )

    pixel_variants = []
    for variant in shader.pixel_variants:
        relative = (
            Path(manifest_name)
            / f"pp-{variant.index}-fog-{variant.fog_type}.asm"
        )
        _write_source_once(source_root / relative, variant.source)
        pixel_variants.append(
            {
                "index": variant.index,
                "fog_type": variant.fog_type,
                "source_sha256": variant.source_sha256,
                "source_file": relative.as_posix(),
                "empty": not bool(variant.source),
            }
        )

    return {
        "manifest": {
            "relative_path": shader.manifest.relative_path,
            "package": shader.manifest.package,
            "main_export": shader.manifest.main_export,
            "descriptor_export": shader.manifest.descriptor_export,
            "vertex_export": shader.manifest.vertex_export,
            "pixel_export": shader.manifest.pixel_export,
        },
        "descriptor": {
            **shader.descriptor.__dict__,
            "stream_flags_hex": [
                f"0x{flags:08X}" for flags in shader.descriptor.stream_flags
            ],
        },
        "vertex_variants": vertex_variants,
        "pixel_variants": pixel_variants,
        "extraction_error": shader.extraction_error,
    }


def _write_source_once(path: Path, source: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(source, encoding="latin-1")


def _potential_pairs(shaders: list[ExtractedShader]) -> list[dict[str, object]]:
    pairs: list[dict[str, object]] = []
    for shader in shaders:
        vertex_variants = [variant for variant in shader.vertex_variants if variant.source]
        pixel_variants = [variant for variant in shader.pixel_variants if variant.source]
        for vertex in vertex_variants:
            stream_flags = (
                shader.descriptor.stream_flags[vertex.index]
                if vertex.index < len(shader.descriptor.stream_flags)
                else None
            )
            for pixel in pixel_variants:
                pairs.append(
                    {
                        "manifest": shader.manifest.relative_path,
                        "vertex_index": vertex.index,
                        "vertex_source_sha256": vertex.source_sha256,
                        "stream_flags": stream_flags,
                        "stream_flags_hex": (
                            f"0x{stream_flags:08X}"
                            if stream_flags is not None
                            else None
                        ),
                        "pixel_index": pixel.index,
                        "fog_type": pixel.fog_type,
                        "pixel_source_sha256": pixel.source_sha256,
                    }
                )
    return pairs


def _write_references_csv(path: Path, content: ContentScan) -> None:
    with path.open("w", encoding="utf-8-sig", newline="") as stream:
        writer = csv.writer(stream)
        writer.writerow(("asset_path", "shader_path"))
        for reference in content.references:
            writer.writerow((reference.asset_path, reference.shader_path))


def _write_summary(path: Path, inventory: dict[str, object]) -> None:
    summary = inventory["summary"]
    content = inventory["content"]
    runtime = inventory["dx12_runtime_catalog"]
    manifests = inventory["manifests"]
    extensions = Counter(content["extension_counts"])
    top_extensions = extensions.most_common(15)
    status = "completo" if summary["scan_complete"] else "incompleto"
    unknown = summary["unknown_shader_reference_count"]

    lines = [
        "# Inventario offline de shaders DirectX 12",
        "",
        f"- Estado del escaneo: **{status}**.",
        f"- Snapshot SHA-256: `{content['snapshot_sha256']}`.",
        (
            f"- Contenido leído: **{summary['content_file_count']:,} archivos**, "
            f"**{summary['content_byte_count']:,} bytes**."
        ),
        (
            f"- Manifiestos: **{summary['manifest_count']}**, de los cuales "
            f"**{summary['referenced_manifest_count']}** tienen referencias explícitas."
        ),
        (
            f"- Código extraído de Shaders.dll: "
            f"**{summary['unique_dll_vertex_source_count']} VS únicos** y "
            f"**{summary['unique_dll_pixel_source_count']} PS únicos**."
        ),
        (
            f"- Superset de parejas componibles: "
            f"**{summary['potential_program_pair_count']}**."
        ),
        (
            f"- Catálogo DX12 actual: **{runtime['vertex_family_count']} VS**, "
            f"**{runtime['pixel_family_count']} PS**, "
            f"**{runtime['implemented_pair_count']} parejas implementadas** y "
            f"**{runtime['validated_pair_count']} validadas**."
        ),
        (
            f"- Fingerprints reconstruidos: "
            f"**{summary['exact_vertex_fingerprint_count']} VS**, "
            f"**{summary['exact_pixel_fingerprint_count']} PS** y "
            f"**{summary['exact_candidate_pair_count']} parejas candidatas**."
        ),
        (
            f"- Correlación con DX12: "
            f"**{summary['implemented_pair_correlation_count']}/"
            f"{runtime['implemented_pair_count']} implementadas** y "
            f"**{summary['validated_pair_correlation_count']}/"
            f"{runtime['validated_pair_count']} validadas**."
        ),
        (
            f"- Variantes exportadas que D3DX9 rechazó: "
            f"**{summary['exact_assembly_error_count']}**."
        ),
        f"- Referencias a manifiestos desconocidos: **{unknown}**.",
        (
            f"- Manifiestos con exports ausentes o inválidos: "
            f"**{summary['manifest_extraction_error_count']}**."
        ),
        "",
        "## Cobertura por manifiesto",
        "",
        "| Manifiesto | Assets | VS | PS (incluye niebla) | Streams |",
        "|---|---:|---:|---:|---|",
    ]
    for manifest in manifests:
        descriptor = manifest["descriptor"]
        stream_flags = ", ".join(descriptor["stream_flags_hex"]) or "—"
        lines.append(
            "| {name} | {assets} | {vertex} | {pixel} | `{streams}` |".format(
                name=manifest["manifest"]["relative_path"],
                assets=manifest["asset_reference_count"],
                vertex=len(manifest["vertex_variants"]),
                pixel=len(manifest["pixel_variants"]),
                streams=stream_flags,
            )
        )

    lines.extend(
        [
            "",
            "## Formatos encontrados",
            "",
            "| Extensión | Archivos |",
            "|---|---:|",
        ]
    )
    for extension, count in top_extensions:
        lines.append(f"| `{extension}` | {count:,} |")

    lines.extend(
        [
            "",
            "## Interpretación",
            "",
            (
                "El escaneo cubre el snapshot instalado sin recorrer mapas: lee cada "
                "byte de las raíces declaradas, reconoce referencias ASCII y UTF-16LE, "
                "extrae los descriptores reales y ejecuta los exports de código de la "
                "DLL en un host aislado."
            ),
            "",
            _exact_interpretation(summary, runtime),
            "",
        ]
    )
    path.write_text("\n".join(lines), encoding="utf-8")


def _exact_interpretation(
    summary: dict[str, object],
    runtime: dict[str, object],
) -> str:
    if (
        summary["implemented_pair_correlation_count"]
        == runtime["implemented_pair_count"]
        and summary["validated_pair_correlation_count"]
        == runtime["validated_pair_count"]
    ):
        return (
            "La reconstrucción exacta materializa pesos, normalización, niebla, "
            "normal mapping, bytecode y declaración D3D9. El cruce reprodujo "
            "todas las parejas implementadas y validadas del catálogo DX12, "
            "incluidos los shaders internos de terreno y efectos."
        )
    return (
        "Una pareja del superset no implica que un material concreto la use. "
        "La reconstrucción exacta materializa pesos, normalización, niebla, "
        "normal mapping, bytecode y declaración D3D9; las ausencias del cruce "
        "permanecen señaladas en exact_runtime_inventory.catalog_correlation."
    )
