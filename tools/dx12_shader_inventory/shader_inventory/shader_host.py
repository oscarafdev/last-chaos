from __future__ import annotations

import ctypes
import hashlib
import os
import shutil
import subprocess
from pathlib import Path

from .models import (
    ExtractedShader,
    ProgramVariant,
    ShaderDescriptor,
    ShaderManifest,
)


FOG_TYPES = {
    0: "none",
    1: "opaque",
    2: "non_opaque",
}


class StaticArray(ctypes.Structure):
    _fields_ = [
        ("count", ctypes.c_int32),
        ("items", ctypes.c_void_p),
    ]


class CTString(ctypes.Structure):
    _fields_ = [("value", ctypes.c_void_p)]


class NativeShaderDescriptor(ctypes.Structure):
    _fields_ = [
        ("texture_names", StaticArray),
        ("texcoord_names", StaticArray),
        ("color_names", StaticArray),
        ("float_names", StaticArray),
        ("flag_names", StaticArray),
        ("stream_flags", StaticArray),
        ("vertex_program_count", ctypes.c_int32),
        ("pixel_program_count", ctypes.c_int32),
        ("shader_info", CTString),
        ("vertex_version", ctypes.c_uint32),
        ("pixel_version", ctypes.c_uint32),
    ]


def build_metadata_host(
    project_path: Path,
    output_dir: Path,
    shaders_dll: Path,
) -> Path:
    output_dir.mkdir(parents=True, exist_ok=True)
    engine_dll = output_dir / "Engine.dll"
    copied_shaders = output_dir / "Shaders.dll"
    inputs = [
        project_path,
        project_path.with_name("EngineMetadataStub.cpp"),
        project_path.with_name("EngineMetadataStub.def"),
    ]
    newest_input = max(path.stat().st_mtime_ns for path in inputs)
    requires_build = (
        not engine_dll.exists()
        or engine_dll.stat().st_mtime_ns < newest_input
    )

    if requires_build:
        msbuild = _find_msbuild()
        intermediate_dir = output_dir / "obj"
        command = [
            str(msbuild),
            str(project_path),
            "/nologo",
            "/m",
            "/t:Build",
            "/p:Configuration=Release",
            "/p:Platform=x64",
            f"/p:OutDir={output_dir.resolve()}\\",
            f"/p:IntDir={intermediate_dir.resolve()}\\",
            "/v:minimal",
        ]
        result = subprocess.run(
            command,
            cwd=project_path.parent,
            check=False,
            capture_output=True,
            text=True,
        )
        if result.returncode != 0:
            details = "\n".join(
                part.strip() for part in (result.stdout, result.stderr) if part.strip()
            )
            raise RuntimeError(f"No se pudo compilar el host de metadatos:\n{details}")

    if not engine_dll.exists():
        raise FileNotFoundError(f"MSBuild no produjo {engine_dll}")

    if (
        not copied_shaders.exists()
        or copied_shaders.stat().st_size != shaders_dll.stat().st_size
        or copied_shaders.stat().st_mtime_ns < shaders_dll.stat().st_mtime_ns
    ):
        shutil.copy2(shaders_dll, copied_shaders)
    return output_dir


class ShaderMetadataHost:
    def __init__(self, host_dir: Path):
        self._dll_directory = os.add_dll_directory(str(host_dir.resolve()))
        self._engine = ctypes.WinDLL(str((host_dir / "Engine.dll").resolve()))
        self._shaders = ctypes.WinDLL(str((host_dir / "Shaders.dll").resolve()))
        self._string_destructor = getattr(
            self._engine,
            "??1CTString@@QEAA@XZ",
        )
        self._string_destructor.argtypes = [ctypes.POINTER(CTString)]
        self._string_destructor.restype = None

    def close(self) -> None:
        self._dll_directory.close()

    def extract(self, manifest: ShaderManifest) -> ExtractedShader:
        native = self._read_descriptor(manifest)
        descriptor = ShaderDescriptor(
            texture_names=_read_string_array(native.texture_names),
            texcoord_names=_read_string_array(native.texcoord_names),
            color_names=_read_string_array(native.color_names),
            float_names=_read_string_array(native.float_names),
            flag_names=_read_string_array(native.flag_names),
            stream_flags=_read_uint32_array(native.stream_flags),
            vertex_program_count=native.vertex_program_count,
            pixel_program_count=native.pixel_program_count,
            shader_info=_decode_pointer(native.shader_info.value),
            vertex_version=_format_shader_version(
                native.vertex_version,
                "vs",
            ),
            pixel_version=_format_shader_version(
                native.pixel_version,
                "ps",
            ),
            vertex_version_value=native.vertex_version,
            pixel_version_value=native.pixel_version,
        )
        result = ExtractedShader(manifest=manifest, descriptor=descriptor)

        if manifest.vertex_export:
            function = getattr(self._shaders, manifest.vertex_export)
            function.argtypes = [ctypes.POINTER(CTString), ctypes.c_int32]
            function.restype = None
            for index in range(descriptor.vertex_program_count):
                source = self._read_program(function, index)
                result.vertex_variants.append(_variant(index, source))

        if manifest.pixel_export:
            function = getattr(self._shaders, manifest.pixel_export)
            function.argtypes = [
                ctypes.POINTER(CTString),
                ctypes.c_int32,
                ctypes.c_int32,
            ]
            function.restype = None
            for index in range(descriptor.pixel_program_count):
                for fog_value, fog_name in FOG_TYPES.items():
                    source = self._read_program(function, index, fog_value)
                    result.pixel_variants.append(
                        _variant(index, source, fog_type=fog_name)
                    )
        return result

    def _read_descriptor(self, manifest: ShaderManifest) -> NativeShaderDescriptor:
        function = getattr(self._shaders, manifest.descriptor_export)
        function.argtypes = [
            ctypes.POINTER(ctypes.POINTER(NativeShaderDescriptor)),
        ]
        function.restype = None
        descriptor = ctypes.POINTER(NativeShaderDescriptor)()
        function(ctypes.byref(descriptor))
        if not descriptor:
            raise RuntimeError(
                f"{manifest.descriptor_export} devolvió un descriptor nulo"
            )
        return descriptor.contents

    def _read_program(self, function: object, *arguments: int) -> str:
        output = CTString()
        function(ctypes.byref(output), *arguments)
        try:
            return _decode_pointer(output.value)
        finally:
            self._string_destructor(ctypes.byref(output))


def _variant(
    index: int,
    source: str,
    fog_type: str | None = None,
) -> ProgramVariant:
    return ProgramVariant(
        index=index,
        fog_type=fog_type,
        source_sha256=hashlib.sha256(source.encode("latin-1")).hexdigest(),
        source=source,
    )


def failed_shader(
    manifest: ShaderManifest,
    error: Exception,
) -> ExtractedShader:
    return ExtractedShader(
        manifest=manifest,
        descriptor=ShaderDescriptor(
            texture_names=[],
            texcoord_names=[],
            color_names=[],
            float_names=[],
            flag_names=[],
            stream_flags=[],
            vertex_program_count=0,
            pixel_program_count=0,
            shader_info="",
            vertex_version="",
            pixel_version="",
            vertex_version_value=0,
            pixel_version_value=0,
        ),
        extraction_error=f"{type(error).__name__}: {error}",
    )


def _decode_pointer(pointer: int | None) -> str:
    if not pointer:
        return ""
    return ctypes.string_at(pointer).decode("latin-1")


def _read_string_array(array: StaticArray) -> list[str]:
    if array.count <= 0 or not array.items:
        return []
    values = ctypes.cast(array.items, ctypes.POINTER(CTString))
    return [_decode_pointer(values[index].value) for index in range(array.count)]


def _read_uint32_array(array: StaticArray) -> list[int]:
    if array.count <= 0 or not array.items:
        return []
    values = ctypes.cast(array.items, ctypes.POINTER(ctypes.c_uint32))
    return [values[index] for index in range(array.count)]


def _format_shader_version(value: int, prefix: str) -> str:
    return f"{prefix}_{(value >> 8) & 0xFF}_{value & 0xFF}"


def _find_msbuild() -> Path:
    command = shutil.which("MSBuild.exe") or shutil.which("msbuild")
    if command:
        return Path(command)

    program_files_x86 = Path(
        os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)")
    )
    installer = program_files_x86 / "Microsoft Visual Studio" / "Installer" / "vswhere.exe"
    if installer.exists():
        result = subprocess.run(
            [
                str(installer),
                "-latest",
                "-requires",
                "Microsoft.Component.MSBuild",
                "-find",
                r"MSBuild\**\Bin\MSBuild.exe",
            ],
            check=False,
            capture_output=True,
            text=True,
        )
        candidates = [line.strip() for line in result.stdout.splitlines() if line.strip()]
        if candidates:
            return Path(candidates[0])

    roots = (
        program_files_x86 / "Microsoft Visual Studio",
        Path(os.environ.get("ProgramFiles", r"C:\Program Files"))
        / "Microsoft Visual Studio",
    )
    for root in roots:
        if root.exists():
            candidates = sorted(root.glob("*/**/MSBuild/Current/Bin/MSBuild.exe"))
            if candidates:
                return candidates[-1]
    raise FileNotFoundError("No se encontró MSBuild con las herramientas de C++")
