from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path


TOOL_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOL_ROOT))

from shader_inventory.content_scan import _find_references, scan_content
from shader_inventory.manifests import load_manifests


class ReferenceScannerTests(unittest.TestCase):
    def test_finds_ascii_and_utf16_references(self) -> None:
        data = (
            b"Shaders\\Default.sha\0"
            + "Shaders/NormalMap.sha".encode("utf-16le")
        )

        self.assertEqual(
            _find_references(data),
            {
                "Shaders\\Default.sha",
                "Shaders\\NormalMap.sha",
            },
        )

    def test_scans_every_file_and_reports_unknown_extensions(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            client_root = Path(temporary)
            content_root = client_root / "Data"
            content_root.mkdir()
            (content_root / "model.bm").write_bytes(b"Shaders\\Base.sha")
            (content_root / "opaque").write_bytes(b"not a shader")

            result = scan_content([content_root], client_root)

        self.assertEqual(result.file_count, 2)
        self.assertEqual(result.byte_count, 28)
        self.assertEqual(result.extension_counts[".bm"], 1)
        self.assertEqual(result.extension_counts["<sin_extension>"], 1)
        self.assertEqual(len(result.references), 1)


class ManifestTests(unittest.TestCase):
    def test_reads_optional_program_exports(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            client_root = Path(temporary)
            shader_root = client_root / "Shaders"
            shader_root.mkdir()
            (shader_root / "Fixed.sha").write_text(
                "\n".join(
                    (
                        r"Package: TFNM Bin\Shaders.dll",
                        "Name: Shader_Fixed",
                        "Info: Shader_Desc_Fixed",
                    )
                ),
                encoding="utf-8",
            )

            manifests = load_manifests(shader_root)

        self.assertEqual(len(manifests), 1)
        self.assertIsNone(manifests[0].vertex_export)
        self.assertIsNone(manifests[0].pixel_export)


if __name__ == "__main__":
    unittest.main()
