"""Tier-1 recovery: mechanically lift one original function to labeled GAS assembly.

The output keeps the original layout order. Each instruction is labeled with its
original VA; jump tables become ``.long`` label tables; data embedded in the
function region is copied as labeled ``.byte`` runs. Direct calls and tail jumps
to other functions use ``_cm_fn_<va>`` symbols, which the test build binds to the
original addresses (hybrid boundary), so callees stay original and explicit.

Lifting refuses (returns problems) rather than guessing: unknown indirect jumps,
references into the middle of instructions, extent disagreement with Ghidra, and
anything outside the function's region all block acceptance.
"""
from __future__ import annotations

import json
import re
from dataclasses import dataclass, field

import capstone
from capstone import x86_const as X

from . import env, image, inventory

NORETURN_IMPORTS = {"KERNEL32.dll!ExitProcess", "KERNEL32.dll!RaiseException", "KERNEL32.dll!ExitThread"}
STOP_MNEMONICS = {"ret", "retf", "iretd", "hlt", "int3", "ud2"}
STRING_OPS = re.compile(r"^((rep|repe|repne|repz|repnz) )?(movs|stos|lods|scas|cmps|ins|outs)[bwd]$")


def symbol(va: int) -> str:
    return f"_cm_fn_{va:08x}"


def label(va: int) -> str:
    return f"v_{va:08x}"


@dataclass
class Insn:
    va: int
    raw: bytes
    mnemonic: str
    op_str: str
    target: int | None = None  # direct branch/call target
    kind: str = "plain"  # plain|jcc|jmp|call|ret|table_jmp|tail_jmp|tail_jcc|import_call|import_jmp|indirect_call|indirect_jmp
    substitutions: dict[int, str] = field(default_factory=dict)  # literal VA -> label
    as_bytes: bool = False
    table_anchor: tuple[int, int] | None = None  # physical first slot, signed first index

    @property
    def size(self) -> int:
        return len(self.raw)


@dataclass
class Table:
    va: int
    targets: list[int]


@dataclass
class DataRun:
    va: int
    raw: bytes


@dataclass
class Lifted:
    va: int
    name: str
    region_end: int
    insns: dict[int, Insn]
    tables: dict[int, Table]
    data: list[DataRun]
    callees: list[int]
    imports: list[str]
    problems: list[str]
    ghidra_body_bytes: int

    @property
    def instruction_bytes(self) -> int:
        return sum(i.size for i in self.insns.values())

    @property
    def symbol(self) -> str:
        return symbol(self.va)

    def labels(self) -> dict[int, str]:
        out = {va: label(va) for va in self.insns}
        out.update({va: label(va) for va in self.tables})
        out.update({d.va: label(d.va) for d in self.data})
        return out


def _cs() -> capstone.Cs:
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    return md


def _mem_operand(ins):
    for op in ins.operands:
        if op.type == X.X86_OP_MEM:
            return op
    return None


def validate_table_hints(tables: dict) -> None:
    """Validate declarations, not their analyst-supplied path-domain proof."""
    if not isinstance(tables, dict):
        raise ValueError('lift_jump_tables must be an object')
    sites = set()
    for site, row in tables.items():
        if not isinstance(site, str) or not re.fullmatch(r'0x[0-9a-fA-F]{1,8}', site):
            raise ValueError('declared jump table requires a VA key')
        address = int(site, 16)
        if address in sites:
            raise ValueError('duplicate declared jump table VA')
        sites.add(address)
        if not isinstance(row, dict) or set(row) != {'bytes', 'index_range', 'table_sha256', 'evidence'}:
            raise ValueError('declared jump table requires bytes, index_range, table_sha256 and evidence')
        bounds = row['index_range']
        if (not isinstance(bounds, list) or len(bounds) != 2
                or any(type(v) is not int or not -0x80000000 <= v <= 0x7fffffff for v in bounds)
                or bounds[0] > bounds[1]):
            raise ValueError('declared jump table index_range must be ascending signed dword bounds')
        if not isinstance(row['bytes'], str) or not re.fullmatch(r'(?:[0-9a-f]{2}){1,15}', row['bytes']):
            raise ValueError('declared jump table requires exact lowercase instruction bytes')
        if not isinstance(row['table_sha256'], str) or not re.fullmatch(r'[0-9a-f]{64}', row['table_sha256']):
            raise ValueError('declared jump table requires a table SHA-256')
        if not isinstance(row['evidence'], str) or not row['evidence'].strip():
            raise ValueError('declared jump table requires instruction-backed domain evidence')


def _declared_table(img, rec, base, row, start, end):
    lo, hi = row['index_range']
    first = (base + 4 * lo) & 0xffffffff
    size = 4 * (hi - lo + 1)
    if rec.raw.hex() != row['bytes']:
        raise ValueError('instruction bytes changed')
    if size > end - start or not start <= first < first + size <= end:
        raise ValueError('physical slots leave function region')
    raw = img.read(first, size)
    if env.digest_bytes(raw) != row['table_sha256']:
        raise ValueError('physical slot bytes changed')
    targets = [int.from_bytes(raw[p:p+4], 'little') for p in range(0,size,4)]
    if any(not start <= target < end for target in targets):
        raise ValueError('target leaves function region')
    return first, lo, targets


def discover(va: int) -> Lifted:
    inv = inventory.load()
    img = image.primary()
    fn = inv.functions.get(va)
    problems: list[str] = []
    if fn is None:
        raise SystemExit(f"{va:#010x} is not a function entry in the pinned inventory")
    region_end = inv.next_entry_after(va)
    md = _cs()
    insns: dict[int, Insn] = {}
    tables: dict[int, Table] = {}
    callees: set[int] = set()
    imports: set[str] = set()
    work = [va]
    hint_path = env.HINTS_DIR / f'fn_{va:08x}.json'
    declarations = json.loads(hint_path.read_text(encoding='utf-8')).get('lift_jump_tables', {}) if hint_path.exists() else {}
    validate_table_hints(declarations)
    declarations = {int(site,16): row for site,row in declarations.items()}
    used_declarations = set()

    def in_region(a: int) -> bool:
        return va <= a < region_end

    while work:
        a = work.pop()
        while a not in insns:
            if not in_region(a):
                problems.append(f"control flow leaves region at {a:#010x}")
                break
            try:
                ins = next(md.disasm(img.read_upto(a, 16), a, count=1))
            except StopIteration:
                problems.append(f"undecodable instruction at {a:#010x}")
                break
            rec = Insn(a, bytes(ins.bytes), ins.mnemonic, ins.op_str)
            insns[a] = rec
            m = ins.mnemonic
            is_jump = ins.group(capstone.CS_GRP_JUMP)
            is_call = ins.group(capstone.CS_GRP_CALL)
            if m in STOP_MNEMONICS or m.startswith("ret"):
                rec.kind = "ret"
                break
            if is_jump or is_call:
                op = ins.operands[0]
                unconditional = m == "jmp"
                if op.type == X.X86_OP_IMM:
                    t = op.imm & 0xFFFFFFFF
                    rec.target = t
                    if is_call:
                        if t == va or t in inv.functions:
                            rec.kind = "call"
                            if t != va:
                                callees.add(t)
                            if t in inv.functions and inv.functions[t].noreturn:
                                break
                        elif in_region(t):
                            problems.append(f"call into own region at {a:#010x} -> {t:#010x}")
                        else:
                            problems.append(f"call to non-entry {t:#010x} at {a:#010x}")
                    elif t in inv.functions and t != va:
                        rec.kind = "tail_jmp" if unconditional else "tail_jcc"
                        callees.add(t)
                        if unconditional:
                            break
                    elif in_region(t):
                        rec.kind = "jmp" if unconditional else "jcc"
                        work.append(t)
                        if unconditional:
                            break
                    else:
                        problems.append(f"branch to {t:#010x} outside region at {a:#010x}")
                        break
                elif op.type == X.X86_OP_MEM and op.mem.base == 0 and op.mem.index == 0 and op.mem.disp in img.imports:
                    name = img.imports[op.mem.disp]
                    imports.add(name)
                    rec.kind = "import_call" if is_call else "import_jmp"
                    if not is_call or name in NORETURN_IMPORTS:
                        break
                elif (
                    is_jump
                    and unconditional
                    and op.type == X.X86_OP_MEM
                    and op.mem.base == 0
                    and op.mem.index != 0
                    and op.mem.scale == 4
                    and in_region(op.mem.disp)
                ):
                    base = op.mem.disp
                    first, low = base, 0
                    if a in declarations:
                        used_declarations.add(a)
                        try:
                            first, low, targets = _declared_table(img, rec, base, declarations[a], va, region_end)
                        except ValueError as exc:
                            problems.append(f'declared jump table at {a:#010x}: {exc}')
                            break
                    else:
                        targets = []
                        p = base
                        while p + 4 <= region_end and in_region(img.dword(p)) and p not in insns:
                            targets.append(img.dword(p))
                            p += 4
                    if not targets:
                        problems.append(f"empty jump table at {base:#010x} from {a:#010x}")
                    if declarations:
                        # Shared/negative domains may name the same physical slots.
                        # Emit each real slot once, with an addressable label.
                        for n, target in enumerate(targets):
                            slot = first + 4 * n
                            tables[slot] = Table(slot, [target])
                        rec.table_anchor = first, low
                    else:
                        tables[base] = Table(base, targets)
                    rec.kind = "table_jmp"
                    rec.substitutions[base] = label(first) if low == 0 else f'({label(first)} - ({4 * low}))'
                    work.extend(targets)
                    break
                else:
                    rec.kind = "indirect_call" if is_call else "indirect_jmp"
                    if not is_call:
                        break
            a += ins.size

    for site in declarations.keys() - used_declarations:
        problems.append(f'declared jump table at {site:#010x}: unused or not an indexed dispatch')
    if declarations:
        slots = sorted(tables)
        if any(b < a + 4 for a,b in zip(slots,slots[1:])):
            problems.append('declared jump table physical slots overlap')
        code = sorted(insns.values(),key=lambda ins: ins.va)
        if any(b.va < a.va + a.size for a,b in zip(code,code[1:])):
            problems.append('declared jump table discovers overlapping instructions')
    # Literal operands pointing inside the region must resolve to a label we emit.
    _check_self_references(md, insns, tables, va, region_end, problems)
    data = _data_runs(img, insns, tables, va, region_end, problems)
    _check_referenced_relative_data(md, insns, data, problems)
    covered = sum(i.size for i in insns.values())
    if covered != fn.body_bytes:
        problems.append(f"decoded {covered} instruction bytes but Ghidra body is {fn.body_bytes}")
    for t in tables.values():
        for p in range(t.va, t.va + 4 * len(t.targets)):
            if any(i.va <= p < i.va + i.size for i in insns.values()):
                problems.append(f"jump table {t.va:#010x} overlaps code")
                break
    return Lifted(va, fn.name, region_end, dict(sorted(insns.items())), tables, data,
                  sorted(callees), sorted(imports), problems, fn.body_bytes)


def _literals(ins) -> list[int]:
    out = []
    for op in ins.operands:
        if op.type == X.X86_OP_IMM:
            out.append(op.imm & 0xFFFFFFFF)
        elif op.type == X.X86_OP_MEM and op.mem.disp:
            out.append(op.mem.disp & 0xFFFFFFFF)
    return out


def _check_self_references(md, insns, tables, va, region_end, problems):
    starts = set(insns) | set(tables)
    for rec in insns.values():
        if rec.kind in ("jcc", "jmp", "call", "tail_jmp", "tail_jcc", "table_jmp"):
            continue
        ins = next(md.disasm(rec.raw, rec.va, count=1))
        for value in _literals(ins):
            if va <= value < region_end:
                if value in starts:
                    rec.substitutions[value] = label(value)
                elif any(i.va < value < i.va + i.size for i in insns.values()):
                    problems.append(f"{rec.va:#010x} references inside instruction at {value:#010x}")
                else:
                    rec.substitutions[value] = label(value)  # data start; emitted by _data_runs


def _data_runs(img, insns, tables, va, region_end, problems) -> list[DataRun]:
    """Copy region bytes not covered by code/tables, split at every referenced address.

    Trailing alignment padding (0xCC/0x90 only) after the last item is dropped.
    """
    covered: dict[int, int] = {i.va: i.size for i in insns.values()}
    covered.update({t.va: 4 * len(t.targets) for t in tables.values()})
    refs = {v for i in insns.values() if i.table_anchor is None
            for v in i.substitutions if v not in insns and v not in tables}
    end_used = max(a + s for a, s in covered.items())
    if any(r >= end_used for r in refs):
        end_used = region_end  # referenced data of unknown length: copy the rest of the region
    occupied = bytearray(end_used - va)
    for a, s in covered.items():
        occupied[a - va : a - va + s] = b"\1" * s
    runs: list[DataRun] = []
    p = va
    while p < end_used:
        if occupied[p - va]:
            p += 1
            continue
        q = p
        while q < end_used and not occupied[q - va]:
            q += 1
        cuts = sorted({p, q} | {r for r in refs if p < r < q})
        for s, e in zip(cuts, cuts[1:]):
            runs.append(DataRun(s, img.read(s, e - s)))
        p = q
    for r in refs:
        if not any(d.va == r for d in runs):
            problems.append(f"data reference {r:#010x} does not start a data run")
    return runs


def _check_referenced_relative_data(md, insns, data, problems):
    """Do not copy a referenced possible callback's PC-relative bytes verbatim.

    This is deliberately a refusal, not code discovery from data. A literal
    pointing at such bytes needs an evidenced auxiliary-code boundary before
    relocation is safe. In fn_0055efe0 the original vector destructor invokes
    the referenced E9 thunk; copying it silently changes its destination.
    """
    refs = {value for rec in insns.values() for value in rec.substitutions}
    for run in data:
        if run.va not in refs:
            continue
        first = next(md.disasm(run.raw, run.va, count=1), None)
        if (first is not None and (first.group(capstone.CS_GRP_JUMP)
                                  or first.group(capstone.CS_GRP_CALL))
                and first.operands and first.operands[0].type == X.X86_OP_IMM):
            problems.append(f"referenced data at {run.va:#010x} begins PC-relative control flow; "
                            "auxiliary code requires evidenced recovery")


def _operand_text(rec: Insn) -> str:
    text = rec.op_str
    if STRING_OPS.match(rec.mnemonic):
        return ""  # GAS takes implicit operands for string instructions
    if rec.kind in ("jcc", "jmp"):
        return label(rec.target)
    if rec.kind in ("call", "tail_jmp", "tail_jcc"):
        return symbol(rec.target)
    for value, name in rec.substitutions.items():
        # Inside [...] a bare symbol is an address; outside it GAS Intel syntax
        # would also read memory, so immediates need `offset`.
        parts = re.split(r"(\[[^\]]*\])", text)
        text = "".join(
            re.sub(rf"\b{value:#x}\b", name if p.startswith("[") else f"offset {name}", p) for p in parts
        )
    return text


def render(lifted: Lifted) -> str:
    lines = [
        f"/* Tier-1 lifted recovery of {lifted.name} at VA {lifted.va:#010x}",
        f"   input {env.TARGET_SHA256}",
        f"   {env.ADDRESS_CONVENTION}. Generated by cmre lift; do not hand-edit:",
        "   regenerate, or write a tier-2 C recovery instead. Each label is the",
        "   original VA of the following instruction or data. */",
        "    .intel_syntax noprefix",
        "    .text",
        f"    .globl {lifted.symbol}",
        f"{lifted.symbol}:",
    ]
    items = [(i.va, "i", i) for i in lifted.insns.values()]
    items += [(t.va, "t", t) for t in lifted.tables.values()]
    items += [(d.va, "d", d) for d in lifted.data]
    for va, kind, item in sorted(items, key=lambda x: x[0]):
        lines.append(f"{label(va)}:")
        if kind == "i":
            if item.as_bytes:
                lines.append(f"    .byte {', '.join(f'0x{b:02x}' for b in item.raw)}  /* {item.mnemonic} {item.op_str} */")
            else:
                operands = _operand_text(item)
                lines.append(f"    {item.mnemonic}{' ' + operands if operands else ''}")
        elif kind == "t":
            lines.extend(f"    .long {label(t)}" for t in item.targets)
        else:
            for k in range(0, len(item.raw), 16):
                lines.append("    .byte " + ", ".join(f"0x{b:02x}" for b in item.raw[k : k + 16]))
    return "\n".join(lines) + "\n"
