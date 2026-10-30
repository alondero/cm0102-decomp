"""Owned VM dispatch after a byte-identical native indexed JMP reads original data.

Only the actual CPU read from the declared physical slot qualifies an arrival.
The VM changes EIP alone; original tables/code, flags, registers and stack stay
observable. This contract does not establish standalone native execution.
"""
from __future__ import annotations

import json
import re
from dataclasses import asdict, dataclass

import capstone
from unicorn import x86_const as R

from . import env, image, lift

MARKER = re.compile(r'cmre_jump_([0-9a-f]{8})\Z')


@dataclass(frozen=True)
class Binding:
    owner: int
    original_jump: int
    compiled_jump: int
    original_bytes: str
    native_bytes: str
    index_register: str
    displacement: int
    first_slot: int
    slot_count: int
    logical_range: tuple[int, int]
    table_sha256: str
    targets: tuple[tuple[int, int], ...]


def bind(recovered: image.Image, symbols: dict[str, int], entries: dict[int, int]) -> tuple[Binding, ...]:
    """Admit only parent-declared absolute indexed scale-four 32-bit JMPs."""
    names = sorted(name for name in symbols if name.startswith('cmre_jump_'))
    if not names:
        return ()
    if len(set(entries.values())) != len(entries):
        raise ValueError('ambiguous compiled owner entries')
    decoded = {va: lift.discover(va) for va in entries}
    original = image.primary()
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    boundaries = {}
    ordered = sorted(entries.values()) + [recovered.text.end]
    for owner, entry in entries.items():
        end = ordered[ordered.index(entry) + 1]
        if not recovered.text.va <= entry < end <= recovered.text.end:
            raise ValueError('compiled owner must occupy executable text')
        boundaries[owner] = {ins.address: ins for ins in decoder.disasm(recovered.read(entry, end-entry), entry)}
    bindings = []
    # CALL continuation overlap is intentionally outside this bounded contract.
    # Shared physical table slots are allowed; overlapping native instructions
    # or two protocols claiming the same original arrival are not.
    occupied = {at for binding in recovered.return_bindings
                for at in range(binding.compiled_push, binding.compiled_continuation)}
    call_arrivals = {binding.original_return for binding in recovered.return_bindings}
    call_arrivals |= {binding.original_call for binding in recovered.return_bindings if binding.kind == 'indirect'}
    call_arrivals |= {original_entry for binding in recovered.return_bindings
                      for original_entry, _ in binding.entry_bindings}
    target_owners = {}
    for name in names:
        match = MARKER.fullmatch(name)
        if not match:
            raise ValueError('malformed indexed JMP marker')
        site = int(match.group(1), 16)
        owners = [va for va, body in decoded.items() if site in body.insns]
        if len(owners) != 1:
            raise ValueError('indexed JMP needs one compiled original owner')
        owner = owners[0]
        body, starts = decoded[owner], boundaries[owner]
        rec = body.insns[site]
        hint_path = env.HINTS_DIR / f'fn_{owner:08x}.json'
        declarations = json.loads(hint_path.read_text(encoding='utf-8')).get('lift_jump_tables', {}) if hint_path.exists() else {}
        row = declarations.get(f'0x{site:08x}')
        if body.problems or rec.kind != 'table_jmp' or rec.table_anchor is None or row is None:
            raise ValueError('indexed JMP requires an intact parent-declared bounded table')
        raw = original.read(site, rec.size)
        ins = next(decoder.disasm(raw, site, count=1), None)
        if (len(raw) != 7 or raw[:2] != b'\xff\x24' or ins is None
                or len(ins.operands) != 1 or ins.operands[0].type != capstone.x86_const.X86_OP_MEM):
            raise ValueError('unsupported pinned indexed JMP form')
        operand = ins.operands[0].mem
        register = ins.reg_name(operand.index)
        if operand.base or operand.segment or operand.scale != 4 or register not in ('eax', 'ecx', 'edx', 'ebx', 'ebp', 'esi', 'edi'):
            raise ValueError('indexed JMP must use a 32-bit index and absolute scale-four slots')
        low, high = row['index_range']
        displacement = operand.disp & 0xffffffff
        first = (displacement + 4 * low) & 0xffffffff
        slots = original.read(first, 4 * (high-low+1))
        if (raw.hex() != row['bytes'] or env.digest_bytes(slots) != row['table_sha256']
                or rec.table_anchor != (first, low)):
            raise ValueError('indexed JMP opcode/domain/slot identity differs from parent declaration')
        native = symbols[name]
        native_ins = starts.get(native)
        if native_ins is None or native_ins.size != 7 or recovered.read(native, 7) != raw:
            raise ValueError('native indexed JMP must be byte-identical at an owned instruction boundary')
        span = set(range(native, native+7))
        if span & occupied:
            raise ValueError('overlapping native indexed JMP ownership')
        occupied |= span
        targets = []
        for target in sorted({int.from_bytes(slots[n:n+4], 'little') for n in range(0, len(slots), 4)}):
            anchor = symbols.get(lift.label(target))
            if target not in body.insns or anchor not in starts:
                raise ValueError('missing/outside/interior indexed JMP target anchor')
            previous = target_owners.setdefault(anchor, target)
            if previous != target:
                raise ValueError('ambiguous native target anchor')
            targets.append((target, anchor))
        bindings.append(Binding(owner, site, native, raw.hex(), raw.hex(), register,
                                displacement, first, high-low+1, (low, high),
                                row['table_sha256'], tuple(targets)))
    jump_arrivals = {b.original_jump for b in bindings} | {target for b in bindings for target, _ in b.targets}
    if jump_arrivals & call_arrivals:
        raise ValueError('indexed-JMP arrivals overlap owned CALL/RET arrivals; unsupported contract')
    return tuple(bindings)


def report(bindings: tuple[Binding, ...]) -> dict | None:
    if not bindings:
        return None
    return {'schema': 'cmre-owned-indexed-jump/1', 'bindings': [asdict(b) for b in bindings],
            'scope': 'Byte-identical native indexed JMP, actual original dword operand read, owned EIP-only VM target dispatch.',
            'limitations': ['Physical slots use wrapped 32-bit effective addresses; modulo index aliases are admitted.',
                            'Original arrival overlap with owned CALL/RET transport is excluded.',
                            'Original code/table data remain dependencies. Native dispatch and exception unwinds are unproved.']}


@dataclass
class Pending:
    binding: Binding
    slot: int
    target: int | None = None


class Runtime:
    def __init__(self, uc, bindings: tuple[Binding, ...]):
        self.uc = uc
        self.compiled = {b.compiled_jump: b for b in bindings}
        self.original = {b.original_jump for b in bindings} | {target for b in bindings for target, _ in b.targets}
        self.reset()

    def reset(self) -> None:
        self.pending: Pending | None = None
        self.redirect: tuple[int, int] | None = None
        self.rejection: str | None = None
        self.dispatches = 0

    def _reject(self, reason: str) -> bool:
        self.rejection = reason
        return False

    def redirected(self, address: int) -> bool:
        return self.redirect == (address, self.uc.reg_read(R.UC_X86_REG_EIP))

    def observe_read(self, address: int, size: int, value: int) -> None:
        pending = self.pending
        # This callback is reached only from the CPU memory-access hook, never
        # from host mem_read. Source PC and slot are frozen for this token.
        if (pending is not None and pending.target is None and size == 4
                and address == pending.slot
                and self.uc.reg_read(R.UC_X86_REG_EIP) == pending.binding.compiled_jump):
            pending.target = value

    def step(self, address: int, size: int) -> bool:
        self.redirect = None
        if self.pending is not None:
            pending = self.pending
            binding = pending.binding
            target = dict(binding.targets).get(address)
            if pending.target is None or address != pending.target or target is None:
                return self._reject('arrival lacks matching owned CPU indexed-JMP operand read')
            if bytes(self.uc.mem_read(binding.compiled_jump, 7)).hex() != binding.native_bytes:
                return self._reject('native indexed JMP changed before target arrival')
            self.pending = None
            self.redirect = address, target
            self.dispatches += 1
            self.uc.reg_write(R.UC_X86_REG_EIP, target)
            return True
        binding = self.compiled.get(address)
        if binding is not None:
            if size != 7 or bytes(self.uc.mem_read(address, 7)).hex() != binding.native_bytes:
                return self._reject('native indexed JMP changed before execution')
            index = self.uc.reg_read(getattr(R, 'UC_X86_REG_' + binding.index_register.upper()))
            slot = (binding.displacement + 4 * index) & 0xffffffff
            if not (binding.first_slot <= slot < binding.first_slot + 4 * binding.slot_count
                    and (slot-binding.first_slot) % 4 == 0):
                return self._reject('indexed JMP effective address leaves declared physical slots')
            self.pending = Pending(binding, slot)
        elif address in self.original:
            return self._reject('original indexed-JMP site/target has no owned transfer')
        return True

    def completed(self) -> bool:
        if self.pending is not None:
            return self._reject('normal completion bypassed pending indexed-JMP target ownership')
        return True
