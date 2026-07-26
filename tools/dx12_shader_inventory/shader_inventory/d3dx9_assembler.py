from __future__ import annotations

import ctypes
from pathlib import Path


class ShaderAssemblyError(RuntimeError):
    def __init__(self, hresult: int, message: str):
        super().__init__(f"0x{hresult & 0xFFFFFFFF:08X}: {message.strip()}")
        self.hresult = hresult & 0xFFFFFFFF
        self.message = message.strip()


class D3DX9Assembler:
    """Adaptador mínimo para D3DXAssembleShader, sin crear un dispositivo D3D9."""

    def __init__(self, library_path: Path | None = None):
        path = library_path or Path(
            r"C:\Windows\System32\d3dx9_43.dll"
        )
        if not path.exists():
            raise FileNotFoundError(
                "No se encontró d3dx9_43.dll para ensamblar shaders legacy"
            )
        self._library = ctypes.WinDLL(str(path))
        self._assemble = self._library.D3DXAssembleShader
        self._assemble.argtypes = [
            ctypes.c_char_p,
            ctypes.c_uint32,
            ctypes.c_void_p,
            ctypes.c_void_p,
            ctypes.c_uint32,
            ctypes.POINTER(ctypes.c_void_p),
            ctypes.POINTER(ctypes.c_void_p),
        ]
        self._assemble.restype = ctypes.c_long

    def assemble(self, source: str) -> bytes:
        encoded = source.encode("latin-1")
        output = ctypes.c_void_p()
        errors = ctypes.c_void_p()
        hresult = self._assemble(
            encoded,
            len(encoded),
            None,
            None,
            0,
            ctypes.byref(output),
            ctypes.byref(errors),
        )
        try:
            if hresult < 0:
                message = (
                    self._read_buffer(errors).decode("latin-1", errors="replace")
                    if errors.value
                    else "El ensamblador no devolvió detalles"
                )
                raise ShaderAssemblyError(hresult, message)
            return self._read_buffer(output)
        finally:
            if errors.value:
                self._release(errors)
            if output.value:
                self._release(output)

    @staticmethod
    def _method(
        pointer: ctypes.c_void_p,
        index: int,
        result_type: object,
    ) -> object:
        vtable = ctypes.cast(
            pointer,
            ctypes.POINTER(ctypes.POINTER(ctypes.c_void_p)),
        ).contents
        return ctypes.WINFUNCTYPE(result_type, ctypes.c_void_p)(vtable[index])

    def _read_buffer(self, pointer: ctypes.c_void_p) -> bytes:
        get_pointer = self._method(pointer, 3, ctypes.c_void_p)
        get_size = self._method(pointer, 4, ctypes.c_uint32)
        address = get_pointer(pointer)
        size = get_size(pointer)
        return ctypes.string_at(address, size)

    def _release(self, pointer: ctypes.c_void_p) -> None:
        release = self._method(pointer, 2, ctypes.c_ulong)
        release(pointer)
