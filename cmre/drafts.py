"""Ghidra pseudocode -> cmre C draft (`cmre draft`). Translation input, never evidence.

The converter is purely textual: Ghidra types become cmre.h widths, FUN_/named
functions become cm_fn_<va> with extern prototypes, referenced globals become
CM_GLOBAL macros typed by the decompiler's symbol table, and Ghidra helper
operators get exact-width macros. Constructs C cannot express (registers that
Ghidra could not tie to a parameter, partial-variable pieces, unrecovered jump
tables) are reported as problems for the translator to resolve from the listing.
Acceptance still comes only from `cmre test --c` / `cmre accept --c`.
"""
from __future__ import annotations

import json

import csv
import functools
import re
from pathlib import Path

from . import csource, env, inventory

DRAFTS = env.ANALYSIS / "ghidra-drafts"
OUT = env.ANALYSIS / "cmre" / "c-drafts"

TYPES = {
    "undefined": "u8", "undefined1": "u8", "undefined2": "u16", "undefined3": "u32", "undefined4": "u32",
    "undefined6": "unsigned long long", "undefined7": "unsigned long long", "undefined8": "unsigned long long",
    "byte": "u8", "uchar": "u8", "bool": "u8", "ushort": "u16", "word": "u16", "uint": "u32", "dword": "u32",
    "ulong": "u32", "long": "s32", "ulonglong": "unsigned long long", "longlong": "long long",
    "qword": "unsigned long long", "float10": "long double", "code": "void", "sbyte": "s8",
    "BYTE": "u8", "WORD": "u16", "DWORD": "u32", "UINT": "u32", "ULONG": "u32", "LONG": "s32", "BOOL": "int",
    "HANDLE": "void *", "HWND": "void *", "HDC": "void *", "HINSTANCE": "void *", "HMODULE": "void *",
    "LPVOID": "void *", "PVOID": "void *", "LPCVOID": "const void *", "LPSTR": "char *", "LPCSTR": "const char *",
    "LPDWORD": "u32 *", "LPBYTE": "u8 *", "SIZE_T": "u32", "size_t": "u32", "SOCKET": "u32", "WPARAM": "u32",
    "LPARAM": "s32", "LRESULT": "s32", "HRESULT": "s32", "DWORD_PTR": "u32", "UINT_PTR": "u32", "INT_PTR": "s32",
}
CONVENTIONS = {"__thiscall": "CM_THISCALL", "__stdcall": "CM_STDCALL", "__fastcall": "CM_FASTCALL",
               "__cdecl": "CM_CDECL"}
WIDTH = {1: "u8", 2: "u16", 4: "u32", 8: "unsigned long long"}
SIGNED = {1: "s8", 2: "s16", 4: "s32", 8: "long long"}
UNSUPPORTED = re.compile(r"\b(?:extraout_\w+|unaff_\w+|in_\w+|halt_baddata|UNRECOVERED_JUMPTABLE|swi|trap)\b"
                         r"|\.\_\d+_\d+_|\(\*\(code \*\*?\)")
IDENT = re.compile(r"\b[A-Za-z_][A-Za-z0-9_]*\b")
SUFFIX_VA = re.compile(r"_([0-9a-f]{8})$")


@functools.cache
def _names() -> dict[str, int]:
    seen: dict[str, list[int]] = {}
    for va, f in inventory.load().functions.items():
        seen.setdefault(f.name, []).append(va)
    return {name: vas[0] for name, vas in seen.items() if len(vas) == 1}


def _types(text: str) -> str:
    return IDENT.sub(lambda m: TYPES.get(m.group(0), m.group(0)), text)


def _conventions(text: str) -> str:
    return IDENT.sub(lambda m: CONVENTIONS.get(m.group(0), m.group(0)), text)


def _functions(text: str) -> tuple[str, set[int]]:
    names, used = _names(), set()

    def repl(m):
        va = names.get(m.group(0))
        if va is None:
            return m.group(0)
        used.add(va)
        return f"cm_fn_{va:08x}"
    return IDENT.sub(repl, text), used


def _helper(name: str) -> str | None:
    if m := re.fullmatch(r"CONCAT(\d)(\d)", name):
        hi, lo = int(m[1]), int(m[2])
        if hi + lo not in WIDTH:
            return None
        t = WIDTH[hi + lo]
        low = f"({t})({WIDTH[lo]})(b)" if lo in WIDTH else f"(({t})(b) & {(1 << 8 * lo) - 1:#x}u)"
        return f"#define {name}(a, b) (({t})((({t})({WIDTH.get(hi, t)})(a) << {8 * lo}) | {low}))"
    if m := re.fullmatch(r"SUB(\d)(\d)", name):
        return f"#define {name}(v, off) (({WIDTH.get(int(m[2]), 'u32')})((v) >> (8 * (off))))" if int(m[2]) in WIDTH else None
    if m := re.fullmatch(r"(Z|S)EXT(\d)(\d)", name):
        src, dst = int(m[2]), int(m[3])
        if src not in WIDTH or dst not in WIDTH:
            return None
        cast = (WIDTH if m[1] == "Z" else SIGNED)[src]
        return f"#define {name}(v) (({(WIDTH if m[1] == 'Z' else SIGNED)[dst]})({cast})(v))"
    if m := re.fullmatch(r"CARRY(\d)", name):
        t = WIDTH[int(m[1])]
        return f"#define {name}(a, b) ((u8)(({t})(({t})(a) + ({t})(b)) < ({t})(a)))"
    if m := re.fullmatch(r"SCARRY(\d)", name):
        t, s = WIDTH[int(m[1])], SIGNED[int(m[1])]
        return f"#define {name}(a, b) ((u8)(((({t})(a) ^ ({t})(({t})(a) + ({t})(b))) & (({t})(b) ^ ({t})(({t})(a) + ({t})(b)))) >> ({8 * int(m[1])} - 1)))"
    if m := re.fullmatch(r"SBORROW(\d)", name):
        t = WIDTH[int(m[1])]
        return f"#define {name}(a, b) ((u8)(((({t})(a) ^ ({t})(b)) & (({t})(a) ^ ({t})(({t})(a) - ({t})(b)))) >> ({8 * int(m[1])} - 1)))"
    return None


def _global(name: str, va: int, gtype: str, length: int) -> str:
    if name.startswith("s_") or gtype in ("string", "char[]") or gtype.startswith("char["):
        return f"#define {name} ((char *)0x{va:08x})"
    if m := re.fullmatch(r"(.+?)\s*\[\d+\]", gtype):
        return f"#define {name} (({_types(m[1])} *)0x{va:08x})"
    if gtype in ("pointer", "addr"):
        return f"#define {name} CM_GLOBAL(void *, 0x{va:08x})"
    t = _types(gtype) if gtype else WIDTH.get(length, "u32")
    if t == "u8" and length in WIDTH and gtype.startswith("undefined") and gtype != "undefined":
        t = WIDTH[length]
    return f"#define {name} CM_GLOBAL({t}, 0x{va:08x})"


def _globals_table(va: int) -> dict[int, tuple[str, int]]:
    path = DRAFTS / f"fn_{va:08x}.globals.tsv"
    if not path.exists():
        return {}
    with path.open(encoding="utf-8") as f:
        return {int(r["va"], 16): (r["type"], int(r["length"] or 0)) for r in csv.DictReader(f, delimiter="\t")}


def accepted_prototype(va: int) -> str | None:
    """Declaration text of an accepted C definition, from recovery/c."""
    path = env.C_DIR / f"fn_{va:08x}.c"
    if not path.exists():
        return None
    found = csource.definition(path.read_text(encoding="utf-8"), va)
    if found is None:
        return None
    text = " ".join(csource.strip_attributes("".join(found)).split())
    return re.sub(r"\s+([,)])", r"\1", text)


def prototype(va: int) -> str:
    accepted = accepted_prototype(va)
    if accepted:
        return f"extern {accepted};"
    f = inventory.load().functions.get(va)
    draft = DRAFTS / f"fn_{va:08x}.c"
    if draft.exists():
        # The callee's own decompilation carries the prototype its callers were decompiled against.
        text = re.sub(r"^/\*.*?\*/\s*", "", draft.read_text(encoding="utf-8"), flags=re.S)
        sig = " ".join(text.split("{", 1)[0].split())
        sig = re.sub(rf"\b{re.escape(f.name) if f else f'FUN_{va:08x}'}\b", f"cm_fn_{va:08x}", sig)
        if f"cm_fn_{va:08x}" in sig:
            return f"extern {_conventions(_types(sig))};"
    sig = f.signature if f else f"undefined FUN_{va:08x}(void)"
    sig = re.sub(r"^\s*noreturn\s+", "", sig)
    sig = _conventions(_types(sig))
    sig = re.sub(rf"\b{re.escape(f.name) if f else f'FUN_{va:08x}'}\b", f"cm_fn_{va:08x}", sig)
    sig = re.sub(r"^u8(\s+(CM_\w+\s+)?cm_fn_)", r"void\1", sig) if f and f.signature.startswith("undefined ") else sig
    return f"extern {sig};"


def convert(va: int, text: str | None = None) -> tuple[str, list[str]]:
    """(draft C source, problems) for the function at `va`."""
    if text is None:
        path = DRAFTS / f"fn_{va:08x}.c"
        if not path.exists():
            raise FileNotFoundError(f"no Ghidra draft {env.rel(path)}; run py -3.13 scripts/export_ghidra_drafts.py")
        text = path.read_text(encoding="utf-8")
    body = re.sub(r"^/\*.*?\*/\s*", "", text, flags=re.S).strip()
    problems = sorted(set(m.group(0) for m in UNSUPPORTED.finditer(body)))
    body = re.sub(r"\bWARNING:", "NOTE:", body)
    body, callees = _functions(_conventions(_types(body)))
    callees.discard(va)
    helpers = [h for name in sorted(set(IDENT.findall(body))) if (h := _helper(name))]
    table = _globals_table(va)
    defines = []
    if re.search(r"\bExceptionList\b", body):
        defines.append("#define ExceptionList CM_FS(void *, 0)")  # Ghidra's fs:[0] SEH chain head (ADR 0003)
    for name in sorted(set(IDENT.findall(body))):
        m = SUFFIX_VA.search(name)
        if name.startswith("LAB_") and m and not re.search(rf"\bgoto\s+{name}\b|^\s*{name}\s*:", body, re.M):
            # A code address used as a value (an SEH handler pushed by the prologue), not a goto label.
            defines.append(f"#define {name} CM_GLOBAL(u8, 0x{int(m[1], 16):08x})")
            continue
        if not m or name.startswith(("cm_fn_", "LAB_", "switchD_", "caseD_")):
            continue
        gva = int(m[1], 16)
        gtype, length = table.get(gva, ("", 4))
        defines.append(_global(name, gva, gtype, length))
    header = [f"/* Draft for preferred-image VA 0x{va:08x}, image base 0x00400000, converted from Ghidra 12.1.4",
              "   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against",
              "   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */",
              '#include "cmre.h"', "#ifndef NULL", "#define NULL ((void *)0)", "#endif",
              "#define true 1", "#define false 0"]
    externs = []
    for c in sorted(callees):
        proto = prototype(c)
        count = _argument_count(body, f"cm_fn_{c:08x}")
        if proto.endswith("(void);") and count:
            # GCC 15 defaults to C23, where `()` declares no parameters. Cdecl callers clean the
            # stack, so `(...)` is call-compatible; other conventions need a fixed count.
            params = "..." if not re.search(r"\bCM_(?:STD|THIS|FAST)CALL\b", proto) else ", ".join(["u32"] * count)
            proto = (proto[:-len("(void);")] + f"({params});"
                     "  /* NEEDS-REVIEW: callee draft says (void); these are usually arguments"
                     " MSVC pushed for the next call (right-to-left evaluation, deferred cleanup) */")
        externs.append(proto)
    parts = header + helpers + defines + externs
    if problems:
        parts.append("/* NEEDS-REVIEW: " + ", ".join(problems)[:400] + " */")
    return "\n".join(parts) + "\n\n" + body + "\n", problems


REPAIR_ROUNDS = 8
_DIAGNOSTIC = re.compile(r"^(?:.*[\\/])?draft\.c:(\d+):(\d+): (error|warning|note): (.*)$")
_EXPECTED = re.compile(r"expected '([^']*)'(?: \{aka '([^']*)'\})? but argument is of type '([^']*)'(?: \{aka '([^']*)'\})?")
_ARGUMENT = re.compile(r"(?:pointer targets in )?passing argument (\d+) of '(\w+)'")
_ASSIGNMENT = re.compile(r"(?:assignment to|initialization of) '([^']*)'(?: \{aka '([^']*)'\})? from '([^']*)'"
                         r"(?: \{aka '([^']*)'\})?|pointer targets in (?:assignment|initialization) from '([^']*)'"
                         r"(?: \{aka '([^']*)'\})? to '([^']*)'(?: \{aka '([^']*)'\})?")
_TOO_FEW = re.compile(r"too few arguments to function '(\w+)'; expected (\d+), have (\d+)")
_INT32 = {"int", "unsigned int", "long int", "long unsigned int"}
_INT64 = {"long long int", "long long unsigned int"}


def _compile(source: str) -> list[tuple[int, int, str, str, list[str]]]:
    """(line, column, kind, message, notes) for each diagnostic under the C gate's warning flags."""
    import tempfile

    with tempfile.TemporaryDirectory(prefix="cmre-draft-") as tmp:
        path = Path(tmp) / "draft.c"
        path.write_text(source, encoding="utf-8", newline="\n")
        result = env.run_tool([env.tool("gcc"), "-m32", "-fsyntax-only", str(path), "-Wall", "-Wextra", "-Werror", "-O0",
                               "-ffreestanding", "-fno-builtin", "-fdiagnostics-plain-output",
                               "-fdiagnostics-column-unit=byte", f"-I{env.ROOT / 'recovery/include'}"])
    found = []
    for line in result.stderr.splitlines():
        m = _DIAGNOSTIC.match(line)
        if not m:
            continue
        if m[3] != "note":
            found.append([int(m[1]), int(m[2]), m[3], m[4], []])
        elif found:
            found[-1][4].append(m[4])
            if m[4].startswith("in expansion of macro"):
                # Reported inside a global's #define; the expression is at the expansion site.
                found[-1][0], found[-1][1] = int(m[1]), int(m[2])
    return [tuple(d) for d in found]


def _diagnostics(source: str) -> list[str]:
    return [message for _, _, _, message, _ in _compile(source)]


def _width(name: str, aka: str | None) -> int | None:
    canonical = (aka or name).strip()
    if canonical.endswith("*"):
        return 4
    return 4 if canonical in _INT32 else 8 if canonical in _INT64 else None


def _cast(target: str, target_aka: str | None, source: str, source_aka: str | None) -> str | None:
    """A cast that keeps the value a 32-bit original passes: same width, or the low dword of a
    64-bit (EDX:EAX) declaration. Narrow or unknown types are left for the translator."""
    to, frm = _width(target, target_aka), _width(source, source_aka)
    if to != 4 or frm is None:
        return None
    return f"({target})" if frm == 4 else f"({target})(u32)"


def _arguments(line: str, open_paren: int) -> list[tuple[int, int]]:
    """Spans of the top-level arguments of the call whose '(' is at open_paren (one line only)."""
    spans, depth, start = [], 0, open_paren + 1
    for i in range(open_paren + 1, len(line)):
        ch = line[i]
        if ch in "([":
            depth += 1
        elif ch in ")]":
            if depth == 0:
                spans.append((start, i))
                return spans
            depth -= 1
        elif ch == "," and depth == 0:
            spans.append((start, i))
            start = i + 1
    return []


def _wrap(text: str, cast: str) -> str:
    lead = text[:len(text) - len(text.lstrip())]
    trail = text[len(text.rstrip()):]
    return f"{lead}{cast}({text.strip()}){trail}"


def _fix_argument(line: str, column: int, function: str, index: int, cast: str) -> str | None:
    for m in re.finditer(rf"\b{function}\s*\(", line):
        spans = _arguments(line, m.end() - 1)
        if len(spans) >= index and spans[index - 1][0] <= column <= spans[index - 1][1]:
            lo, hi = spans[index - 1]
            return line[:lo] + _wrap(line[lo:hi], cast) + line[hi:]
    return None


def _fix_assignment(line: str, column: int, cast: str) -> str | None:
    for m in re.finditer(r"(?<![=!<>+\-*/%&|^])=(?!=)", line):
        if m.start() < column - 1:
            continue
        depth = 0
        for i in range(m.end(), len(line)):
            if line[i] in "([":
                depth += 1
            elif line[i] in ")]":
                depth -= 1
            elif line[i] in ";," and depth == 0:
                return line[:m.end()] + _wrap(line[m.end():i], cast) + line[i:]
        return None
    return None


def _ecx_sites(va: int, callee: int) -> list[tuple[int, str, bool]]:
    """(call VA, ECX setup, forwarded) for each direct call to `callee`. The setup is the nearest
    preceding ECX write in address order. `forwarded` holds only when no branch target lies
    between the entry and the call and nothing writes ECX or calls before it, so the call
    receives the entry ECX on every path."""
    from . import lift

    lifted = lift.discover(va)
    insns = sorted(lifted.insns.values(), key=lambda i: i.va)
    targets = {i.target for i in insns if i.kind in ("jcc", "jmp") and i.target is not None}
    targets |= {t for table in lifted.tables.values() for t in table.targets}
    sites = []
    for n, ins in enumerate(insns):
        if ins.kind != "call" or ins.target != callee:
            continue
        setup = "unchanged since entry"
        for prev in reversed(insns[:n]):
            if prev.kind in ("call", "import_call", "indirect_call"):
                setup = f"not written after the call at {prev.va:#010x}"
                break
            if re.match(r"ecx\b", prev.op_str) and prev.mnemonic not in ("cmp", "test", "push"):
                setup = f"{prev.va:#010x} {prev.mnemonic} {prev.op_str}"
                break
        straight = not any(va <= t <= ins.va for t in targets)
        sites.append((ins.va, setup, straight and setup == "unchanged since entry"))
    return sites


def _receiver_note(sites: list[tuple[int, str, bool]], callee: int, convention: str) -> str:
    """Where the listing sets ECX for a call that dropped its receiver; never inferred from C names."""
    return (f"/* NEEDS-REVIEW: receiver missing for {convention} cm_fn_{callee:08x}; "
            + ("; ".join(f"ECX before call at {s:#010x}: {setup}" for s, setup, _ in sites)
               or "no direct call in the listing") + " */")


def _entry_ecx_parameter(va: int, lines: list[str]) -> str | None:
    """Name of this function's ECX parameter (first of a thiscall/fastcall definition), unless the
    C body assigns it: then the name no longer holds the entry ECX."""
    name = f"cm_fn_{va:08x}"
    at = next((i for i, l in enumerate(lines) if not l.startswith("extern ") and re.search(rf"\b{name}\s*\(", l)
               and re.search(r"\bCM_(?:THIS|FAST)CALL\b", l)), None)
    if at is None:
        return None
    m = re.search(rf"\b{name}\s*\(([^,)]*)", lines[at])
    param = re.findall(r"[A-Za-z_]\w*", m[1]) if m else []
    if not param or param[-1] == "void":
        return None
    body = "\n".join(lines[at + 1:])
    if re.search(rf"\b{param[-1]}\s*(?:[-+*/%&|^]?=(?!=)|\+\+|--)|(?:\+\+|--)\s*{param[-1]}\b|&\s*{param[-1]}\b", body):
        return None
    return param[-1]


def _insert_receiver(lines: list[str], callee: str, receiver: str) -> int:
    count = 0
    for i, l in enumerate(lines):
        if l.startswith("extern "):
            continue
        new = re.sub(rf"\b{callee}\s*\(\s*(\)?)", lambda m: f"{callee}({receiver}" + (")" if m[1] else ", "), l)
        if new != l:
            lines[i], count = new, count + l.count(callee)
    return count


def repair(va: int, source: str) -> tuple[str, list[str]]:
    """Compile the draft under the gate's flags and apply only type-level fixes that keep every
    value: casts between 32-bit integer and pointer types (or the low dword of a 64-bit
    declaration), removal of unused declarations, `unused` on write-only Ghidra locals. A missing
    thiscall receiver is filled only when every listing call site forwards the entry ECX and this
    function's ECX parameter is never reassigned; otherwise it is annotated with the ECX setup."""
    lines = source.split("\n")
    notes: list[str] = []
    receivers: dict[str, str] = {}
    for _ in range(REPAIR_ROUNDS):
        by_line: dict[int, list] = {}
        for diagnostic in _compile("\n".join(lines)):
            if diagnostic[2] == "error" and 1 <= diagnostic[0] <= len(lines):
                by_line.setdefault(diagnostic[0], []).append(diagnostic)
        changed = False
        # Bottom-up, and right to left within a line: an edit never moves a pending position.
        for line_no in sorted(by_line, reverse=True):
            for diagnostic in sorted(by_line[line_no], key=lambda d: -d[1]):
                text = lines[line_no - 1]
                fixed, note = _repair_one(va, lines, text, diagnostic, receivers)
                if fixed is None or fixed == text:
                    continue
                notes.append(note)
                changed = True
                if fixed == "":
                    del lines[line_no - 1]
                    break
                lines[line_no - 1] = fixed
        if receivers and not changed:
            # Type errors are settled; supply receivers the listing proves are the entry ECX.
            entry_ecx = _entry_ecx_parameter(va, lines)
            for name, convention in sorted(receivers.items()):
                sites = _ecx_sites(va, int(name[-8:], 16))
                if (convention == "CM_THISCALL" and entry_ecx and sites and all(f for _, _, f in sites)
                        and _insert_receiver(lines, name, entry_ecx)):
                    notes.append(f"receiver of {name}: forwarded entry ECX ({entry_ecx}) at "
                                 + " ".join(f"{s:#010x}" for s, _, _ in sites))
                    del receivers[name]
                    changed = True
        if not changed:
            break
    for name, convention in sorted(receivers.items()):
        at = next((i for i, p in enumerate(lines) if p.startswith("extern ") and re.search(rf"\b{name}\s*\(", p)), None)
        if at is not None:
            lines.insert(at, _receiver_note(_ecx_sites(va, int(name[-8:], 16)), int(name[-8:], 16), convention))
    return "\n".join(lines), notes


def _repair_one(va, lines, text, diagnostic, receivers) -> tuple[str | None, str | None]:
    """(replacement line or "" to delete, note) for one gcc error, or (None, None)."""
    line_no, column, _, message, extra = diagnostic
    if (m := _ARGUMENT.search(message)) and (e := next(filter(None, map(_EXPECTED.search, extra)), None)):
        if cast := _cast(e[1], e[2], e[3], e[4]):
            return (_fix_argument(text, column - 1, m[2], int(m[1]), cast),
                    f"line {line_no}: cast argument {m[1]} of {m[2]} to {cast}")
    elif m := _ASSIGNMENT.search(message):
        target, target_aka, frm, frm_aka = (m[1], m[2], m[3], m[4]) if m[1] else (m[7], m[8], m[5], m[6])
        if cast := _cast(target, target_aka, frm, frm_aka):
            return _fix_assignment(text, column, cast), f"line {line_no}: cast assigned value to {cast}"
    elif m := re.search(r"unused variable '(\w+)'", message):
        if re.fullmatch(rf"\s*[A-Za-z_][\w\s*]*\b{m[1]}\s*(\[[^\]]*\])?\s*;\s*", text):
            return "", f"line {line_no}: removed unused declaration of {m[1]}"
    elif m := re.search(r"variable '(\w+)' set but not used", message):
        return (re.sub(rf"\b{m[1]}\b(\s*\[[^\]]*\])?", rf"{m[1]}\1 __attribute__((unused))", text, count=1),
                f"line {line_no}: marked write-only {m[1]} unused")
    elif (m := _TOO_FEW.search(message)) and int(m[2]) == int(m[3]) + 1:
        proto = next((p for p in lines if p.startswith("extern ") and re.search(rf"\b{m[1]}\s*\(", p)), "")
        if convention := next((c for c in ("CM_THISCALL", "CM_FASTCALL") if c in proto), None):
            receivers.setdefault(m[1], convention)
    return None, None


def _argument_count(body: str, name: str) -> int:
    """Argument count at the first call of `name` (top-level commas), 0 when absent or empty."""
    m = re.search(rf"\b{name}\s*\(", body)
    if not m:
        return 0
    depth, count, seen = 0, 1, False
    for ch in body[m.end():]:
        if ch == "(":
            depth += 1
        elif ch == ")":
            if depth == 0:
                return count if seen else 0
            depth -= 1
        elif ch == "," and depth == 0:
            count += 1
        elif not ch.isspace():
            seen = True
    return 0


def _register_inputs(insns) -> list[str]:
    """Registers read before written, in address order: a calling-convention hint."""
    import capstone

    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    full = {"cx": "ecx", "cl": "ecx", "ch": "ecx", "dx": "edx", "dl": "edx", "dh": "edx", "ax": "eax",
            "al": "eax", "ah": "eax", "si": "esi", "di": "edi", "bx": "ebx", "bl": "ebx", "bh": "ebx"}
    names = {"ecx", "edx", "eax", "ebx", "esi", "edi"}
    written, inputs = set(), []
    for ins in insns:
        decoded = next(md.disasm(ins.raw, ins.va), None)
        if decoded is None:
            continue
        read, wrote = decoded.regs_access()
        operands = decoded.op_str.split(", ")
        if decoded.mnemonic in ("xor", "sub") and len(operands) == 2 and operands[0] == operands[1]:
            read = ()  # self-clear
        for r in read:
            reg = full.get(decoded.reg_name(r), decoded.reg_name(r))
            prologue_save = decoded.mnemonic == "push" and reg in ("ebx", "esi", "edi")
            if reg in names and reg not in written and reg not in inputs and not prologue_save:
                inputs.append(reg)
        written |= {full.get(decoded.reg_name(r), decoded.reg_name(r)) for r in wrote}
        if decoded.group(capstone.CS_GRP_CALL):
            written |= {"eax", "ecx", "edx"}
    return inputs


def legacy_sources(data: dict, function: dict | None = None) -> dict[str, Path]:
    """Authored-file references in shared and per-function frozen ledger layouts."""
    function = function or {}
    return {key: env.ROOT / name for key in ('source', 'header', 'entry_source')
            if (name := function.get(key) or data.get(key))}


def legacy_provenance(entry) -> str:
    """Historical recovery provenance, without inventing a canonical C binding."""
    path = env.ROOT / entry.source
    data = json.loads(path.read_text(encoding='utf-8'))
    references = legacy_sources(data, entry.record)
    authored = '; '.join(f'{kind}={env.rel(source)} sha256='
                         + (env.digest(source) if source.is_file() else 'unavailable')
                         for kind, source in references.items()) or 'authored source unavailable'
    return (f"frozen legacy recovery; {env.rel(path)} sha256={env.digest(path)}; "
            f"symbol={entry.record.get('symbol', '?')}; {authored}; canonical cm_fn binding not inferred")


def brief(va: int, asm: bool = True) -> str:
    """Everything needed to translate one function, in one compact block."""
    from . import ledger, lift, recover, semantic

    inv = inventory.load()
    f = inv.functions[va]
    lifted = lift.discover(va)
    insns = sorted(lifted.insns.values(), key=lambda i: i.va)
    rets = sorted({i.op_str or "0" for i in insns if i.kind == "ret"})
    sites = semantic._call_sites().get(va, ())
    lines = [f"fn_{va:08x} {f.name} body={f.body_bytes}B module={', '.join(inv.modules.get(va, [])) or 'unknown'}",
             f"ghidra signature: {f.signature}",
             f"evidence: ret {'/'.join(rets) or 'none'} (stack bytes popped); registers read before written: "
             f"{' '.join(_register_inputs(insns)) or 'none'}",
             f"callers: {len(sites)} direct" + (f" e.g. {' '.join(f'{s:#010x}' for s in sites[:4])}" if sites else ""),
             f"C profile: {recover.c_profile(va)}, inputs {recover.c_inputs(va)}; EDX read by callers at: "
             f"{' '.join(f'{s:#010x}' for s in semantic.edx_readers(va)[:4]) or 'none'}"]
    if (env.C_DIR / f"fn_{va:08x}.c").exists():
        lines.append(f"existing C: {env.rel(env.C_DIR / f'fn_{va:08x}.c')}")
    legacy = {entry.va: entry for entry in ledger.legacy_entries()}
    if va in legacy:
        entry = legacy[va]
        lines.append(legacy_provenance(entry) + '; normal acceptance retains the legacy record')
    rec = ledger.recovered()
    for c in lifted.callees:
        state = "legacy" if c in legacy else "C" if (env.C_DIR / f"fn_{c:08x}.c").exists() else "lifted" if c in rec else "original"
        lines.append(f"callee {state:8s} {prototype(c)}")
    if lifted.imports:
        lines.append("imports: " + ", ".join(lifted.imports))
    for gva, (gtype, length) in sorted(_globals_table(va).items()):
        lines.append(f"global 0x{gva:08x} {gtype or '?'} ({length} bytes)")
    if va not in legacy:
        try:
            path, problems = write(va)
            lines.append(f"draft: {env.rel(path)}" + (f"  NEEDS-REVIEW: {', '.join(problems)[:200]}" if problems else ""))
        except FileNotFoundError as exc:
            lines.append(f"draft: unavailable ({exc})")
    for p in lifted.problems:
        lines.append(f"PROBLEM {p}")
    if asm:
        lines.append("listing (VA, instruction):")
        lines += [f"{i.va:08x}  {i.mnemonic} {i.op_str}".rstrip() for i in insns]
        lines += [f"table {t.va:08x} -> {' '.join(f'{x:08x}' for x in t.targets)}" for t in lifted.tables.values()]
        lines += [f"data {d.va:08x} {d.raw.hex()}" for d in lifted.data]
    return "\n".join(lines) + "\n"


def write(va: int, out: Path | None = None) -> tuple[Path, list[str]]:
    source, problems = convert(va)
    source, repairs = repair(va, source)
    if repairs:
        head, sep, rest = source.partition("*/\n")
        source = (head + sep + "/* cmre draft repairs (value-preserving casts and declarations; receivers from listing ECX): "
                  + "; ".join(repairs)[:1500] + " */\n" + rest)
    out = out or OUT / f"fn_{va:08x}.c"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(source, encoding="utf-8", newline="\n")
    return out, problems
