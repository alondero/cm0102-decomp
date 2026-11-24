"""Assemble/compile and link recovered code for the emulator, then map it back.

External function symbols (``_cm_fn_<va>``) are bound to the original addresses
with ``--defsym`` so callees remain the original code: an explicit hybrid
boundary, never a substituted behaviour.
"""
from __future__ import annotations

import json
import re
import tempfile
from dataclasses import dataclass
from pathlib import Path

import capstone

from . import env, image, jump_transport, lift, return_transport, x87_transport

HEX = re.compile(r"0x[0-9a-f]+")
# Kinds whose encoding has no PC-relative or label operand, so exact bytes are position-independent.
BYTE_SAFE = {"plain", "ret", "import_call", "import_jmp", "indirect_call", "indirect_jmp"}


@dataclass
class Built:
    image: image.Image
    symbols: dict[str, int]
    entry: int
    log: list[list[str]]


class BuildError(Exception):
    def __init__(self, message: str, stderr: str = ""):
        super().__init__(message)
        self.stderr = stderr


DECORATED = re.compile(r"^[_@]?cm_fn_([0-9a-f]{8})(@\d+)?$")


def validate_lift_calls(declarations: dict) -> tuple[int, ...]:
    """Validate explicit original-CALL declarations; binding checks pinned bytes."""
    if not isinstance(declarations, dict):
        raise ValueError('lift_original_calls must be an object')
    sites = set()
    for key, row in declarations.items():
        if not isinstance(key, str) or not re.fullmatch(r'0x[0-9a-fA-F]{1,8}', key):
            raise ValueError('lift_original_calls requires preferred-image VA keys')
        site = int(key, 16)
        if site in sites:
            raise ValueError('duplicate lift_original_calls VA')
        sites.add(site)
        if (not isinstance(row, dict) or set(row) != {'bytes', 'evidence'}
                or not isinstance(row['bytes'], str)
                or not re.fullmatch(r'e8(?:[0-9a-f]{2}){4}', row['bytes'])
                or not isinstance(row['evidence'], str) or not row['evidence'].strip()):
            raise ValueError('lift_original_calls requires exact E8 bytes and evidence')
    return tuple(sorted(sites))


def _lift_return_sites(lifted: lift.Lifted) -> tuple[int, ...]:
    path = env.HINTS_DIR / f'fn_{lifted.va:08x}.json'
    hints = json.loads(path.read_text(encoding='utf-8')) if path.exists() else {}
    declarations = hints.get('lift_original_calls', {})
    sites = validate_lift_calls(declarations)
    by_va = {int(key, 16): row for key, row in declarations.items()}
    for site in sites:
        instruction = lifted.insns.get(site)
        if (instruction is None or instruction.kind != 'call' or instruction.size != 5
                or instruction.raw.hex() != by_va[site]['bytes']):
            raise ValueError('lift_original_calls bytes/site differ from the pinned direct CALL')
    return sites


def _object_symbols(obj: Path) -> list[tuple[str, str]]:
    """(type, name) pairs from nm; type U = undefined."""
    out = env.run_tool([env.tool("nm"), str(obj)]).stdout
    return [(line.split()[-2], line.split()[-1]) for line in out.splitlines() if len(line.split()) >= 2]


def syntax_probe(text: str, workdir: Path) -> tuple[int, str]:
    """(exit code, diagnostics) of a -fsyntax-only compile of C text under the gate's C flags.

    -Wfloat-conversion reports implicit float-to-integer conversions (ADR 0006 sites)."""
    source = workdir / "probe.c"
    source.write_text(text, encoding="utf-8", newline="")
    cc = [env.tool("gcc"), "-m32", "-fsyntax-only", "-Wfloat-conversion", "-mno-sse3", "-ffreestanding",
          "-fno-builtin", f"-I{env.ROOT / 'recovery/include'}", str(source)]
    r = env.run_tool(cc)
    return r.returncode, r.stderr


def build(source: Path, entry_symbol: str, workdir: Path, opt: str = "-O0", extra_defs: dict[str, int] | None = None) -> Built:
    """Build a .S or .c recovery into a PE linked at env.LINK_BASE.

    Symbol names come from the object file, so MinGW decorations (stdcall
    `_f@N`, fastcall `@f@N`) bind correctly: every undefined `cm_fn_<va>` is
    bound to the original VA, and the entry is the defined `cm_fn_<va>` symbol.
    """
    obj = workdir / (source.stem + ".o")
    exe = workdir / (source.stem + ".exe")
    cc = [env.tool("gcc"), "-m32", "-c", str(source), "-o", str(obj), "-Wall", "-Wextra", "-Werror"]
    if source.suffix == ".c":
        # The emulator maps the whole test stack; do not introduce GCC's external
        # __chkstk_ms dependency when allocating a frame larger than one page.
        # -mno-sse3: float-to-integer casts use FISTP under a chopping control word,
        # as the original __ftol does; Unicorn mis-stores out-of-range FISTTP (ADR 0010).
        cc += [opt, "-mno-sse3", "-mno-stack-arg-probe", "-fno-asynchronous-unwind-tables", "-ffreestanding", "-fno-builtin", f"-I{env.ROOT / 'recovery/include'}"]
    log = [cc]
    r = env.run_tool(cc)
    if r.returncode:
        raise BuildError(f"compile failed: {source.name}", r.stderr)
    own = DECORATED.match(entry_symbol).group(1)
    defs, entry = {}, None
    for kind, name in _object_symbols(obj):
        m = DECORATED.match(name)
        if not m:
            continue
        if kind == "U":
            defs[name] = int(m.group(1), 16)
        elif m.group(1) == own and kind.upper() == "T":
            entry = name
    if entry is None:
        raise BuildError(f"no definition of cm_fn_{own} in {source.name}")
    defs.update(extra_defs or {})
    # A linker script, not --defsym: decorated names contain '@', which --defsym rejects.
    script = workdir / (source.stem + ".externs.ld")
    script.write_text("".join(f'"{k}" = {v:#x};\n' for k, v in sorted(defs.items())), encoding="utf-8", newline="\n")
    ld = [env.tool("ld"), "-mi386pe", "--image-base", hex(env.LINK_BASE), "--entry", entry,
          "--disable-reloc-section", "-o", str(exe), str(obj), str(script)]
    log.append(ld)
    r = env.run_tool(ld)
    if r.returncode:
        raise BuildError(f"link failed: {source.name}", r.stderr)
    nm = env.run_tool([env.tool("nm"), str(exe)])
    symbols = {}
    for line in nm.stdout.splitlines():
        parts = line.split()
        if len(parts) == 3:
            symbols[parts[2]] = int(parts[0], 16)
    img = image.Image(exe.read_bytes(), env.LINK_BASE)
    entries = {}
    for name, address in symbols.items():
        match = DECORATED.match(name)
        if match and img.text.va <= address < img.text.end:
            va = int(match.group(1), 16)
            if va in entries and entries[va] != address:
                raise BuildError('return transport: ambiguous compiled entry binding')
            entries[va] = address
    try:
        img.return_bindings = return_transport.bind(img, symbols, entries)
    except ValueError as exc:
        raise BuildError(f'return transport: {exc}') from exc
    try:
        img.jump_bindings = jump_transport.bind(img, symbols, entries)
    except ValueError as exc:
        raise BuildError(f'jump transport: {exc}') from exc
    try:
        img.x87_bindings = x87_transport.bind(img, symbols, entries)
    except ValueError as exc:
        raise BuildError(f'x87 transport: {exc}') from exc
    return Built(img, symbols, symbols[entry], log)


def round_trip(lifted: lift.Lifted, built: Built) -> list[str]:
    """Compare every compiled instruction with the original after mapping labels.

    Returns the original VAs (as hex strings) whose compiled form differs.
    """
    to_original = {}
    for name, addr in built.symbols.items():
        if name.startswith("v_"):
            to_original[addr] = int(name[2:], 16)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    mismatches = []
    for va, rec in lifted.insns.items():
        addr = built.symbols.get(lift.label(va))
        if addr is None:
            mismatches.append(f"{va:#010x}: label missing")
            continue
        ins = next(md.disasm(built.image.read_upto(addr, 16), addr, count=1), None)
        if ins is None:
            mismatches.append(f"{va:#010x}: undecodable")
            continue
        literal_map = to_original
        if rec.table_anchor is not None:
            first, low = rec.table_anchor
            base = next(iter(rec.substitutions))
            # A logical displacement may overlap unrelated code. Verify its exact
            # relocation from the first real slot, independently for this dispatch.
            relocated = (built.symbols[lift.label(first)] - 4 * low) & 0xffffffff
            if (len(ins.operands) != 1 or ins.operands[0].type != capstone.x86_const.X86_OP_MEM
                    or ins.operands[0].mem.disp & 0xffffffff != relocated):
                mismatches.append(f'{va:#010x}: declared table displacement must relocate to {relocated:#010x}')
                continue
            literal_map = {relocated: base}
        compiled = f"{ins.mnemonic} {HEX.sub(lambda m: _map_literal(m, literal_map), ins.op_str)}".strip()
        original = f"{rec.mnemonic} {rec.op_str}".strip()
        if compiled != original:
            mismatches.append(f"{va:#010x}: {original!r} became {compiled!r}")
    return mismatches


def _map_literal(m: re.Match, to_original: dict[int, int]) -> str:
    value = int(m.group(0), 16)
    return hex(to_original.get(value, value))


def lift_and_build(va: int, workdir: Path, max_rounds: int = 4) -> tuple[lift.Lifted, str, Built]:
    """Lift, build and round-trip; fall back to exact bytes for instructions GAS alters.

    Only instructions without label/branch operands may fall back to bytes.
    """
    lifted = lift.discover(va)
    if lifted.problems:
        return lifted, lift.render(lifted), None
    try:
        return_sites = _lift_return_sites(lifted)
    except ValueError as exc:
        lifted.problems.append(f'return transport declaration: {exc}')
        return lifted, lift.render(lifted), None
    for _ in range(max_rounds):
        text = lift.render(lifted)
        src = workdir / f"fn_{va:08x}.S"
        src.write_text(text, encoding="utf-8", newline="\n")
        try:
            built = build(src, lifted.symbol, workdir)
        except BuildError as exc:
            bad = _error_lines(exc.stderr, text, lifted)
            if not bad:
                lifted.problems.append(f"assembler: {exc.stderr.strip()[:400]}")
                return lifted, text, None
            continue
        bad = []
        for line in round_trip(lifted, built):
            bad.append(int(line.split(":")[0], 16))
        if not bad:
            try:
                if return_sites:
                    built.image.return_bindings += return_transport.bind_lifted(
                        built.image, built.symbols, {lifted.va: built.entry}, return_sites)
            except ValueError as exc:
                lifted.problems.append(f'return transport: {exc}')
                return lifted, text, None
            return lifted, text, built
        if not _fallback(lifted, bad):
            lifted.problems.extend(round_trip(lifted, built))
            return lifted, text, None
    lifted.problems.append("round trip did not converge")
    return lifted, lift.render(lifted), None


def _error_lines(stderr: str, text: str, lifted: lift.Lifted) -> bool:
    lines = text.splitlines()
    bad = []
    for m in re.finditer(r":(\d+): Error", stderr):
        n = int(m.group(1)) - 1
        for k in range(n, -1, -1):
            if lines[k].startswith("v_") and lines[k].endswith(":"):
                bad.append(int(lines[k][2:-1], 16))
                break
    return _fallback(lifted, bad) if bad else False


def _fallback(lifted: lift.Lifted, vas: list[int]) -> bool:
    changed = False
    for va in vas:
        rec = lifted.insns.get(va)
        if rec is None or rec.as_bytes or rec.kind not in BYTE_SAFE or rec.substitutions:
            return False
        rec.as_bytes = True
        changed = True
    return changed


def scratch() -> tempfile.TemporaryDirectory:
    env.RUNS.mkdir(parents=True, exist_ok=True)
    return tempfile.TemporaryDirectory(dir=env.RUNS.parent, prefix="tmp-")
