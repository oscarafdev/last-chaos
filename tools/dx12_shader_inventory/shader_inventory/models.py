from __future__ import annotations

from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Any


@dataclass(frozen=True)
class ShaderManifest:
    path: Path
    relative_path: str
    package: str
    main_export: str
    descriptor_export: str
    vertex_export: str | None
    pixel_export: str | None


@dataclass
class ProgramVariant:
    index: int
    source_sha256: str
    source: str
    fog_type: str | None = None


@dataclass
class ShaderDescriptor:
    texture_names: list[str]
    texcoord_names: list[str]
    color_names: list[str]
    float_names: list[str]
    flag_names: list[str]
    stream_flags: list[int]
    vertex_program_count: int
    pixel_program_count: int
    shader_info: str
    vertex_version: str
    pixel_version: str
    vertex_version_value: int
    pixel_version_value: int


@dataclass
class ExtractedShader:
    manifest: ShaderManifest
    descriptor: ShaderDescriptor
    vertex_variants: list[ProgramVariant] = field(default_factory=list)
    pixel_variants: list[ProgramVariant] = field(default_factory=list)
    extraction_error: str | None = None


@dataclass(frozen=True)
class AssetShaderReference:
    asset_path: str
    shader_path: str


@dataclass
class ContentScan:
    roots: list[str]
    file_count: int
    byte_count: int
    snapshot_sha256: str
    extension_counts: dict[str, int]
    references: list[AssetShaderReference]
    errors: list[dict[str, str]]


def to_serializable(value: Any) -> Any:
    if hasattr(value, "__dataclass_fields__"):
        data = asdict(value)
        if isinstance(value, ShaderManifest):
            data["path"] = str(value.path)
        return data
    if isinstance(value, Path):
        return str(value)
    raise TypeError(f"No se puede serializar {type(value)!r}")
