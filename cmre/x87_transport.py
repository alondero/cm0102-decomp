"""Owned single-instruction execution at original x87 PCs, redirecting EIP only.

The original CPU instruction supplies numeric state, FIP/FDP, status and faults.
This is an explicit original-code dependency, not standalone native recovery.
"""
from __future__ import annotations

import json
import re
from dataclasses import asdict, dataclass

import capstone
from unicorn import x86_const as R

from . import env, image, lift

SITE = re.compile(r'cmre_x87_([0-9a-f]{8})\Z')
CONTINUATION = re.compile(r'cmre_x87_return_([0-9a-f]{8})_([0-9a-f]{8})\Z')
FP_REGISTERS = tuple('FP'+str(i) for i in range(8)) + ('FPCW', 'FPSW', 'FPTAG', 'FIP', 'FDP', 'MXCSR')


def snapshot(uc) -> dict:
    """Actual sampled CPU state, including unmapped numeric FIP and FDP."""
    return {name: uc.reg_read(getattr(R, 'UC_X86_REG_'+name)) for name in FP_REGISTERS}


def difference(original: dict, recovered: dict) -> dict:
    changed = [name for name in FP_REGISTERS if original[name] != recovered[name]]
    return ({'field': 'fp', 'registers': changed, 'original': original, 'recovered': recovered}
            if changed else {})


def require_complete(bindings: tuple[Binding, ...], bodies: dict) -> None:
    """Explicit admission covers every selected x87 site, with no extra owner."""
    expected = {(owner, at) for owner, body in bodies.items() for at, ins in body.insns.items()
                if ins.mnemonic.startswith('f')}
    actual = {(b.owner, b.original_instruction) for b in bindings}
    if actual != expected:
        raise ValueError('x87 admission requires exactly every selected original FP instruction and no extra owners')


def validate_declarations(rows: dict) -> None:
    if not isinstance(rows, dict):
        raise ValueError('original_x87 declarations must be an object')
    for key, row in rows.items():
        if (not isinstance(key, str) or re.fullmatch(r'0x[0-9a-f]{8}', key) is None
                or not isinstance(row, dict) or set(row) != {'bytes', 'evidence'}
                or not isinstance(row['bytes'], str)
                or re.fullmatch(r'(?:[0-9a-f]{2}){1,15}', row['bytes']) is None
                or not isinstance(row['evidence'], str) or not row['evidence'].strip()):
            raise ValueError('invalid original_x87 opcode/evidence declaration')


def _supported(ins) -> bool:
    if ins.mnemonic == 'fnstsw':
        return ins.bytes == b'\xdf\xe0' and ins.op_str == 'ax'
    widths = {'fild': 4, 'fstp': 8, 'fmul': 8, 'fcomp': 8}
    return (ins.mnemonic in widths and len(ins.operands) == 1
            and ins.operands[0].type == capstone.x86_const.X86_OP_MEM
            and ins.operands[0].size == widths[ins.mnemonic])


@dataclass(frozen=True)
class Binding:
    owner: int
    original_instruction: int
    compiled_instruction: int
    original_successor: int
    compiled_continuation: int
    instruction_bytes: str
    original_successor_bytes: str
    compiled_continuation_bytes: str


def bind(recovered: image.Image, symbols: dict[str, int], entries: dict[int, int]) -> tuple[Binding, ...]:
    names = [name for name in symbols if name.startswith('cmre_x87_')]
    if not names:
        return ()
    sites, continuations = {}, {}
    for name in names:
        site, continuation = SITE.fullmatch(name), CONTINUATION.fullmatch(name)
        if site:
            sites[int(site[1], 16)] = symbols[name]
        elif continuation:
            key = int(continuation[1], 16)
            if key in continuations:
                raise ValueError('duplicate x87 continuation marker')
            continuations[key] = (int(continuation[2], 16), symbols[name])
        else:
            raise ValueError('malformed x87 instruction/continuation marker')
    if set(sites) != set(continuations):
        raise ValueError('x87 instruction and continuation markers must pair exactly')
    if len(set(entries.values())) != len(entries):
        raise ValueError('ambiguous compiled x87 owners')
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    decoded = {owner: lift.discover(owner) for owner in entries}
    ordered = sorted(entries.values()) + [recovered.text.end]
    boundaries = {}
    for owner, entry in entries.items():
        end = ordered[ordered.index(entry)+1]
        if not recovered.text.va <= entry < end <= recovered.text.end:
            raise ValueError('compiled x87 owner must occupy executable text')
        boundaries[owner] = {i.address: i for i in decoder.disasm(recovered.read(entry, end-entry), entry)}
    occupied = {at for b in recovered.return_bindings for at in range(b.compiled_push, b.compiled_continuation)}
    occupied |= {at for b in recovered.jump_bindings for at in range(b.compiled_jump, b.compiled_jump+7)}
    original_occupied = set()
    call_arrivals = {b.original_return for b in recovered.return_bindings}
    call_arrivals |= {b.original_call for b in recovered.return_bindings if b.kind == 'indirect'}
    call_arrivals |= {va for b in recovered.return_bindings for va, _ in b.entry_bindings}
    jump_arrivals = {b.original_jump for b in recovered.jump_bindings}
    jump_arrivals |= {va for b in recovered.jump_bindings for va, _ in b.targets}
    primary = image.primary()
    bindings = []
    for site, native in sorted(sites.items()):
        owners = [va for va, body in decoded.items() if site in body.insns]
        if len(owners) != 1:
            raise ValueError('x87 instruction needs one compiled original owner')
        owner = owners[0]
        body = decoded[owner]
        rec = body.insns[site]
        original = next(decoder.disasm(rec.raw, site, count=1), None)
        path = env.HINTS_DIR / f'fn_{owner:08x}.json'
        declarations = json.loads(path.read_text(encoding='utf-8')).get('original_x87', {}) if path.exists() else {}
        validate_declarations(declarations)
        row = declarations.get(f'0x{site:08x}')
        if (body.problems or original is None or not _supported(original)
                or row is None or row['bytes'] != rec.raw.hex()
                or primary.read(site, rec.size) != rec.raw):
            raise ValueError('x87 instruction requires a pinned supported parent declaration')
        successor = site + rec.size
        if successor not in body.insns or {site, successor} & (call_arrivals | jump_arrivals):
            raise ValueError('x87 successor missing or overlaps CALL/JMP ownership')
        starts = boundaries[owner]
        ins = starts.get(native)
        cont = starts.get(native+rec.size)
        if (ins is None or cont is None or ins.size != rec.size
                or recovered.read(native, rec.size) != rec.raw
                or continuations[site] != (successor, native+rec.size)):
            raise ValueError('x87 native instruction/continuation must be exact owned boundaries')
        native_span = set(range(native, native+rec.size))
        original_span = set(range(site, successor))
        if native_span & occupied or original_span & original_occupied:
            raise ValueError('overlapping x87 instruction ownership')
        occupied |= native_span
        original_occupied |= original_span
        bindings.append(Binding(owner, site, native, successor, native+rec.size,
                                rec.raw.hex(), body.insns[successor].raw.hex(), bytes(cont.bytes).hex()))
    return tuple(bindings)


def report(bindings: tuple[Binding, ...]) -> dict | None:
    if not bindings:
        return None
    return {'schema': 'cmre-owned-original-x87/1', 'bindings': [asdict(b) for b in bindings],
            'scope': 'CPU executes each pinned original instruction; EIP-only dispatch preserves actual FP state and faults.',
            'limitations': ['Original x87 instruction bytes remain an execution dependency.',
                            'CALL/JMP arrival overlap and unsupported x87 forms are excluded.',
                            'No host FP-register normalization, native exception delivery or standalone execution claim.']}


class Runtime:
    def __init__(self, uc, bindings: tuple[Binding, ...]):
        self.uc = uc
        self.compiled = {b.compiled_instruction: b for b in bindings}
        self.original = {b.original_instruction for b in bindings}
        self.reset()

    def reset(self) -> None:
        self.pending: tuple[str, Binding] | None = None
        self.redirect: tuple[int, int] | None = None
        self.admission: tuple[int, int] | None = None
        self.rejection: str | None = None
        self.dispatches = 0

    def reject(self, reason: str) -> bool:
        self.rejection = reason
        self.redirect = self.admission = None
        return False

    def redirected(self, address: int) -> bool:
        return self.redirect == (address, self.uc.reg_read(R.UC_X86_REG_EIP))

    def admitted(self, address: int) -> bool:
        return self.admission == (address, self.uc.reg_read(R.UC_X86_REG_EIP))

    def _intact(self, b: Binding) -> bool:
        return all(bytes(self.uc.mem_read(at, len(bytes.fromhex(raw)))).hex() == raw for at, raw in (
            (b.original_instruction, b.instruction_bytes), (b.compiled_instruction, b.instruction_bytes),
            (b.original_successor, b.original_successor_bytes),
            (b.compiled_continuation, b.compiled_continuation_bytes)))

    def step(self, address: int, size: int) -> bool:
        self.redirect = self.admission = None
        if self.rejection:
            return False
        if self.pending:
            phase, b = self.pending
            if not self._intact(b):
                return self.reject('x87 instruction or continuation changed during transfer')
            if phase == 'instruction':
                if address != b.original_instruction or size != len(bytes.fromhex(b.instruction_bytes)):
                    return self.reject('original x87 arrival lacks owned native source')
                self.pending = ('successor', b)
                self.admission = (address, address)
            else:
                if address != b.original_successor or size != len(bytes.fromhex(b.original_successor_bytes)):
                    return self.reject('original x87 did not reach its exact successor')
                self.pending = None
                self.redirect = (address, b.compiled_continuation)
                self.dispatches += 1
                self.uc.reg_write(R.UC_X86_REG_EIP, b.compiled_continuation)
            return True
        b = self.compiled.get(address)
        if b is not None:
            if size != len(bytes.fromhex(b.instruction_bytes)) or not self._intact(b):
                return self.reject('x87 instruction or continuation changed before execution')
            self.pending = ('instruction', b)
            self.redirect = (address, b.original_instruction)
            self.uc.reg_write(R.UC_X86_REG_EIP, b.original_instruction)
        elif address in self.original:
            return self.reject('original x87 instruction has no owned transfer')
        return True

    def completed(self) -> bool:
        if self.pending is not None:
            return self.reject('normal completion bypassed pending original x87 execution')
        return self.rejection is None
