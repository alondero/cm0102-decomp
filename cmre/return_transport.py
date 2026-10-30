"""Explicit original CALL execution with owned VM continuation dispatch.

Direct native PUSH/JMP and a declared original CALL each write the original
return word exactly once. A declared ordinary lifted E8 retains its source and
executes at the original CALL by EIP routing. Only an observed near RET from
the active slot may redirect EIP to compiled source. An indirect CALL reads its
target before writing the return slot; selected entries redirect by EIP only.
No original bytes, memory values or general registers are rewritten. This is
a VM execution contract, not evidence of standalone native execution.
"""
from __future__ import annotations

import re
import struct
from dataclasses import asdict, dataclass

import capstone
import unicorn
from unicorn import x86_const as R

from . import env, image, lift

CALL_MARKER = re.compile(r"cmre_call_([0-9a-f]{8})_([0-9a-f]{8})_([0-9a-f]{8})\Z")
RETURN_MARKER = re.compile(r"cmre_return_([0-9a-f]{8})_([0-9a-f]{8})\Z")
INDIRECT_MARKER = re.compile(r"cmre_indirect_([0-9a-f]{8})_([0-9a-f]{8})\Z")

# The pinned original helper copies its return word after probing EAX bytes.
# Its unsigned arithmetic makes the RET slot entryESP-EAX modulo 2^32. This is
# a single original-code contract, not general permission to migrate a frame.
STACK_PROBE = 0x00945610
STACK_PROBE_RET = 0x0094563E
# Identified by hash so the original instructions stay in the local image.
STACK_PROBE_SIZE = 47
STACK_PROBE_SHA256 = '4be32df90e8e3db3f5eed0d31857bddd88255c7ed09ae57e0e20e4510124a255'


def is_stack_probe(code: bytes) -> bool:
    return len(code) == STACK_PROBE_SIZE and env.digest_bytes(code) == STACK_PROBE_SHA256


@dataclass(frozen=True)
class Binding:
    owner: int
    original_call: int
    original_return: int
    original_callee: int
    compiled_push: int
    compiled_continuation: int
    callee_entry: int
    original_bytes: str
    native_bytes: str
    kind: str = 'direct'
    entry_bindings: tuple[tuple[int, int], ...] = ()
    return_slot_rule: str = 'entry-slot'


def _indirect_operand(raw: bytes) -> tuple[str, int]:
    if raw == b'\xff\x10':
        return 'memory', R.UC_X86_REG_EAX
    if raw == b'\xff\x12':
        return 'memory', R.UC_X86_REG_EDX
    if raw == b'\xff\x52\x44':
        return 'memory-edx-plus-44', R.UC_X86_REG_EDX
    if raw == b'\xff\xd0':
        return 'register', R.UC_X86_REG_EAX
    if raw == b'\xff\xd6':
        return 'register', R.UC_X86_REG_ESI
    if raw == b'\xff\xd5':
        return 'register', R.UC_X86_REG_EBP
    if len(raw) == 6 and raw[:2] == b'\xff\x15':
        return 'absolute', struct.unpack('<I', raw[2:])[0]
    raise ValueError('unsupported pinned indirect CALL form')


def bind(recovered: image.Image, symbols: dict[str, int], entries: dict[int, int]) -> tuple[Binding, ...]:
    """Reject malformed transfers or bytes differing from the pinned CALL.

    Marker ownership comes from the original discovered instructions of compiled
    definitions. Direct native JMPs bind the declared original/compiled callee. Indirect JMPs
    admit only pinned CALL [EAX], CALL [EDX], CALL EAX, CALL ESI, CALL EBP
    or CALL [absolute32], plus the exact FF5244 CALL [EDX+44h].
    Its actual dynamic target is observed before the CPU pushes the return word.
    """
    call_names = [name for name in symbols if name.startswith(('cmre_call_', 'cmre_indirect_'))]
    return_names = {name for name in symbols if name.startswith('cmre_return_')}
    if not call_names and not return_names:
        return ()
    decoded = {va: lift.discover(va) for va in entries}
    original = image.primary()
    used_returns, bindings = set(), []
    for name in sorted(call_names):
        indirect = name.startswith('cmre_indirect_')
        match = (INDIRECT_MARKER if indirect else CALL_MARKER).fullmatch(name)
        if not match:
            raise ValueError('malformed call marker')
        values = tuple(int(value, 16) for value in match.groups())
        call, continuation = values[:2]
        callee = 0 if indirect else values[2]
        owners = [va for va, routine in decoded.items() if call in routine.insns]
        if len(owners) != 1:
            raise ValueError('CALL needs one compiled original owner')
        instruction = decoded[owners[0]].insns[call]
        raw = original.read(call, instruction.size if indirect else 5)
        if indirect:
            _indirect_operand(raw)
            if instruction.kind not in ('indirect_call', 'import_call') or continuation != call + len(raw):
                raise ValueError('declared indirect CALL/return differs from supported pinned instruction')
        elif (instruction.kind != 'call' or instruction.size != 5 or raw[0] != 0xe8
                or continuation != call + 5
                or (call + 5 + struct.unpack('<i', raw[1:])[0]) & 0xffffffff != callee):
            raise ValueError('declared CALL/return/callee differs from pinned instruction')
        return_name = f'cmre_return_{call:08x}_{continuation:08x}'
        if return_name not in return_names or not RETURN_MARKER.fullmatch(return_name):
            raise ValueError('missing continuation marker')
        push, resume = symbols[name], symbols[return_name]
        length = 5 if indirect else 10
        if not recovered.text.va <= push < resume < recovered.text.end or resume != push + length:
            raise ValueError('native transfer must have its exact contiguous text size')
        native = recovered.read(push, length)
        target = entries.get(callee, callee)
        if indirect:
            if (native[0] != 0xe9
                    or (push + 5 + struct.unpack('<i', native[1:])[0]) & 0xffffffff != call):
                raise ValueError('native indirect transfer must jump to its pinned original CALL')
        elif (native[0] != 0x68 or struct.unpack('<I', native[1:5])[0] != continuation
                or native[5] != 0xe9
                or (push + 10 + struct.unpack('<i', native[6:])[0]) & 0xffffffff != target):
            raise ValueError('native pair does not push original word and jump to declared binding')
        slot_rule = 'entry-slot'
        if not indirect and callee == target == STACK_PROBE:
            if not is_stack_probe(original.read(STACK_PROBE, STACK_PROBE_SIZE)):
                raise ValueError('original stack-probe instructions differ from pinned contract')
            slot_rule = 'original-stack-probe-eax'
        bindings.append(Binding(owners[0], call, continuation, callee, push, resume, target,
                                raw.hex(), native.hex(), 'indirect' if indirect else 'direct',
                                tuple(sorted(entries.items())) if indirect else (), slot_rule))
        used_returns.add(return_name)
    if used_returns != return_names:
        raise ValueError('orphaned or malformed continuation marker')
    if len({b.original_return for b in bindings}) != len(bindings):
        raise ValueError('ambiguous original continuation ownership')
    continuations = {b.original_return: b for b in bindings}
    for following in bindings:
        previous = continuations.get(following.original_call)
        if (following.kind == 'indirect' and previous is not None
                and previous.owner != following.owner):
            raise ValueError('original indirect CALL overlaps another owner continuation')
    occupied = set()
    for b in bindings:
        span = set(range(b.compiled_push, b.compiled_continuation))
        if occupied & span:
            raise ValueError('overlapping native transfer declarations')
        occupied.update(span)
    return tuple(bindings)


def bind_lifted(recovered: image.Image, symbols: dict[str, int], entries: dict[int, int],
                sites: tuple[int, ...]) -> tuple[Binding, ...]:
    """Bind only declared ordinary E8 sites, retaining their compiled bytes.

    A lifted CALL is executed at its pinned original address by EIP transport.
    The existing source labels still prove the compiled CALL and fallthrough;
    no instruction substitution or permissive round-trip exception is needed.
    Returned bindings exclude any existing marker-owned bindings on the image.
    """
    if (not isinstance(sites, tuple)
            or any(type(site) is not int or not 0 <= site <= 0xffffffff for site in sites)
            or len(set(sites)) != len(sites)):
        raise ValueError('lifted CALL sites require distinct 32-bit instruction VAs')
    if not sites:
        return ()
    original = image.primary()
    decoded = {va: lift.discover(va) for va in entries}
    bindings = []
    for call in sorted(sites):
        owners = [va for va,routine in decoded.items() if call in routine.insns]
        if len(owners) != 1:
            raise ValueError('lifted CALL needs one compiled original owner')
        owner = owners[0]
        instruction = decoded[owner].insns[call]
        raw = original.read(call, 5)
        continuation = call + 5
        if (instruction.kind != 'call' or instruction.size != 5 or raw[0] != 0xe8
                or instruction.raw != raw or continuation not in decoded[owner].insns):
            raise ValueError('lifted site differs from pinned direct CALL and continuation')
        callee = (continuation + struct.unpack('<i', raw[1:])[0]) & 0xffffffff
        if instruction.target != callee:
            raise ValueError('lifted CALL target differs from pinned instruction')
        push, resume = symbols.get(lift.label(call)), symbols.get(lift.label(continuation))
        if push is None or resume is None:
            raise ValueError('lifted CALL needs its instruction and continuation labels')
        entry = entries[owner]
        if (symbols.get(lift.label(owner)) != entry
                or not recovered.text.va <= entry <= push < resume < recovered.text.end
                or resume != push + 5
                or any(entry < following <= push for following in entries.values())):
            raise ValueError('lifted CALL labels do not belong to its compiled owner/fallthrough')
        for va,at in entries.items():
            if (not recovered.text.va <= at < recovered.text.end
                    or symbols.get(lift.symbol(va)) != at):
                raise ValueError('lifted CALL selected entry differs from compiled symbol')
        target = entries.get(callee,callee)
        native = recovered.read(push,5)
        if (native[0] != 0xe8
                or (resume + struct.unpack('<i',native[1:])[0]) & 0xffffffff != target):
            raise ValueError('lifted native CALL differs from pinned target binding')
        slot_rule = 'entry-slot'
        if callee == target == STACK_PROBE:
            if not is_stack_probe(original.read(STACK_PROBE, STACK_PROBE_SIZE)):
                raise ValueError('original stack-probe instructions differ from pinned contract')
            slot_rule = 'original-stack-probe-eax'
        bindings.append(Binding(owner,call,continuation,callee,push,resume,target,
                                raw.hex(),native.hex(),'lifted-direct',
                                tuple(sorted(entries.items())),slot_rule))
    combined = tuple(recovered.return_bindings) + tuple(bindings)
    if (len({b.original_call for b in combined}) != len(combined)
            or len({b.original_return for b in combined}) != len(combined)):
        raise ValueError('ambiguous lifted CALL/continuation ownership')
    continuations = {b.original_return:b for b in combined}
    occupied = set()
    for binding in combined:
        previous = continuations.get(binding.original_call)
        if (binding.kind in ('indirect','lifted-direct') and previous is not None
                and previous.owner != binding.owner):
            raise ValueError('original CALL overlaps another owner continuation')
        span = set(range(binding.compiled_push,binding.compiled_continuation))
        if occupied & span:
            raise ValueError('overlapping native transfer declarations')
        occupied.update(span)
    return tuple(bindings)


def report(bindings: tuple[Binding, ...]) -> dict | None:
    if not bindings:
        return None
    result = {'schema': 'cmre-owned-return-transport/3', 'bindings': [asdict(b) for b in bindings],
            'scope': ('Native direct original-return PUSH/JMP; declared JMP into a pinned '
                      'original indirect CALL; owned 32-bit near-RET VM continuation dispatch; '
                      'instruction-pinned original VA00945610 return-slot migration.'),
            'limitations': ['No standalone native continuation binding or SEH/non-RET unwind support.',
                            'Original selected code stays guarded; unowned arrivals remain escapes.',
                            'Stack-probe migration admits only the unchanged original helper, not a compiled imitation.']}
    if any(b.kind == 'lifted-direct' for b in bindings):
        result['schema'] = 'cmre-owned-return-transport/4'
        result['scope'] += (' Source-preserving ordinary lifted E8 CALL routed by EIP '
                            'to its unchanged pinned original CALL.')
        result['limitations'].append(
            'Lifted CALL routing and its logical instruction budget are VM execution contracts only.')
    return result


@dataclass(frozen=True)
class Frame:
    binding: Binding
    slot: int
    probe_return_slot: int | None = None


@dataclass(frozen=True)
class Pending:
    frame: Frame
    phase: str
    target: int | None


class Runtime:
    """One owned LIFO for marker transfers and source-preserving lifted CALLs.

    Only EIP is redirected. The CPU reads the indirect operand and pushes the
    original return word at the original CALL, including native faults/aliases.
    """
    def __init__(self, uc, bindings: tuple[Binding, ...]):
        self.uc = uc
        self.pushes = {b.compiled_push: b for b in bindings if b.kind == 'direct'}
        self.jumps = {b.compiled_push + 5: b for b in bindings if b.kind == 'direct'}
        self.indirect_jumps = {b.compiled_push: b for b in bindings if b.kind == 'indirect'}
        self.lifted_calls = {b.compiled_push: b for b in bindings if b.kind == 'lifted-direct'}
        self.original_calls = {b.original_call: b for b in bindings
                               if b.kind in ('indirect','lifted-direct')}
        self.returns = {b.original_return: b for b in bindings}
        self.entries = dict(pair for b in bindings for pair in b.entry_bindings)
        self.decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        self.decoder.detail = True
        self.reset()

    def reset(self) -> None:
        self.frames: list[Frame] = []
        self.pending: Pending | None = None
        self.last_ret: tuple[int, int, int, int] | None = None
        self.redirect: tuple[int, int] | None = None
        self.admitted_call: int | None = None
        self.rejection: str | None = None
        self.dispatches = 0

    def _reject(self, reason: str) -> bool:
        self.rejection = reason
        return False

    def redirected(self, address: int) -> bool:
        return self.redirect == (address, self.uc.reg_read(R.UC_X86_REG_EIP))

    def admitted(self, address: int) -> bool:
        return self.admitted_call == address == self.uc.reg_read(R.UC_X86_REG_EIP)

    def _intact(self, binding: Binding) -> bool:
        uc = self.uc
        if bytes(uc.mem_read(binding.compiled_push, len(bytes.fromhex(binding.native_bytes)))).hex() != binding.native_bytes:
            return False
        if binding.return_slot_rule == 'original-stack-probe-eax' and (
                binding.kind not in ('direct','lifted-direct') or binding.original_callee != STACK_PROBE
                or binding.callee_entry != STACK_PROBE
                or not is_stack_probe(bytes(uc.mem_read(STACK_PROBE, STACK_PROBE_SIZE)))):
            return False
        if binding.kind == 'lifted-direct':
            original = bytes.fromhex(binding.original_bytes)
            native = bytes.fromhex(binding.native_bytes)
            if (len(original) != 5 or original[0] != 0xe8 or len(native) != 5
                    or native[0] != 0xe8 or binding.original_return != binding.original_call+5
                    or binding.compiled_continuation != binding.compiled_push+5
                    or (binding.original_return+struct.unpack('<i',original[1:])[0]) & 0xffffffff
                        != binding.original_callee
                    or (binding.compiled_continuation+struct.unpack('<i',native[1:])[0]) & 0xffffffff
                        != binding.callee_entry
                    or dict(binding.entry_bindings).get(binding.original_callee,binding.original_callee)
                        != binding.callee_entry):
                return False
        return (binding.kind == 'direct'
                or bytes(uc.mem_read(binding.original_call, len(bytes.fromhex(binding.original_bytes)))).hex() == binding.original_bytes)

    def _slot_matches(self, frame: Frame, esp: int) -> bool:
        return (esp == frame.slot
                and int.from_bytes(self.uc.mem_read(esp, 4), 'little') == frame.binding.original_return)

    def step(self, address: int, size: int) -> bool:
        uc = self.uc
        self.redirect = None
        self.admitted_call = None
        esp = uc.reg_read(R.UC_X86_REG_ESP)
        binding = self.returns.get(address)
        # Adjacent original CALLs share a continuation/call address. A pending
        # original-call transfer comes from its checked native JMP; an arrival
        # after RET must still satisfy the separate active-slot/cleanup proof.
        if binding is not None and not (
                self.pending and self.pending.phase == 'original_call'
                and self.pending.target == address
                and self.pending.frame.binding.owner == binding.owner):
            if not self.frames or self.frames[-1].binding != binding or self.pending:
                return self._reject('original continuation has no matching active call')
            frame = self.frames[-1]
            if self.last_ret is None:
                return self._reject('arrival did not follow an observed near RET')
            slot, word, cleanup, ret_site = self.last_ret
            expected_slot = frame.slot if frame.probe_return_slot is None else frame.probe_return_slot
            if frame.probe_return_slot is not None and (ret_site != STACK_PROBE_RET or cleanup != 0):
                return self._reject('stack-probe continuation did not follow its pinned near RET')
            if slot != expected_slot or word != address or esp != (slot + 4 + cleanup) & 0xffffffff:
                return self._reject('RET slot/word/post-ESP differs from owned call')
            if not self._intact(binding):
                return self._reject('native transfer or original indirect CALL changed during call')
            self.frames.pop()
            self.redirect = (address, binding.compiled_continuation)
            self.dispatches += 1
            uc.reg_write(R.UC_X86_REG_EIP, binding.compiled_continuation)
            self.last_ret = None
            return True
        self.last_ret = None

        if self.pending:
            pending = self.pending
            frame, phase = pending.frame, pending.phase
            binding = frame.binding
            if pending.target is None or address != pending.target:
                return self._reject('transfer did not enter its declared next instruction/callee')
            if not self._intact(binding):
                return self._reject('native transfer or original indirect CALL changed before entry')
            if phase == 'jump':
                if not self._slot_matches(frame, esp):
                    return self._reject('native PUSH word/slot differs from declaration')
                self.pending = Pending(frame, 'callee', binding.callee_entry)
                return True
            if phase == 'original_call':
                if esp != (frame.slot + 4) & 0xffffffff:
                    return self._reject('original indirect CALL ESP differs from declared JMP')
                if binding.kind == 'lifted-direct':
                    self.pending = Pending(frame,'lifted_callee',binding.original_callee)
                    self.admitted_call = address
                    return True
                # Host observation only: the native CALL still performs the real
                # operand read, return write and any memory/execute fault.
                mode, operand = _indirect_operand(bytes.fromhex(binding.original_bytes))
                if mode == 'register':
                    target = uc.reg_read(operand)
                else:
                    if mode == 'memory-edx-plus-44':
                        operand_address = (uc.reg_read(operand) + 0x44) & 0xffffffff
                    else:
                        operand_address = uc.reg_read(operand) if mode == 'memory' else operand
                    try:
                        target = int.from_bytes(uc.mem_read(operand_address, 4), 'little')
                    except unicorn.UcError:
                        target = None
                self.pending = Pending(frame, 'indirect_callee', target)
                self.admitted_call = address
                return True
            if not self._slot_matches(frame, esp):
                return self._reject('callee return word/ESP differs from native owned slot')
            if binding.return_slot_rule == 'original-stack-probe-eax':
                frame = Frame(binding, frame.slot, (esp - uc.reg_read(R.UC_X86_REG_EAX)) & 0xffffffff)
            self.frames.append(frame)
            self.pending = None
            if phase in ('indirect_callee','lifted_callee') and address in self.entries:
                target = self.entries[address]
                self.redirect = (address, target)
                uc.reg_write(R.UC_X86_REG_EIP, target)
                return True

        binding = self.pushes.get(address)
        if binding is not None:
            if self.pending:
                return self._reject('overlapping pending call transfer')
            if not self._intact(binding):
                return self._reject('native pair differs from compiled declaration')
            self.pending = Pending(Frame(binding, (esp - 4) & 0xffffffff), 'jump', address + 5)
        elif address in self.jumps:
            return self._reject('JMP without its observed native PUSH')

        binding = self.indirect_jumps.get(address)
        if binding is not None:
            if self.pending or not self._intact(binding):
                return self._reject('overlapping or changed indirect transfer')
            self.pending = Pending(Frame(binding, (esp - 4) & 0xffffffff),
                                   'original_call', binding.original_call)
        elif address in self.original_calls:
            return self._reject('original indirect CALL has no owned compiled JMP')

        binding = self.lifted_calls.get(address)
        if binding is not None:
            if self.pending or not self._intact(binding):
                return self._reject('overlapping or changed lifted CALL transfer')
            self.pending = Pending(Frame(binding,(esp-4) & 0xffffffff),
                                   'original_call',binding.original_call)
            self.redirect = (address,binding.original_call)
            uc.reg_write(R.UC_X86_REG_EIP,binding.original_call)
            return True

        if self.frames and 1 <= size <= 15:
            raw = bytes(uc.mem_read(address, size))
            if raw[0] in (0xc2, 0xc3, 0xf2, 0xf3, 0x66, 0x67, 0x2e, 0x3e, 0x26, 0x36, 0x64, 0x65):
                instruction = next(self.decoder.disasm(raw, address), None)
                if instruction and instruction.mnemonic == 'ret' and 0x66 not in instruction.prefix:
                    try:
                        word = int.from_bytes(uc.mem_read(esp, 4), 'little')
                    except unicorn.UcError:
                        return True  # Let the actual RET produce its native fault.
                    cleanup = int(instruction.op_str, 0) if instruction.op_str else 0
                    self.last_ret = (esp, word, cleanup, address)
        return True
