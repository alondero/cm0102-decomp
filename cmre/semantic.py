"""Semantic C comparison support: what a caller or the game can observe (ADR 0002).

The `semantic` profile compares the return value, callee-saved registers, ESP,
FPU control word and stack depth, all non-stack memory, import calls with their
arguments, and a fault's data access. Register allocation, the faulting
instruction, stack bytes and stack addresses stored as values are compiler
layout. Two inputs decide the return comparison: the C definition's declared
return type, and a scan of every direct caller for EDX use after the call.
"""
from __future__ import annotations

import csv
import functools
import re
from dataclasses import dataclass

import capstone
from capstone import x86_const as X

from . import csource, emu, env, image

RETURN_KINDS = ("void", "int8", "int16", "int", "int64", "float")
RETURN_MASK = {"int8": 0xFF, "int16": 0xFFFF}
STACK_MARK = 0x5354_4B00  # canonical stand-in for any test-stack address
STACK_GUARD_MARK = 0x5354_4700  # canonical stand-in for a stack-probe fault below the test stack
# Faults of the pinned original stack probe (return_transport.STACK_PROBE..STACK_PROBE_RET)
# fold to one location. A probe read lies in the page below the stack; the return
# push can fault only in its last dword. The offset in that page is the caller's frame depth.
GUARD_START = emu.STACK - 0x1000
_EDX = {X.X86_REG_EDX, X.X86_REG_DX, X.X86_REG_DL, X.X86_REG_DH}
_SCAN_LIMIT = 24
_BITS = {
    X.X86_REG_EDX: {X.X86_REG_EDX: 0xFFFFFFFF, X.X86_REG_DX: 0xFFFF,
                    X.X86_REG_DL: 0xFF, X.X86_REG_DH: 0xFF00},
    X.X86_REG_EAX: {X.X86_REG_EAX: 0xFFFFFFFF, X.X86_REG_AX: 0xFFFF,
                    X.X86_REG_AL: 0xFF, X.X86_REG_AH: 0xFF00},
}


def in_stack(value: int) -> bool:
    return emu.STACK <= value < emu.STACK + emu.STACK_SIZE


def canonical(value: int) -> int:
    return STACK_MARK if in_stack(value) else value


def canonical_blob(blob: bytes) -> bytes:
    """Aligned dwords holding test-stack addresses become one marker value."""
    if len(blob) % 4:
        return blob
    words = memoryview(blob).cast("I")
    if not any(emu.STACK <= w < emu.STACK + emu.STACK_SIZE for w in words):
        return blob
    return b"".join(canonical(w).to_bytes(4, "little") for w in words)


def declared_return(text: str, va: int) -> str:
    """Return kind from the return type of the cm_fn_<va> definition."""
    found = csource.definition(text, va)
    if found is None:
        raise ValueError(f"no definition of cm_fn_{va:08x} in the C source")
    prefix = csource.strip_attributes(found[0])
    if "*" not in prefix and re.search(r"\bvoid\b", prefix):
        return "void"
    if re.search(r"\b(float|double)\b", prefix):
        return "float"
    if re.search(r"long\s+long|\b[us]64\b|int64", prefix):
        return "int64"
    if "*" not in prefix and re.search(r"\b(u8|s8|char|_Bool|bool)\b", prefix):
        return "int8"
    if "*" not in prefix and re.search(r"\b(u16|s16|short)\b", prefix):
        return "int16"
    return "int"


def _reads_before_write(code: bytes, va: int, observed: set[int], full: int) -> bool:
    """True when straight-line code after a CALL reads an `observed` register before
    overwriting the bits read. The scan stops at the next transfer: a CALL clobbers scratch
    registers; RET/JMP/Jcc are treated as dead (a pass-through wrapper is judged on
    its own callers)."""
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    bits = _BITS[full]
    overwritten = 0
    for count, ins in enumerate(md.disasm(code, va)):
        if count >= _SCAN_LIMIT or ins.group(capstone.CS_GRP_CALL):
            return False
        read, written = ins.regs_access()
        operands = ins.operands
        self_clear = (ins.mnemonic in ("xor", "sub") and len(operands) == 2
                      and all(o.type == X.X86_OP_REG for o in operands)
                      and operands[0].reg == operands[1].reg and operands[0].reg in bits)
        # MSVC passes a char/short argument as `mov dl, x ; push edx`; the callee reads the low part.
        narrow_argument = (bool(overwritten & 0xFF) and ins.mnemonic == "push"
                           and len(operands) == 1 and operands[0].type == X.X86_OP_REG
                           and operands[0].reg == full)
        if not self_clear and not narrow_argument and any(bits[r] & ~overwritten for r in observed & set(read)):
            return True
        if full in written:
            return False
        for register in written:
            overwritten |= bits.get(register, 0)
        if ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_RET) or ins.group(capstone.CS_GRP_IRET):
            return False
    return False


def reads_edx(code: bytes, va: int) -> bool:
    return _reads_before_write(code, va, _EDX, X.X86_REG_EDX)


def reads_eax(code: bytes, va: int) -> bool:
    """True when a caller observes any EAX bit left by a void callee."""
    return _reads_before_write(code, va, set(_BITS[X.X86_REG_EAX]), X.X86_REG_EAX)


def reads_wider_eax(code: bytes, va: int, width: int) -> bool:
    """True when a caller reads EAX bits beyond a declared 8- or 16-bit return."""
    wider = {X.X86_REG_EAX} | ({X.X86_REG_AX, X.X86_REG_AH} if width == 1 else set())
    return _reads_before_write(code, va, wider, X.X86_REG_EAX)


@functools.cache
def _call_sites() -> dict[int, tuple[int, ...]]:
    sites: dict[int, list[int]] = {}
    with env.CALLS_TSV.open(encoding="utf-8") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            if row["reference_type"] == "UNCONDITIONAL_CALL" and row["target_address"]:
                sites.setdefault(int(row["target_address"], 16), []).append(int(row["site_va"], 16))
    return {k: tuple(sorted(v)) for k, v in sites.items()}


def edx_readers(va: int) -> list[int]:
    """Direct call sites whose following instructions read EDX left by this callee."""
    img = image.primary()
    return [site for site in _call_sites().get(va, ()) if reads_edx(img.read(site + 5, 64), site + 5)]


def eax_readers(va: int) -> list[int]:
    """Direct call sites whose following instructions read EAX left by this callee."""
    img = image.primary()
    return [site for site in _call_sites().get(va, ()) if reads_eax(img.read(site + 5, 64), site + 5)]


def wide_eax_readers(va: int, width: int) -> list[int]:
    """Direct call sites that read EAX beyond a declared `width`-byte return."""
    img = image.primary()
    return [site for site in _call_sites().get(va, ())
            if reads_wider_eax(img.read(site + 5, 64), site + 5, width)]


# Indirect CALL/JMP whose target cannot be compared -----------------------------
#
# A slot target that is not a function entry either runs from the middle of an
# original routine (what follows reads registers the compiler chose) or is not
# code. Both runs stop before the transfer; the gate then compares the target and
# what the listing passes: ECX when it loads the object, and the dwords it pushes.

INDIRECT_STOPS = ("interior", "bad-target")
_ECX = {X.X86_REG_ECX, X.X86_REG_CX, X.X86_REG_CL, X.X86_REG_CH}
_SAVED = {X.X86_REG_EBX, X.X86_REG_ESI, X.X86_REG_EDI, X.X86_REG_EBP}
_SCRATCH = {X.X86_REG_EAX, X.X86_REG_ECX, X.X86_REG_EDX}


@dataclass(frozen=True)
class IndirectCall:
    """What the original listing passes to one indirect CALL/JMP: ECX as the object
    pointer, and `args` pushed dwords (None when the listing does not show them)."""
    ecx: bool
    args: int | None


def _cs() -> capstone.Cs:
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    return md


def _is_indirect(ins) -> bool:
    return ins.mnemonic in ("call", "jmp") and ins.operands[0].type != X.X86_OP_IMM


def _is_transfer(ins) -> bool:
    return any(ins.group(g) for g in (capstone.CS_GRP_CALL, capstone.CS_GRP_JUMP,
                                      capstone.CS_GRP_RET, capstone.CS_GRP_IRET))


def _pushed_register(ins) -> int | None:
    op = ins.operands[0] if ins.mnemonic == "push" and ins.operands else None
    return op.reg if op is not None and op.type == X.X86_OP_REG else None


def indirect_sites(code: bytes, va: int) -> list[int]:
    """Indirect CALL/JMP instructions in compiled code, by linear sweep."""
    md = _cs()
    md.skipdata = True
    return [ins.address for ins in md.disasm(code, va) if ins.id and _is_indirect(ins)]


def indirect_calls(insns, entry: int, labels: set[int]) -> dict[int, IndirectCall]:
    """Argument shape of every indirect CALL/JMP in an original function listing.

    `insns` are (va, bytes) pairs; `labels` are branch and jump-table targets. Pushes
    are counted back to the previous transfer, label, `add esp`/`pop` cleanup or the
    entry, so a count can only fall short: pushes for an enclosing call or another
    path are never claimed. Prologue saves of callee-saved registers are not arguments.
    A prologue push of an unwritten scratch register reserves a local, and a non-prologue
    `sub esp`-style adjustment means stored arguments: both make the count unknown. ECX
    is the object when the window's last ECX write is followed by no push and the
    transfer's own operand does not use ECX."""
    md = _cs()
    code = [ins for va, raw in sorted(insns) if (ins := next(md.disasm(raw, va), None))]
    prologue, saves, reserves, written = set(), set(), set(), set()
    for k, ins in enumerate(code):
        if (k and ins.address in labels) or _is_transfer(ins):
            break
        reg = _pushed_register(ins)
        if ins.mnemonic == "push" and reg not in written and reg in _SAVED:
            saves.add(ins.address)
        elif ins.mnemonic == "push" and reg not in written and reg in _SCRATCH:
            reserves.add(ins.address)
        elif ins.mnemonic == "push":
            break  # the first argument push ends the prologue
        prologue.add(ins.address)
        written |= set(ins.regs_access()[1])
    out = {}
    for k, site in enumerate(code):
        if not _is_indirect(site):
            continue
        j, known = k, True
        while j > 0 and code[j].address not in labels:
            prev = code[j - 1]
            if _is_transfer(prev) or prev.mnemonic == "pop":
                break
            if X.X86_REG_ESP in prev.regs_access()[1] and prev.mnemonic != "push":
                cleanup = (prev.mnemonic == "add" and prev.operands[0].type == X.X86_OP_REG
                           and prev.operands[1].type == X.X86_OP_IMM)
                known = cleanup or prev.address in prologue
                break
            j -= 1
        window = code[j:k]
        args, last_push, last_ecx = 0, -1, -1
        for n, ins in enumerate(window):
            if ins.mnemonic == "push":
                if ins.address in reserves or ins.operands[0].size != 4:
                    known = False
                if ins.address not in saves:
                    args, last_push = args + 1, n
            if _ECX & set(ins.regs_access()[1]):
                last_ecx = n
        ecx = last_ecx >= 0 and last_ecx > last_push and not _ECX & set(site.regs_access()[0])
        out[site.address] = IndirectCall(ecx, args if known else None)
    return out
