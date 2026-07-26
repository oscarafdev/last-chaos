from __future__ import annotations

import hashlib
import re
import struct
from dataclasses import dataclass
from pathlib import Path

from .c_source import load_static_char_arrays
from .d3dx9_assembler import D3DX9Assembler, ShaderAssemblyError
from .legacy_constants import (
    D3DPS_VERSION_1_1,
    GFX_COLOR_STREAM,
    GFX_NORMAL_STREAM,
    GFX_POSITION_STREAM,
    GFX_TANGENT_STREAM,
    GFX_TEXCOORD0,
    GFX_TEXCOORD1,
    GFX_TEXCOORD2,
    GFX_TEXCOORD3,
    GFX_WEIGHT_STREAM,
    SHA_NAKED_CODE,
)
from .models import ExtractedShader


_FNV_OFFSET = 14695981039346656037
_FNV_PRIME = 1099511628211
_NORMALIZE_NORMAL = (
    "dp3  r8.w,   r1,      r1       \n"
    "rsq  r7.w,   r8.w              \n"
    "mul  r1,     r1,      r7.wwww  \n"
)
_WEIGHT_FRAGMENTS = (
    "strNoWeights",
    "strOneWeight",
    "strTwoWeights",
    "strThreeWeights",
    "strFourWeights",
)
_SHADER_MAIN = re.compile(r"\bSHADER_MAIN\s*\(\s*([A-Za-z_]\w*)")
_FOG_UNIT = re.compile(r"\bshaSetFogTextureUnit\s*\(\s*(\d+)")


@dataclass(frozen=True)
class ShaderPolicy:
    vertex_mode: str
    fog_texture_units: tuple[int, ...]


def build_exact_runtime_inventory(
    shaders: list[ExtractedShader],
    shader_code_header: Path,
    shader_source_root: Path,
    runtime_catalog: dict[str, object],
) -> dict[str, object]:
    fragments = load_static_char_arrays(shader_code_header)
    _validate_fragments(fragments)
    policies = _load_shader_policies(shader_source_root)
    assembler = D3DX9Assembler()
    errors: list[dict[str, object]] = []
    vertex_variants: list[dict[str, object]] = []
    pixel_variants: list[dict[str, object]] = []

    for shader in shaders:
        if shader.extraction_error:
            continue
        policy = policies.get(
            shader.manifest.main_export,
            ShaderPolicy("default", ()),
        )
        vertex_variants.extend(
            _materialize_vertex_variants(
                shader,
                policy,
                fragments,
                assembler,
                errors,
            )
        )
        pixel_variants.extend(
            _materialize_pixel_variants(
                shader,
                fragments,
                assembler,
                errors,
            )
        )

    candidate_pairs = _build_candidate_pairs(
        vertex_variants,
        pixel_variants,
        runtime_catalog,
    )
    implemented = {
        (
            pair["vertex_fingerprint"],
            pair["pixel_fingerprint"],
        )
        for pair in runtime_catalog["implemented_pairs"]
    }
    validated = {
        (
            pair["vertex_fingerprint"],
            pair["pixel_fingerprint"],
        )
        for pair in runtime_catalog["validated_pairs"]
    }
    candidates = {
        (
            pair["vertex_fingerprint"],
            pair["pixel_fingerprint"],
        )
        for pair in candidate_pairs
    }
    generated_vertex = {
        variant["fingerprint"] for variant in vertex_variants
    }
    generated_pixel = {
        variant["fingerprint"] for variant in pixel_variants
    }
    catalog_vertex = {
        item["fingerprint"] for item in runtime_catalog["vertex_families"]
    }
    catalog_pixel = {
        item["fingerprint"] for item in runtime_catalog["pixel_families"]
    }

    return {
        "method": {
            "assembler": "d3dx9_43!D3DXAssembleShader",
            "vertex_conversion": "CompileVertexProgram_D3D",
            "vertex_declaration": "GetShaderDeclaration_D3D9",
            "hash": "FNV-1a 64",
            "normalization_variants": [True, False],
            "weight_variants": [0, 1, 2, 3, 4],
        },
        "summary": {
            "vertex_variant_count": len(vertex_variants),
            "unique_vertex_fingerprint_count": len(generated_vertex),
            "pixel_variant_count": len(pixel_variants),
            "unique_pixel_fingerprint_count": len(generated_pixel),
            "candidate_pair_count": len(candidate_pairs),
            "implemented_pair_correlation_count": len(implemented & candidates),
            "validated_pair_correlation_count": len(validated & candidates),
            "catalog_vertex_fingerprint_correlation_count": len(
                catalog_vertex & generated_vertex
            ),
            "catalog_pixel_fingerprint_correlation_count": len(
                catalog_pixel & generated_pixel
            ),
            "assembly_error_count": len(errors),
        },
        "catalog_correlation": {
            "implemented_pairs_found": _format_pair_set(implemented & candidates),
            "implemented_pairs_missing": _format_pair_set(implemented - candidates),
            "validated_pairs_found": _format_pair_set(validated & candidates),
            "validated_pairs_missing": _format_pair_set(validated - candidates),
            "vertex_fingerprints_found": sorted(
                catalog_vertex & generated_vertex
            ),
            "vertex_fingerprints_missing": sorted(
                catalog_vertex - generated_vertex
            ),
            "pixel_fingerprints_found": sorted(
                catalog_pixel & generated_pixel
            ),
            "pixel_fingerprints_missing": sorted(
                catalog_pixel - generated_pixel
            ),
        },
        "policies": {
            name: {
                "vertex_mode": policy.vertex_mode,
                "fog_texture_units": list(policy.fog_texture_units),
            }
            for name, policy in sorted(policies.items())
        },
        "vertex_variants": vertex_variants,
        "pixel_variants": pixel_variants,
        "candidate_pairs": candidate_pairs,
        "assembly_errors": errors,
    }


def convert_vertex_program_to_d3d9(source: str) -> str:
    if "ps.1" in source:
        return source

    declarations = (
        ("v0", "    dcl_position v0\n"),
        ("v4", "    dcl_color v4\n"),
        ("v5", "    dcl_texcoord0 v5\n"),
        ("v6", "    dcl_texcoord1 v6\n"),
        ("v7", "    dcl_texcoord2 v7\n"),
        ("v8", "    dcl_texcoord3 v8\n"),
        ("v2", "    dcl_blendweight v2\n"),
        ("v1", "    dcl_normal v1\n"),
        ("v3", "    dcl_blendindices v3\n"),
        ("v9", "    dcl_tangent v9\n"),
    )
    initializers = tuple(
        (f"r{register}", f"    mov r{register}, c0\n")
        for register in range(11, -1, -1)
    ) + (
        ("oD1", "    mov oD1, c0\n"),
        ("oD0", "    mov oD0, c0\n"),
        ("oT3", "    mov oT3, c0\n"),
        ("oT2", "    mov oT2, c0\n"),
        ("oT1", "    mov oT1, c0\n"),
        ("oT0", "    mov oT0, c0\n"),
    )
    header = "    vs_1_1\n"
    header += "".join(value for token, value in declarations if token in source)
    header += "".join(value for token, value in initializers if token in source)
    return header + "//" + source[2:]


def build_vertex_declaration(stream_flags: int) -> bytes:
    elements: list[tuple[int, int, int, int, int, int]] = []
    simple_elements = (
        (
            GFX_POSITION_STREAM,
            (0, 0, 2, 0, 0, 0),
        ),
        (
            GFX_COLOR_STREAM,
            (1, 0, 4, 0, 10, 0),
        ),
        (
            GFX_TEXCOORD0,
            (2, 0, 1, 0, 5, 0),
        ),
        (
            GFX_TEXCOORD1,
            (3, 0, 1, 0, 5, 1),
        ),
        (
            GFX_TEXCOORD2,
            (4, 0, 1, 0, 5, 2),
        ),
        (
            GFX_TEXCOORD3,
            (5, 0, 1, 0, 5, 3),
        ),
        (
            GFX_NORMAL_STREAM,
            (6, 0, 2, 0, 3, 0),
        ),
    )
    for flag, element in simple_elements:
        if stream_flags & flag:
            elements.append(element)
    if stream_flags & GFX_WEIGHT_STREAM:
        elements.extend(
            (
                (7, 0, 4, 0, 2, 0),
                (7, 4, 4, 0, 1, 0),
            )
        )
    if stream_flags & GFX_TANGENT_STREAM:
        elements.append((8, 0, 3, 0, 6, 0))
    elements.append((0xFF, 0, 17, 0, 0, 0))
    return b"".join(
        struct.pack("<HHBBBB", *element)
        for element in elements
    )


def fnv1a64(data: bytes, initial: int = _FNV_OFFSET) -> int:
    value = initial
    for byte in data:
        value ^= byte
        value = (value * _FNV_PRIME) & 0xFFFFFFFFFFFFFFFF
    return value


def _materialize_vertex_variants(
    shader: ExtractedShader,
    policy: ShaderPolicy,
    fragments: dict[str, str],
    assembler: D3DX9Assembler,
    errors: list[dict[str, object]],
) -> list[dict[str, object]]:
    records: list[dict[str, object]] = []
    for variant in shader.vertex_variants:
        if not variant.source:
            continue
        base_flags = (
            shader.descriptor.stream_flags[variant.index]
            if variant.index < len(shader.descriptor.stream_flags)
            else 0
        )
        naked = bool(base_flags & SHA_NAKED_CODE)
        normalizations = (
            (True, False)
            if (
                not naked
                and policy.vertex_mode == "default"
                and base_flags & GFX_NORMAL_STREAM
            )
            else (False,)
        )
        fog_units: tuple[int | None, ...] = (
            (None,)
            if naked
            else (None, *policy.fog_texture_units)
        )

        weight_counts = (0,) if naked else range(5)
        for weight_count in weight_counts:
            stream_flags = base_flags & ~SHA_NAKED_CODE
            if weight_count > 0:
                stream_flags |= GFX_WEIGHT_STREAM
            for normalized in normalizations:
                for fog_unit in fog_units:
                    full_source = _assemble_vertex_source(
                        variant.source,
                        base_flags,
                        policy.vertex_mode,
                        weight_count,
                        normalized,
                        fog_unit,
                        fragments,
                    )
                    try:
                        bytecode = assembler.assemble(
                            convert_vertex_program_to_d3d9(full_source)
                        )
                    except ShaderAssemblyError as error:
                        _record_error(
                            errors,
                            shader.manifest.relative_path,
                            "vertex",
                            variant.index,
                            error,
                            {
                                "weight_count": weight_count,
                                "normalized": normalized,
                                "fog_texture_unit": fog_unit,
                            },
                        )
                        continue
                    declaration = build_vertex_declaration(stream_flags)
                    fingerprint = fnv1a64(bytecode + declaration)
                    records.append(
                        {
                            "manifest": shader.manifest.relative_path,
                            "program_index": variant.index,
                            "source_sha256": variant.source_sha256,
                            "vertex_mode": policy.vertex_mode,
                            "weight_count": weight_count,
                            "normalized": normalized,
                            "fog_texture_unit": fog_unit,
                            "stream_flags": stream_flags,
                            "stream_flags_hex": f"0x{stream_flags:08X}",
                            "bytecode_size": len(bytecode),
                            "bytecode_sha256": hashlib.sha256(bytecode).hexdigest(),
                            "declaration_sha256": hashlib.sha256(
                                declaration
                            ).hexdigest(),
                            "fingerprint": f"{fingerprint:016X}",
                        }
                    )
    return records


def _materialize_pixel_variants(
    shader: ExtractedShader,
    fragments: dict[str, str],
    assembler: D3DX9Assembler,
    errors: list[dict[str, object]],
) -> list[dict[str, object]]:
    records: list[dict[str, object]] = []
    prefix_name = (
        "strPixelProgramPrefix11"
        if shader.descriptor.pixel_version_value == D3DPS_VERSION_1_1
        else "strPixelProgramPrefix14"
    )
    for variant in shader.pixel_variants:
        if not variant.source:
            continue
        source = fragments[prefix_name] + variant.source
        try:
            bytecode = assembler.assemble(source)
        except ShaderAssemblyError as error:
            _record_error(
                errors,
                shader.manifest.relative_path,
                "pixel",
                variant.index,
                error,
                {"fog_type": variant.fog_type},
            )
            continue
        fingerprint = fnv1a64(bytecode)
        records.append(
            {
                "manifest": shader.manifest.relative_path,
                "program_index": variant.index,
                "fog_type": variant.fog_type,
                "source_sha256": variant.source_sha256,
                "shader_version": (
                    "ps_1_1"
                    if prefix_name == "strPixelProgramPrefix11"
                    else "ps_1_4"
                ),
                "bytecode_size": len(bytecode),
                "bytecode_sha256": hashlib.sha256(bytecode).hexdigest(),
                "fingerprint": f"{fingerprint:016X}",
            }
        )
    return records


def _assemble_vertex_source(
    user_source: str,
    base_flags: int,
    vertex_mode: str,
    weight_count: int,
    normalized: bool,
    fog_texture_unit: int | None,
    fragments: dict[str, str],
) -> str:
    if base_flags & SHA_NAKED_CODE:
        return user_source

    weight_fragment = _WEIGHT_FRAGMENTS[weight_count]
    if vertex_mode == "tangent_b":
        prefix = fragments[f"{weight_fragment}_NMTangentSpaceB"]
    elif base_flags & GFX_NORMAL_STREAM:
        prefix = fragments[weight_fragment]
    else:
        prefix = fragments[f"{weight_fragment}_OnlyPosition"]

    source = prefix
    if normalized:
        source += _NORMALIZE_NORMAL
    source += user_source
    if fog_texture_unit is not None:
        source += fragments["strFogVP"]
        source += (
            f"mul  oT{fog_texture_unit}.x,  r9.w,    c20.z \n"
            f"mul  oT{fog_texture_unit}.y,  r8.w,    c20.w \n"
        )
    return source


def _build_candidate_pairs(
    vertex_variants: list[dict[str, object]],
    pixel_variants: list[dict[str, object]],
    runtime_catalog: dict[str, object],
) -> list[dict[str, object]]:
    implemented = {
        (
            pair["vertex_fingerprint"],
            pair["pixel_fingerprint"],
        )
        for pair in runtime_catalog["implemented_pairs"]
    }
    validated = {
        (
            pair["vertex_fingerprint"],
            pair["pixel_fingerprint"],
        )
        for pair in runtime_catalog["validated_pairs"]
    }
    vertices_by_manifest: dict[str, list[dict[str, object]]] = {}
    pixels_by_manifest: dict[str, list[dict[str, object]]] = {}
    for variant in vertex_variants:
        vertices_by_manifest.setdefault(
            str(variant["manifest"]),
            [],
        ).append(variant)
    for variant in pixel_variants:
        pixels_by_manifest.setdefault(
            str(variant["manifest"]),
            [],
        ).append(variant)

    aggregated: dict[tuple[str, str], dict[str, object]] = {}
    for manifest, vertices in vertices_by_manifest.items():
        for vertex in vertices:
            for pixel in pixels_by_manifest.get(manifest, []):
                if not _fog_variants_are_compatible(vertex, pixel):
                    continue
                key = (
                    str(vertex["fingerprint"]),
                    str(pixel["fingerprint"]),
                )
                entry = aggregated.setdefault(
                    key,
                    {
                        "vertex_fingerprint": key[0],
                        "pixel_fingerprint": key[1],
                        "implemented": key in implemented,
                        "validated": key in validated,
                        "configuration_count": 0,
                        "sample_configurations": [],
                    },
                )
                entry["configuration_count"] += 1
                samples = entry["sample_configurations"]
                if len(samples) < 8:
                    samples.append(
                        {
                            "manifest": manifest,
                            "vertex_index": vertex["program_index"],
                            "pixel_index": pixel["program_index"],
                            "weight_count": vertex["weight_count"],
                            "normalized": vertex["normalized"],
                            "fog_texture_unit": vertex["fog_texture_unit"],
                            "fog_type": pixel["fog_type"],
                        }
                    )
    return [
        aggregated[key]
        for key in sorted(aggregated)
    ]


def _fog_variants_are_compatible(
    vertex: dict[str, object],
    pixel: dict[str, object],
) -> bool:
    if pixel["fog_type"] == "none":
        return vertex["fog_texture_unit"] is None
    return vertex["fog_texture_unit"] is not None


def _load_shader_policies(source_root: Path) -> dict[str, ShaderPolicy]:
    policies: dict[str, ShaderPolicy] = {}
    for path in sorted(source_root.glob("*.cpp")):
        source = path.read_text(encoding="latin-1")
        shader_names = _SHADER_MAIN.findall(source)
        if not shader_names:
            continue
        mode = "tangent_b" if "NMMT_TANSPACE" in source else "default"
        fog_units = tuple(
            sorted({int(value) for value in _FOG_UNIT.findall(source)})
        )
        for name in shader_names:
            policies[f"Shader_{name}"] = ShaderPolicy(mode, fog_units)
    return policies


def _record_error(
    errors: list[dict[str, object]],
    manifest: str,
    kind: str,
    program_index: int,
    error: ShaderAssemblyError,
    configuration: dict[str, object],
) -> None:
    errors.append(
        {
            "manifest": manifest,
            "kind": kind,
            "program_index": program_index,
            "configuration": configuration,
            "hresult": f"0x{error.hresult:08X}",
            "message": error.message[:1000],
        }
    )


def _validate_fragments(fragments: dict[str, str]) -> None:
    required = {
        "strPixelProgramPrefix11",
        "strPixelProgramPrefix14",
        "strFogVP",
    }
    for weight_fragment in _WEIGHT_FRAGMENTS:
        required.add(weight_fragment)
        required.add(f"{weight_fragment}_NMTangentSpaceB")
        required.add(f"{weight_fragment}_OnlyPosition")
    missing = sorted(required - fragments.keys())
    if missing:
        raise ValueError(
            "Faltan fragmentos de ShaderCode.h: "
            + ", ".join(missing)
        )


def _format_pair_set(
    pairs: set[tuple[str, str]],
) -> list[dict[str, str]]:
    return [
        {
            "vertex_fingerprint": vertex,
            "pixel_fingerprint": pixel,
        }
        for vertex, pixel in sorted(pairs)
    ]
