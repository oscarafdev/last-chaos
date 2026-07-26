from __future__ import annotations

import re
from pathlib import Path


_VERTEX_ENTRY = re.compile(
    r"\{\s*0x([0-9A-F]+)ULL,\s*"
    r"(DX12_LEGACY_VS_[A-Z0-9_]+),\s*"
    r"(true|false),\s*(true|false),\s*(true|false)\s*\}",
    re.MULTILINE,
)
_PIXEL_ENTRY = re.compile(
    r"\{\s*0x([0-9A-F]+)ULL,\s*"
    r"(DX12_LEGACY_PS_[A-Z0-9_]+),\s*(\d+)\s*\}",
    re.MULTILINE,
)
_PAIR_ENTRY = re.compile(
    r"\{\s*0x([0-9A-F]+)ULL,\s*0x([0-9A-F]+)ULL\s*\}",
    re.MULTILINE,
)


def load_runtime_catalog(path: Path) -> dict[str, object]:
    text = path.read_text(encoding="utf-8", errors="replace")
    vertex_block = _array_block(text, "VERTEX_FAMILIES")
    pixel_block = _array_block(text, "PIXEL_FAMILIES")
    replacement_block = _array_block(text, "REPLACEMENT_PAIRS")
    validated_block = _array_block(text, "VALIDATED_REPLACEMENT_PAIRS")

    vertex_families = [
        {
            "fingerprint": f"{fingerprint:0>16}",
            "family": family,
            "normals": normals == "true",
            "weights": weights == "true",
            "tangents": tangents == "true",
        }
        for fingerprint, family, normals, weights, tangents in _VERTEX_ENTRY.findall(
            vertex_block
        )
    ]
    pixel_families = [
        {
            "fingerprint": f"{fingerprint:0>16}",
            "family": family,
            "texture_count": int(texture_count),
        }
        for fingerprint, family, texture_count in _PIXEL_ENTRY.findall(pixel_block)
    ]
    replacement_pairs = _parse_pairs(replacement_block)
    validated_pairs = _parse_pairs(validated_block)
    return {
        "source": path.as_posix(),
        "vertex_family_count": len(vertex_families),
        "pixel_family_count": len(pixel_families),
        "implemented_pair_count": len(replacement_pairs),
        "validated_pair_count": len(validated_pairs),
        "vertex_families": vertex_families,
        "pixel_families": pixel_families,
        "implemented_pairs": replacement_pairs,
        "validated_pairs": validated_pairs,
    }


def _array_block(text: str, name: str) -> str:
    start_pattern = re.compile(
        rf"\bconst\s+\w+\s+{re.escape(name)}\s*\[\]\s*=\s*\{{"
    )
    match = start_pattern.search(text)
    if not match:
        raise ValueError(f"No se encontró la tabla {name}")
    end = text.find("};", match.end())
    if end < 0:
        raise ValueError(f"La tabla {name} no tiene cierre")
    return text[match.end() : end]


def _parse_pairs(block: str) -> list[dict[str, str]]:
    return [
        {
            "vertex_fingerprint": f"{vertex:0>16}",
            "pixel_fingerprint": f"{pixel:0>16}",
        }
        for vertex, pixel in _PAIR_ENTRY.findall(block)
    ]
