from __future__ import annotations

import ast
import re
from pathlib import Path


_STATIC_CHAR_ARRAY = re.compile(
    r"\bstatic\s+char\s+([A-Za-z_]\w*)\s*\[\]\s*="
)


def load_static_char_arrays(path: Path) -> dict[str, str]:
    """Extrae arrays `static char[]` concatenando literales C adyacentes."""
    source = path.read_text(encoding="latin-1")
    arrays: dict[str, str] = {}
    for match in _STATIC_CHAR_ARRAY.finditer(source):
        expression, _ = read_c_expression(source, match.end())
        arrays[match.group(1)] = concatenate_c_string_literals(expression)
    return arrays


def concatenate_c_string_literals(expression: str) -> str:
    pieces: list[str] = []
    index = 0
    while index < len(expression):
        char = expression[index]
        following = expression[index + 1] if index + 1 < len(expression) else ""
        if char == "/" and following == "/":
            newline = expression.find("\n", index + 2)
            index = len(expression) if newline < 0 else newline + 1
            continue
        if char == "/" and following == "*":
            end = expression.find("*/", index + 2)
            index = len(expression) if end < 0 else end + 2
            continue
        if char != '"':
            index += 1
            continue

        end = _string_literal_end(expression, index)
        pieces.append(ast.literal_eval(expression[index:end]))
        index = end
    return "".join(pieces)


def read_c_expression(source: str, start: int) -> tuple[str, int]:
    index = start
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
                index = _string_literal_end(source, index)
                continue
            if char == ";":
                return source[start:index], index + 1
        elif state == "line_comment":
            if char == "\n":
                state = "normal"
        elif state == "block_comment" and char == "*" and following == "/":
            state = "normal"
            index += 2
            continue
        index += 1
    raise ValueError("Expresión C sin punto y coma de cierre")


def _string_literal_end(source: str, start: int) -> int:
    index = start + 1
    while index < len(source):
        if source[index] == "\\":
            index += 2
            continue
        if source[index] == '"':
            return index + 1
        index += 1
    raise ValueError("Literal C sin comillas de cierre")
