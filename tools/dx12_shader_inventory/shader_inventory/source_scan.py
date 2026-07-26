from __future__ import annotations

import re
from pathlib import Path


_SHADER_MACROS = {
    "descriptor": re.compile(r"\bSHADER_DESC\s*\(\s*([A-Za-z_]\w*)"),
    "vertex": re.compile(r"\bSHADER_VCODE\s*\(\s*([A-Za-z_]\w*)"),
    "pixel": re.compile(r"\bSHADER_PCODE\s*\(\s*([A-Za-z_]\w*)"),
}
_DIRECT_CREATION = {
    "vertex": re.compile(r"\bgfxCreateVertexProgram\s*\("),
    "pixel": re.compile(r"\bgfxCreatePixelProgram\s*\("),
}


def scan_internal_shader_sources(source_root: Path) -> dict[str, object]:
    definitions: dict[str, dict[str, object]] = {}
    direct_sites: list[dict[str, object]] = []
    source_files = sorted(
        (
            path
            for path in source_root.rglob("*")
            if path.is_file() and path.suffix.lower() in {".cpp", ".c", ".h", ".hpp"}
        ),
        key=lambda item: item.as_posix().lower(),
    )

    for path in source_files:
        text = path.read_text(encoding="latin-1")
        relative = path.relative_to(source_root.parent).as_posix()
        for kind, pattern in _SHADER_MACROS.items():
            for match in pattern.finditer(text):
                name = match.group(1)
                entry = definitions.setdefault(
                    name,
                    {
                        "name": name,
                        "descriptor": False,
                        "vertex": False,
                        "pixel": False,
                        "paths": set(),
                    },
                )
                entry[kind] = True
                entry["paths"].add(relative)

        for kind, pattern in _DIRECT_CREATION.items():
            for match in pattern.finditer(text):
                direct_sites.append(
                    {
                        "kind": kind,
                        "path": relative,
                        "line": text.count("\n", 0, match.start()) + 1,
                    }
                )

    normalized_definitions = []
    for entry in sorted(definitions.values(), key=lambda item: str(item["name"]).lower()):
        normalized_definitions.append(
            {
                **entry,
                "paths": sorted(entry["paths"]),
            }
        )
    return {
        "definition_count": len(normalized_definitions),
        "definitions": normalized_definitions,
        "direct_creation_site_count": len(direct_sites),
        "direct_creation_sites": direct_sites,
    }
