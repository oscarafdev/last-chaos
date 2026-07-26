from __future__ import annotations

import hashlib
import re
from pathlib import Path

from .c_source import concatenate_c_string_literals, read_c_expression
from .legacy_constants import (
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
from .models import (
    ExtractedShader,
    ProgramVariant,
    ShaderDescriptor,
    ShaderManifest,
)


_VERTEX_MACRO = re.compile(r"\bSHADER_VCODE\s*\(\s*([A-Za-z_]\w*)")
_PIXEL_MACRO = re.compile(r"\bSHADER_PCODE\s*\(\s*([A-Za-z_]\w*)")
_VERTEX_ASSIGNMENT = re.compile(r"\bstrVPCode\s*=")
_PIXEL_ASSIGNMENT = re.compile(r"\bstrPPCode\s*=")
_STATIC_CHAR_POINTER = re.compile(
    r"\bstatic\s+char\s*\*\s*([A-Za-z_]\w*)\s*="
)
_CREATE_VERTEX = re.compile(
    r"\bgfxCreateVertexProgram\s*\(\s*([A-Za-z_]\w*)\s*,\s*([^)]+)\)"
)
_CREATE_PIXEL = re.compile(
    r"\bgfxCreatePixelProgram\s*\(\s*([A-Za-z_]\w*)\s*\)"
)
_PIXEL_VERSION = re.compile(r"^\s*ps[._]1[._]\d[^\r\n]*(?:\r?\n)?")
_STREAM_FLAGS = {
    "GFX_POSITION_STREAM": GFX_POSITION_STREAM,
    "GFX_COLOR_STREAM": GFX_COLOR_STREAM,
    "GFX_TEXCOORD0": GFX_TEXCOORD0,
    "GFX_TEXCOORD1": GFX_TEXCOORD1,
    "GFX_TEXCOORD2": GFX_TEXCOORD2,
    "GFX_TEXCOORD3": GFX_TEXCOORD3,
    "GFX_NORMAL_STREAM": GFX_NORMAL_STREAM,
    "GFX_WEIGHT_STREAM": GFX_WEIGHT_STREAM,
    "GFX_TANGENT_STREAM": GFX_TANGENT_STREAM,
}


def load_internal_program_shaders(engine_source_root: Path) -> list[ExtractedShader]:
    terrain = _load_terrain_shaders(engine_source_root / "Terrain")
    direct = _load_direct_creation_shaders(engine_source_root)
    return terrain + direct


def _load_terrain_shaders(terrain_root: Path) -> list[ExtractedShader]:
    shaders: list[ExtractedShader] = []
    for path in sorted(terrain_root.glob("TRShader_*.cpp")):
        source = _preprocess(
            path.read_text(encoding="latin-1"),
            {"TER_SHADER_OPT": True},
        )
        vertex_functions = _extract_macro_programs(
            source,
            _VERTEX_MACRO,
            _VERTEX_ASSIGNMENT,
        )
        pixel_functions = _extract_macro_programs(
            source,
            _PIXEL_MACRO,
            _PIXEL_ASSIGNMENT,
        )
        for name in sorted(vertex_functions.keys() | pixel_functions.keys()):
            vertex_sources = vertex_functions.get(name, [])
            pixel_sources = pixel_functions.get(name, [])
            shaders.append(
                _make_shader(
                    path=path,
                    name=name,
                    vertex_sources=vertex_sources,
                    pixel_sources=pixel_sources,
                    stream_flags=[
                        GFX_POSITION_STREAM | SHA_NAKED_CODE
                        for _ in vertex_sources
                    ],
                    package="<internal-terrain>",
                )
            )
    return shaders


def _load_direct_creation_shaders(
    engine_source_root: Path,
) -> list[ExtractedShader]:
    shaders: list[ExtractedShader] = []
    source_files = sorted(
        path
        for path in engine_source_root.rglob("*.cpp")
        if "Terrain" not in path.parts
    )
    for path in source_files:
        source = path.read_text(encoding="latin-1")
        strings = _extract_static_char_pointers(source)
        vertex_calls = [
            (name, _parse_stream_flags(expression))
            for name, expression in _CREATE_VERTEX.findall(source)
            if name in strings
        ]
        pixel_calls = [
            name
            for name in _CREATE_PIXEL.findall(source)
            if name in strings
        ]
        if not vertex_calls or not pixel_calls:
            continue

        vertex_sources = [strings[name] for name, _ in vertex_calls]
        pixel_sources = [
            _PIXEL_VERSION.sub("", strings[name], count=1)
            for name in pixel_calls
        ]
        shaders.append(
            _make_shader(
                path=path,
                name=f"Direct_{path.stem}",
                vertex_sources=vertex_sources,
                pixel_sources=pixel_sources,
                stream_flags=[
                    flags | SHA_NAKED_CODE
                    for _, flags in vertex_calls
                ],
                package="<internal-direct>",
            )
        )
    return shaders


def _make_shader(
    path: Path,
    name: str,
    vertex_sources: list[str],
    pixel_sources: list[str],
    stream_flags: list[int],
    package: str,
) -> ExtractedShader:
    manifest = ShaderManifest(
        path=path.resolve(),
        relative_path=f"Internal/{path.stem}/{name}",
        package=package,
        main_export=f"Shader_{name}",
        descriptor_export="",
        vertex_export=None,
        pixel_export=None,
    )
    descriptor = ShaderDescriptor(
        texture_names=[],
        texcoord_names=[],
        color_names=[],
        float_names=[],
        flag_names=[],
        stream_flags=stream_flags,
        vertex_program_count=len(vertex_sources),
        pixel_program_count=len(pixel_sources),
        shader_info=name,
        vertex_version="vs_1_1",
        pixel_version="ps_1_4",
        vertex_version_value=0xFFFE0101,
        pixel_version_value=0xFFFE0104,
    )
    return ExtractedShader(
        manifest=manifest,
        descriptor=descriptor,
        vertex_variants=[
            _program_variant(index, source)
            for index, source in enumerate(vertex_sources)
        ],
        pixel_variants=[
            _program_variant(index, source, "none")
            for index, source in enumerate(pixel_sources)
        ],
    )


def _program_variant(
    index: int,
    source: str,
    fog_type: str | None = None,
) -> ProgramVariant:
    return ProgramVariant(
        index=index,
        source_sha256=hashlib.sha256(source.encode("latin-1")).hexdigest(),
        source=source,
        fog_type=fog_type,
    )


def _extract_macro_programs(
    source: str,
    macro_pattern: re.Pattern[str],
    assignment_pattern: re.Pattern[str],
) -> dict[str, list[str]]:
    programs: dict[str, list[str]] = {}
    for match in macro_pattern.finditer(source):
        opening = source.find("{", match.end())
        if opening < 0:
            continue
        closing = _matching_brace(source, opening)
        body = source[opening + 1 : closing]
        searchable_body = _mask_comments(body)
        values = []
        for assignment in assignment_pattern.finditer(searchable_body):
            expression, _ = read_c_expression(body, assignment.end())
            value = concatenate_c_string_literals(expression)
            if value:
                values.append(value)
        programs[match.group(1)] = values
    return programs


def _mask_comments(source: str) -> str:
    output = list(source)
    index = 0
    state = "normal"
    while index < len(source):
        char = source[index]
        following = source[index + 1] if index + 1 < len(source) else ""
        if state == "normal":
            if char == "/" and following == "/":
                output[index] = " "
                output[index + 1] = " "
                state = "line_comment"
                index += 2
                continue
            if char == "/" and following == "*":
                output[index] = " "
                output[index + 1] = " "
                state = "block_comment"
                index += 2
                continue
            if char == '"':
                state = "string"
        elif state == "string":
            if char == "\\":
                index += 2
                continue
            if char == '"':
                state = "normal"
        elif state == "line_comment":
            if char == "\n":
                state = "normal"
            else:
                output[index] = " "
        elif state == "block_comment":
            if char == "*" and following == "/":
                output[index] = " "
                output[index + 1] = " "
                state = "normal"
                index += 2
                continue
            if char not in "\r\n":
                output[index] = " "
        index += 1
    return "".join(output)


def _extract_static_char_pointers(source: str) -> dict[str, str]:
    strings: dict[str, str] = {}
    for match in _STATIC_CHAR_POINTER.finditer(source):
        expression, _ = read_c_expression(source, match.end())
        value = concatenate_c_string_literals(expression)
        if value:
            strings[match.group(1)] = value
    return strings


def _matching_brace(source: str, opening: int) -> int:
    depth = 0
    index = opening
    state = "normal"
    while index < len(source):
        char = source[index]
        following = source[index + 1] if index + 1 < len(source) else ""
        if state == "normal":
            if char == "/" and following == "/":
                state = "line_comment"
                index += 2
                continue
            if char == "/" and following == "*":
                state = "block_comment"
                index += 2
                continue
            if char == '"':
                state = "string"
            elif char == "{":
                depth += 1
            elif char == "}":
                depth -= 1
                if depth == 0:
                    return index
        elif state == "string":
            if char == "\\":
                index += 2
                continue
            if char == '"':
                state = "normal"
        elif state == "line_comment":
            if char == "\n":
                state = "normal"
        elif state == "block_comment" and char == "*" and following == "/":
            state = "normal"
            index += 2
            continue
        index += 1
    raise ValueError("Bloque C++ sin llave de cierre")


def _parse_stream_flags(expression: str) -> int:
    flags = 0
    for token in expression.split("|"):
        normalized = token.strip()
        if normalized in _STREAM_FLAGS:
            flags |= _STREAM_FLAGS[normalized]
        elif normalized:
            try:
                flags |= int(normalized, 0)
            except ValueError as error:
                raise ValueError(
                    f"Flag de stream desconocido: {normalized}"
                ) from error
    return flags


def _preprocess(source: str, macros: dict[str, bool]) -> str:
    active_stack = [True]
    branch_stack: list[bool] = []
    output: list[str] = []
    for line in source.splitlines(keepends=True):
        stripped = line.strip()
        if stripped.startswith("#if "):
            condition = stripped[4:].strip()
            branch = _preprocessor_condition(condition, macros)
            branch_stack.append(branch)
            active_stack.append(active_stack[-1] and branch)
            continue
        if stripped.startswith("#ifdef "):
            branch = stripped[7:].strip() in macros
            branch_stack.append(branch)
            active_stack.append(active_stack[-1] and branch)
            continue
        if stripped.startswith("#ifndef "):
            branch = stripped[8:].strip() not in macros
            branch_stack.append(branch)
            active_stack.append(active_stack[-1] and branch)
            continue
        if stripped.startswith("#else"):
            if len(active_stack) == 1:
                raise ValueError("#else sin #if")
            parent_active = active_stack[-2]
            branch_stack[-1] = not branch_stack[-1]
            active_stack[-1] = parent_active and branch_stack[-1]
            continue
        if stripped.startswith("#endif"):
            if len(active_stack) == 1:
                raise ValueError("#endif sin #if")
            active_stack.pop()
            branch_stack.pop()
            continue
        if active_stack[-1]:
            output.append(line)
    if len(active_stack) != 1:
        raise ValueError("Bloque de preprocesador sin #endif")
    return "".join(output)


def _preprocessor_condition(
    expression: str,
    macros: dict[str, bool],
) -> bool:
    normalized = expression.strip()
    if normalized == "0":
        return False
    if normalized == "1":
        return True
    if normalized.startswith("defined(") and normalized.endswith(")"):
        return normalized[8:-1].strip() in macros
    return bool(macros.get(normalized, False))
