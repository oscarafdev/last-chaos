from __future__ import annotations

import hashlib
import re
from collections import Counter
from pathlib import Path
from typing import Callable

from .models import AssetShaderReference, ContentScan


_ASCII_REFERENCE = re.compile(
    rb"Shaders[\\/][A-Za-z0-9_.+\-/\\ ]{1,192}\.sha",
    re.IGNORECASE,
)
_UTF16_REFERENCE = re.compile(
    rb"S\x00h\x00a\x00d\x00e\x00r\x00s\x00(?:\\\x00|/\x00)"
    rb"(?:[A-Za-z0-9_.+\-/\\ ]\x00){1,192}"
    rb"\.\x00s\x00h\x00a\x00",
    re.IGNORECASE,
)
_CHUNK_SIZE = 1024 * 1024
_OVERLAP_SIZE = 1024


def scan_content(
    content_roots: list[Path],
    client_root: Path,
    progress: Callable[[int, int, int], None] | None = None,
) -> ContentScan:
    files = _collect_files(content_roots, client_root)
    snapshot = hashlib.sha256()
    extension_counts: Counter[str] = Counter()
    references: list[AssetShaderReference] = []
    errors: list[dict[str, str]] = []
    byte_count = 0

    for file_index, (relative_path, path) in enumerate(files, start=1):
        extension = path.suffix.lower() or "<sin_extension>"
        extension_counts[extension] += 1
        snapshot.update(relative_path.encode("utf-8", errors="surrogateescape"))
        snapshot.update(b"\0")
        file_references: set[str] = set()
        tail = b""
        try:
            with path.open("rb") as stream:
                while chunk := stream.read(_CHUNK_SIZE):
                    byte_count += len(chunk)
                    snapshot.update(chunk)
                    window = tail + chunk
                    file_references.update(_find_references(window))
                    tail = window[-_OVERLAP_SIZE:]
        except OSError as error:
            errors.append(
                {
                    "path": relative_path,
                    "error": f"{type(error).__name__}: {error}",
                }
            )
            snapshot.update(b"\0ERROR\0")
            snapshot.update(str(error).encode("utf-8", errors="replace"))

        for shader_path in sorted(file_references, key=str.lower):
            references.append(
                AssetShaderReference(
                    asset_path=relative_path,
                    shader_path=shader_path,
                )
            )
        if progress and (file_index % 2500 == 0 or file_index == len(files)):
            progress(file_index, len(files), byte_count)

    return ContentScan(
        roots=[
            path.resolve().relative_to(client_root.resolve()).as_posix()
            for path in content_roots
        ],
        file_count=len(files),
        byte_count=byte_count,
        snapshot_sha256=snapshot.hexdigest(),
        extension_counts=dict(
            sorted(extension_counts.items(), key=lambda item: (-item[1], item[0]))
        ),
        references=references,
        errors=errors,
    )


def canonical_shader_path(value: str) -> str:
    return value.replace("\\", "/").lower()


def _collect_files(
    content_roots: list[Path],
    client_root: Path,
) -> list[tuple[str, Path]]:
    files: list[tuple[str, Path]] = []
    resolved_client = client_root.resolve()
    for root in content_roots:
        if not root.exists():
            raise FileNotFoundError(f"No existe la raíz de contenido {root}")
        for path in root.rglob("*"):
            if path.is_file():
                relative = path.resolve().relative_to(resolved_client).as_posix()
                files.append((relative, path))
    files.sort(key=lambda item: item[0].lower())
    return files


def _find_references(data: bytes) -> set[str]:
    references = {
        _normalize_reference(match.group().decode("ascii"))
        for match in _ASCII_REFERENCE.finditer(data)
    }
    for match in _UTF16_REFERENCE.finditer(data):
        decoded = match.group()[::2].decode("ascii")
        references.add(_normalize_reference(decoded))
    return references


def _normalize_reference(value: str) -> str:
    normalized = value.replace("/", "\\")
    prefix, separator, remainder = normalized.partition("\\")
    if not separator:
        return normalized
    return f"{prefix.capitalize()}\\{remainder}"
