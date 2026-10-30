"""Locate the cm_fn_<va> definition in tier-2 C source text.

Shared by the semantic gate (declared return type) and translation briefs
(callee prototypes). Comments and preprocessor directives are blanked first;
a directive extends over backslash-continued lines, so macro bodies (including
inline-assembly statements in legacy abi records) never leak into the text
before a definition.
"""
from __future__ import annotations

import re

_LITERAL_OR_COMMENT = re.compile(r'''("(?:\\.|[^"\\\n])*"|'(?:\\.|[^'\\\n])*')|/\*.*?\*/|//[^\n]*''', re.S)
_DIRECTIVE = re.compile(r"^[ \t]*#(?:[^\n]*\\\n)*[^\n]*", re.M)


def _blank(text: str) -> str:
    """Spaces for every character except newlines, so offsets and line structure survive."""
    return re.sub(r"[^\n]", " ", text)


def code(text: str) -> str:
    """`text` with comments and preprocessor directives blanked; quoted literals kept."""
    text = _LITERAL_OR_COMMENT.sub(lambda m: m.group(1) or _blank(m.group(0)), text)
    return _DIRECTIVE.sub(lambda m: _blank(m.group(0)), text)


def strip_attributes(text: str) -> str:
    """Remove `__attribute__((...))` specifiers, balancing nested parentheses."""
    out, i = [], 0
    while (j := text.find("__attribute__", i)) >= 0:
        out.append(text[i:j])
        k = text.find("(", j)
        if k < 0:
            return "".join(out) + text[j:]
        depth = 0
        while k < len(text):
            depth += {"(": 1, ")": -1}.get(text[k], 0)
            k += 1
            if depth == 0:
                break
        i = k
    return "".join(out) + text[i:]


def definition(text: str, va: int) -> tuple[str, str] | None:
    """(declaration specifiers, declarator) of the cm_fn_<va> definition, or None.

    The specifiers run from the previous `;`, `{` or `}` to the name; the
    declarator runs from the name through its parameter list. Attributes stay in
    both; callers strip them where they would change the reading."""
    text = code(text)
    for match in re.finditer(rf"\bcm_fn_{va:08x}\s*\(", text):
        depth, i = 1, match.end()
        while i < len(text) and depth:
            depth += {"(": 1, ")": -1}.get(text[i], 0)
            i += 1
        if not strip_attributes(text[i:i + 400]).lstrip().startswith("{"):
            continue  # declaration or call, not the definition
        start = max(text.rfind(c, 0, match.start()) for c in ";{}")
        return text[start + 1:match.start()], text[match.start():i]
    return None
