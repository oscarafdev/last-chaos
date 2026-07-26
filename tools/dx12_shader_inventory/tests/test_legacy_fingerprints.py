from __future__ import annotations

import sys
import unittest
from pathlib import Path


TOOL_ROOT = Path(__file__).resolve().parents[1]
REPOSITORY_ROOT = TOOL_ROOT.parents[1]
sys.path.insert(0, str(TOOL_ROOT))

from shader_inventory.c_source import load_static_char_arrays
from shader_inventory.d3dx9_assembler import D3DX9Assembler
from shader_inventory.internal_programs import load_internal_program_shaders
from shader_inventory.legacy_fingerprints import (
    build_exact_runtime_inventory,
    build_vertex_declaration,
    convert_vertex_program_to_d3d9,
    fnv1a64,
)
from shader_inventory.manifests import load_manifests
from shader_inventory.runtime_catalog import load_runtime_catalog
from shader_inventory.shader_host import (
    ShaderMetadataHost,
    build_metadata_host,
    failed_shader,
)


_NORMALIZATION = (
    "dp3  r8.w,   r1,      r1       \n"
    "rsq  r7.w,   r8.w              \n"
    "mul  r1,     r1,      r7.wwww  \n"
)
_BASE_VERTEX = (
    "dp3  r4.w,   r1,      c4       \n"
    "min  r4.w,   r4.w,    c7.y     \n"
    "max  r4.w,   r4.w,    c7.x     \n"
    "mul  r5.xyz, c5.xyz,  r4.www   \n"
    "add  r5.xyz, r5.xyz,  c6.xyz   \n"
    "mov  r5.w,   c7.y              \n"
    "mul  oD0,    r5,      c7.yyyy  \n"
    "m4x4 oPos, r0,   c0            \n"
    "mov  oT0.xy, v5.xy             \n"
)
_BASE_PIXEL = (
    "texld r0, t0\n"
    "mul r1, r0, c0\n"
    "mul r0.rgb, r1, r1.a\n"
    "mul_x2 r0.rgb, r0, v0\n"
    "+mov r0.a, 1-r1.a\n"
)


class LegacyFingerprintTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.fragments = load_static_char_arrays(
            REPOSITORY_ROOT
            / "client"
            / "src"
            / "Engine"
            / "Graphics"
            / "ShaderCode.h"
        )
        cls.assembler = D3DX9Assembler()

    def test_reproduces_known_base_vertex_fingerprint(self) -> None:
        source = (
            self.fragments["strNoWeights"]
            + _NORMALIZATION
            + _BASE_VERTEX
        )
        bytecode = self.assembler.assemble(
            convert_vertex_program_to_d3d9(source)
        )
        declaration = build_vertex_declaration(0x45)

        self.assertEqual(
            f"{fnv1a64(bytecode + declaration):016X}",
            "BFDAAD52F7C28AAF",
        )

    def test_reproduces_known_base_pixel_fingerprint(self) -> None:
        bytecode = self.assembler.assemble(
            self.fragments["strPixelProgramPrefix14"] + _BASE_PIXEL
        )

        self.assertEqual(
            f"{fnv1a64(bytecode):016X}",
            "000D90AFD69D7DA9",
        )

    def test_finds_internal_terrain_and_effect_programs(self) -> None:
        shaders = load_internal_program_shaders(
            REPOSITORY_ROOT / "client" / "src" / "Engine"
        )
        names = {
            shader.manifest.relative_path
            for shader in shaders
        }

        self.assertIn(
            "Internal/TRShader_2TRL/TRShader_2TRL",
            names,
        )
        self.assertIn(
            "Internal/CTraceEffect/Direct_CTraceEffect",
            names,
        )


class FullCatalogCorrelationTests(unittest.TestCase):
    def test_reproduces_every_catalogued_pair(self) -> None:
        client_root = REPOSITORY_ROOT / "client"
        manifests = load_manifests(client_root / "Shaders")
        host_dir = build_metadata_host(
            TOOL_ROOT / "host" / "EngineMetadataStub.vcxproj",
            REPOSITORY_ROOT
            / ".itconfig"
            / "dx12-shader-inventory"
            / "test-host",
            client_root / "Bin" / "Shaders.dll",
        )
        host = ShaderMetadataHost(host_dir)
        try:
            shaders = []
            for manifest in manifests:
                try:
                    shaders.append(host.extract(manifest))
                except (AttributeError, OSError, RuntimeError) as error:
                    shaders.append(failed_shader(manifest, error))
        finally:
            host.close()

        shaders.extend(
            load_internal_program_shaders(client_root / "src" / "Engine")
        )
        runtime_catalog = load_runtime_catalog(
            client_root
            / "src"
            / "Engine"
            / "Graphics"
            / "DirectX12LegacyShaderFamily.cpp"
        )
        inventory = build_exact_runtime_inventory(
            shaders=shaders,
            shader_code_header=client_root
            / "src"
            / "Engine"
            / "Graphics"
            / "ShaderCode.h",
            shader_source_root=client_root / "src" / "Shaders",
            runtime_catalog=runtime_catalog,
        )

        self.assertEqual(
            inventory["summary"]["implemented_pair_correlation_count"],
            runtime_catalog["implemented_pair_count"],
        )
        self.assertEqual(
            inventory["summary"]["validated_pair_correlation_count"],
            runtime_catalog["validated_pair_count"],
        )
        self.assertEqual(
            inventory["catalog_correlation"]["implemented_pairs_missing"],
            [],
        )


if __name__ == "__main__":
    unittest.main()
