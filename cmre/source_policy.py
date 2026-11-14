"""Mechanical source checks for new plain-C recovery; never execution evidence."""
import re

_NONCODE = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S)
_ASSEMBLY = re.compile(r'\b(?:asm|__asm|__asm__)\b')


def inline_assembly_lines(text):
    """Find literal GNU assembly keywords, including macro bodies, excluding prose."""
    # C splices physical lines before recognizing comments or tokens. Retain a
    # physical-line map so a witness still names the original source location.
    characters, locations = [], []
    index, line = 0, 1
    while index < len(text):
        splice = 3 if text.startswith('\\\r\n', index) else 2 if text.startswith('\\\n', index) else 0
        if splice:
            index += splice
            line += 1
            continue
        characters.append(text[index])
        locations.append(line)
        line += text[index] == '\n'
        index += 1
    clean = _NONCODE.sub(lambda match: ' ' * len(match.group()), ''.join(characters))
    return sorted({locations[match.start()] for match in _ASSEMBLY.finditer(clean)})
