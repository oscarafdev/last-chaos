from __future__ import annotations

from pathlib import Path

from .models import ShaderManifest


_KNOWN_FIELDS = {
    "Package",
    "Name",
    "Info",
    "VPCode",
    "PPCode",
}


def load_manifests(shader_root: Path) -> list[ShaderManifest]:
    manifests: list[ShaderManifest] = []
    for path in sorted(shader_root.glob("*.sha"), key=lambda item: item.name.lower()):
        fields = _parse_fields(path)
        missing = {"Package", "Name", "Info"} - fields.keys()
        if missing:
            missing_text = ", ".join(sorted(missing))
            raise ValueError(f"{path}: faltan campos obligatorios: {missing_text}")

        manifests.append(
            ShaderManifest(
                path=path.resolve(),
                relative_path=path.relative_to(shader_root.parent).as_posix(),
                package=fields["Package"],
                main_export=fields["Name"],
                descriptor_export=fields["Info"],
                vertex_export=fields.get("VPCode"),
                pixel_export=fields.get("PPCode"),
            )
        )
    return manifests


def _parse_fields(path: Path) -> dict[str, str]:
    fields: dict[str, str] = {}
    for line_number, raw_line in enumerate(
        path.read_text(encoding="utf-8-sig", errors="strict").splitlines(),
        start=1,
    ):
        line = raw_line.strip()
        if not line or line.startswith("#") or line.startswith("//"):
            continue
        key, separator, value = line.partition(":")
        if not separator:
            raise ValueError(f"{path}:{line_number}: línea de manifiesto inválida")
        key = key.strip()
        value = value.strip()
        if key not in _KNOWN_FIELDS:
            raise ValueError(f"{path}:{line_number}: campo desconocido {key!r}")
        if key in fields:
            raise ValueError(f"{path}:{line_number}: campo duplicado {key!r}")
        fields[key] = value
    return fields
