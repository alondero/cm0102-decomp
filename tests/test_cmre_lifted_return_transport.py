"""Isolated VM seam tests; synthetic callees do not establish game behavior."""
from dataclasses import replace
from pathlib import Path
from types import SimpleNamespace

import pytest
import unicorn
from unicorn import x86_const as R

from cmre import build, emu, env, image, lift, return_transport

OWNER, CALL, RETURN, CALLEE = 0x005230C0, 0x005230C3, 0x005230C8, 0x00523300
ROOT = Path(__file__).resolve().parents[1]


@pytest.fixture(autouse=True)
def isolated_imports():
    assert env.ROOT.resolve() == ROOT
    assert Path(return_transport.__file__).resolve().is_relative_to(ROOT)


def toy(tmp_path, body='ret', continuation='mov eax, dword ptr [esp-4]',
        call='call _cm_fn_00523300'):
    source = tmp_path / 'lifted-call.S'
    source.write_text(f'''.intel_syntax noprefix
.text
.globl _cm_fn_005230c0
_cm_fn_005230c0:
v_005230c0:
 push esi
 mov esi, ecx
v_005230c3:
 {call}
v_005230c8:
 {continuation}
 pop esi
 ret 4
.globl _cm_fn_00523300
_cm_fn_00523300:
v_00523300:
 {body}
''', encoding='utf-8')
    compiled = build.build(source, lift.symbol(OWNER), tmp_path)
    entries = {va: compiled.symbols[lift.symbol(va)] for va in (OWNER, CALLEE)}
    return source, compiled, entries


def bind(compiled, entries, sites=(CALL,), symbols=None):
    return return_transport.bind_lifted(compiled.image,
        compiled.symbols if symbols is None else symbols, entries, sites)


def machine(compiled, bindings):
    compiled.image.return_bindings = bindings
    vm = emu.Machine(image.primary(), compiled.image,
                     watch=(OWNER, lift.discover(OWNER).region_end), escape=True)
    vm.uc.hook_add(unicorn.UC_HOOK_CODE, vm._escape,
                   begin=CALLEE, end=lift.discover(CALLEE).region_end-1)
    return vm


def case():
    return emu.Case({'eflags': 0x602, 'esi': 0x12345678, 'ecx': 0x03010E58},
                    [0]*10, bytes(emu.ARENA_SIZE))


def test_lifted_direct_call_writes_original_word_once_and_preserves_unaligned_reads(tmp_path):
    body = ('push esi\n movzx ebx, byte ptr [esp+4]\n'
            'movzx edi, word ptr [esp+3]\n mov edx, dword ptr [esp+1]\n pop esi\n ret')
    source, compiled, entries = toy(tmp_path, body)
    before = source.read_bytes(), compiled.image.read(compiled.image.text.va, 64)
    bindings = bind(compiled, entries)
    assert len(bindings) == 1
    binding = bindings[0]
    assert binding.kind == 'lifted-direct'
    assert binding.compiled_push == compiled.symbols[lift.label(CALL)]
    assert binding.compiled_continuation == binding.compiled_push+5
    assert binding.original_callee == CALLEE
    assert dict(binding.entry_bindings) == entries
    vm = machine(compiled, bindings)
    original = image.primary().read(CALL, 32)
    result = vm.run(compiled.entry, case(), 1000)
    assert result.outcome == 'return', (result.outcome, result.detail)
    assert result.regs[0] == RETURN
    assert result.regs[2] == 0xC803010E
    assert result.regs[3] == 0xC8
    assert result.regs[7] == 0xC803
    assert result.regs[6] == 0x12345678
    assert result.regs[8] == 0x602
    slot = emu.STACK_TOP-8
    assert [event for event in result.events if event[:3] == ('w',slot,4)] == [
        ('w',slot,4,RETURN)]
    assert ('r',slot-3,4,0xC803010E) in result.events
    assert bytes(vm.uc.mem_read(CALL,32)) == original
    assert source.read_bytes() == before[0]
    assert compiled.image.read(compiled.image.text.va,64) == before[1]
    assert vm.return_runtime.dispatches == 1
    assert not vm.return_runtime.frames and vm.return_runtime.pending is None


def test_unbound_ordinary_lifted_call_keeps_its_relocated_return_word(tmp_path):
    _, compiled, _ = toy(tmp_path)
    vm = emu.Machine(image.primary(), compiled.image)
    result = vm.run(compiled.entry, case(), 1000)
    assert result.outcome == 'return'
    assert result.regs[0] == compiled.symbols[lift.label(RETURN)] != RETURN


@pytest.mark.parametrize('bound', [False, True])
def test_consumed_return_word_survives_partial_scratch_overwrite(tmp_path, bound):
    # Authored seam, not a game-callee model: RET consumes but does not erase
    # its slot, which is subsequently reused as partially initialized data.
    continuation = ('mov byte ptr [esp-4], 0xa\n'
                    'mov edx, dword ptr [esp-4]\n mov eax, edx')
    _, compiled, entries = toy(tmp_path, continuation=continuation)
    if bound:
        vm = machine(compiled, bind(compiled, entries))
        return_word = RETURN
    else:
        vm = emu.Machine(image.primary(), compiled.image)
        return_word = compiled.symbols[lift.label(RETURN)]
    expected = (return_word & 0xffffff00) | 0xa
    result = vm.run(compiled.entry, case(), 1000)
    assert result.outcome == 'return', (result.outcome, result.detail)
    assert result.regs[0] == result.regs[2] == expected
    # Do not normalize a partly overwritten numeric value as a code pointer.
    other_word = compiled.symbols[lift.label(RETURN)] if bound else RETURN
    assert expected != ((other_word & 0xffffff00) | 0xa)
    slot = emu.STACK_TOP - 8
    events = result.events
    consumed = events.index(('r', slot, 4, return_word))
    overwritten = events.index(('w', slot, 1, 0xa), consumed + 1)
    loaded = events.index(('r', slot, 4, expected), overwritten + 1)
    assert consumed < overwritten < loaded
    if bound:
        assert vm.return_runtime.dispatches == 1
        assert not vm.return_runtime.frames and vm.return_runtime.pending is None


def test_actual_generated_lift_round_trip_and_source_stay_unchanged(tmp_path):
    lifted, text, compiled = build.lift_and_build(OWNER,tmp_path)
    assert compiled is not None, lifted.problems
    before = compiled.image.read(compiled.image.text.va,compiled.image.text.virtual_size)
    bindings = bind(compiled,{OWNER:compiled.entry})
    assert bindings[0].callee_entry == CALLEE
    assert build.round_trip(lifted,compiled) == []
    assert lift.render(lifted) == text
    assert compiled.image.read(compiled.image.text.va,len(before)) == before


@pytest.mark.parametrize('change', ['missing-call','missing-return','wrong-return',
    'wrong-owner-entry','wrong-callee-entry','duplicate-site','interior-site','non-call-site'])
def test_lifted_binding_rejects_forged_sites_labels_and_entry_bindings(tmp_path,change):
    _, compiled, entries = toy(tmp_path)
    symbols, sites = dict(compiled.symbols), (CALL,)
    if change == 'missing-call':
        symbols.pop(lift.label(CALL))
    elif change == 'missing-return':
        symbols.pop(lift.label(RETURN))
    elif change == 'wrong-return':
        symbols[lift.label(RETURN)] += 1
    elif change == 'wrong-owner-entry':
        entries[OWNER] += 1
    elif change == 'wrong-callee-entry':
        entries[CALLEE] += 1
    elif change == 'duplicate-site':
        sites = (CALL,CALL)
    elif change == 'interior-site':
        sites = (CALL+1,)
    else:
        sites = (OWNER,)
    with pytest.raises(ValueError):
        bind(compiled,entries,sites,symbols)


@pytest.mark.parametrize('call', ['call _cm_fn_00523300+1','jmp _cm_fn_00523300'])
def test_lifted_binding_rejects_mutated_native_transfer(tmp_path,call):
    _, compiled, entries = toy(tmp_path,call=call)
    with pytest.raises(ValueError):
        bind(compiled,entries)


def test_lifted_binding_rejects_ambiguous_original_owner(tmp_path,monkeypatch):
    _, compiled, entries = toy(tmp_path)
    routine = lift.discover(OWNER)
    monkeypatch.setattr(lift,'discover',lambda va: routine)
    with pytest.raises(ValueError,match='owner'):
        bind(compiled,entries)


@pytest.mark.parametrize('change',['raw','target','continuation'])
def test_lifted_binding_rechecks_pinned_instruction_instead_of_trusting_discovery(tmp_path,monkeypatch,change):
    _, compiled, entries = toy(tmp_path)
    original_discover = lift.discover
    owner = original_discover(OWNER)
    instructions = dict(owner.insns)
    if change == 'raw':
        instructions[CALL] = replace(instructions[CALL],raw=bytes.fromhex('e838020001'))
    elif change == 'target':
        instructions[CALL] = replace(instructions[CALL],target=CALLEE+1)
    else:
        instructions.pop(RETURN)
    monkeypatch.setattr(lift,'discover',lambda va: SimpleNamespace(insns=instructions)
                        if va == OWNER else original_discover(va))
    with pytest.raises(ValueError,match='pinned'):
        bind(compiled,entries)


@pytest.mark.parametrize('field',['original_return','original_callee',
    'compiled_continuation','callee_entry','entry_bindings'])
def test_forged_lifted_runtime_binding_cannot_redirect_a_call(tmp_path,field):
    _, compiled, entries = toy(tmp_path)
    binding = bind(compiled,entries)[0]
    changed = (tuple((va,at+1 if va == CALLEE else at) for va,at in binding.entry_bindings)
               if field == 'entry_bindings' else getattr(binding,field)+1)
    vm = machine(compiled,(replace(binding,**{field:changed}),))
    result = vm.run(compiled.entry,case(),1000)
    assert result.outcome == 'escape'
    assert vm.return_runtime.dispatches == 0
    assert vm.return_runtime.pending is None
    assert not [event for event in result.events if event[:3] == ('w',emu.STACK_TOP-8,4)]


@pytest.mark.parametrize('body', [
    f'add esp,4\n jmp 0x{RETURN:08x}',
    f'add esp,4\n mov dword ptr [esp],0x{RETURN:08x}\n ret',
])
def test_lifted_return_rejects_non_ret_and_borrowed_word_at_wrong_slot(tmp_path,body):
    _, compiled, entries = toy(tmp_path,body)
    vm = machine(compiled,bind(compiled,entries))
    result = vm.run(compiled.entry,case(),1000)
    assert result.outcome == 'escape'
    assert result.detail == (RETURN,)
    assert vm.return_runtime.rejection is not None
    assert vm.return_runtime.dispatches == 0


@pytest.mark.parametrize('cleanup',[0,4,8])
def test_lifted_owned_ret_immediate_cleanup_uses_actual_post_esp(tmp_path,cleanup):
    body = 'ret' if cleanup == 0 else f'ret {cleanup}'
    continuation = (f'sub esp,{cleanup}\n' if cleanup else '')+'mov eax,dword ptr [esp-4]'
    _, compiled, entries = toy(tmp_path,body,continuation)
    vm = machine(compiled,bind(compiled,entries))
    observed = []
    vm.uc.hook_add(unicorn.UC_HOOK_CODE,
        lambda uc,*args: observed.append(uc.reg_read(R.UC_X86_REG_ESP)),begin=RETURN,end=RETURN)
    result = vm.run(compiled.entry,case(),1000)
    assert result.outcome == 'return',result.detail
    assert observed == [emu.STACK_TOP-4+cleanup]
    assert result.regs[0] == RETURN
    assert vm.return_runtime.dispatches == 1


@pytest.mark.parametrize('body',['.byte 0x66,0xc3','.byte 0xcb','.byte 0xcf'])
def test_non_32bit_near_returns_cannot_authorize_lifted_dispatch(tmp_path,body):
    _, compiled, entries = toy(tmp_path,body)
    vm = machine(compiled,bind(compiled,entries))
    result = vm.run(compiled.entry,case(),1000)
    assert result.outcome != 'return'
    assert vm.return_runtime.dispatches == 0


@pytest.mark.parametrize('entry',[CALL,RETURN,CALLEE])
def test_lifted_original_arrivals_without_owned_transfer_escape(tmp_path,entry):
    _, compiled, entries = toy(tmp_path)
    vm = machine(compiled,bind(compiled,entries))
    result = vm.run(entry,case(),1000)
    assert result.outcome == 'escape'
    assert vm.return_runtime.dispatches == 0


@pytest.mark.parametrize('address', ['native','original'])
@pytest.mark.parametrize('when', ['before-call','inside-callee'])
def test_lifted_call_integrity_checked_before_transfer_and_return(tmp_path,address,when):
    _, compiled, entries = toy(tmp_path)
    bindings = bind(compiled,entries)
    vm = machine(compiled,bindings)
    at = bindings[0].compiled_push if address == 'native' else CALL
    def mutate(uc,*args):
        uc.mem_write(at+1,bytes([uc.mem_read(at+1,1)[0]^1]))
    if when == 'before-call':
        mutate(vm.uc)
    else:
        target = entries[CALLEE]
        vm.uc.hook_add(unicorn.UC_HOOK_CODE,mutate,begin=target,end=target)
    result = vm.run(compiled.entry,case(),1000)
    assert result.outcome == 'escape'
    assert vm.return_runtime.dispatches == 0
    assert vm.return_runtime.rejection is not None


def test_lifted_runtime_resets_after_pending_transfer_and_completed_case(tmp_path):
    _, compiled, entries = toy(tmp_path)
    vm = machine(compiled,bind(compiled,entries))
    rt = vm.return_runtime
    vm.uc.reg_write(R.UC_X86_REG_ESP,emu.STACK_TOP)
    vm.uc.reg_write(R.UC_X86_REG_EIP,next(iter(rt.lifted_calls)))
    assert rt.step(compiled.symbols[lift.label(CALL)],5)
    assert rt.pending is not None
    for _ in range(2):
        result = vm.run(compiled.entry,case(),1000)
        assert result.outcome == 'return'
        assert result.regs[0] == RETURN
        assert rt.rejection is None and rt.dispatches == 1
        assert not rt.frames and rt.pending is None


def _transport_program(tmp_path, owner, bodies, sites):
    """Real pinned transfer sites; the surrounding bodies are VM scaffolding."""
    text = '.intel_syntax noprefix\n.text\n'
    for va, body in bodies.items():
        text += (f'.globl {lift.symbol(va)}\n{lift.symbol(va)}:\n'
                 f'{lift.label(va)}:\n{body}\n')
    path = tmp_path/'lifecycle.S'
    path.write_text(text, encoding='utf-8')
    compiled = build.build(path, lift.symbol(owner), tmp_path)
    entries = {va: compiled.symbols[lift.symbol(va)] for va in bodies}
    compiled.image.return_bindings += return_transport.bind_lifted(
        compiled.image, compiled.symbols, entries, sites)
    vm = emu.Machine(image.primary(), compiled.image)
    for va in bodies:
        vm.uc.hook_add(unicorn.UC_HOOK_CODE, vm._escape,
                       begin=va, end=lift.discover(va).region_end-1)
    return compiled, vm


def _pinned_direct(owner, call, target):
    instruction = lift.discover(owner).insns[call]
    raw = image.primary().read(call, 5)
    assert instruction.kind == 'call' and instruction.size == 5
    assert instruction.raw == raw and raw[0] == 0xe8
    assert instruction.target == target
    assert (call+5+int.from_bytes(raw[1:], 'little', signed=True)) & 0xffffffff == target
    assert call+5 in lift.discover(owner).insns


def test_nested_lifted_calls_own_two_real_return_slots_and_dispatch_in_lifo_order(tmp_path):
    _pinned_direct(0x7e5160, 0x7e522f, 0x785790)
    _pinned_direct(0x785790, 0x7857d4, 0x944cff)
    bodies = {
        0x7e5160: 'v_007e522f:\n call _cm_fn_00785790\n'
                  'v_007e5234:\n inc ebx\n ret',
        0x785790: 'v_007857d4:\n call _cm_fn_00944cff\n'
                  'v_007857d9:\n inc edx\n ret',
        0x944cff: 'inc ecx\n ret',
    }
    compiled, vm = _transport_program(tmp_path, 0x7e5160, bodies, (0x7e522f, 0x7857d4))
    returned = []
    for address in (0x7e5234, 0x7857d9):
        vm.uc.hook_add(unicorn.UC_HOOK_CODE,
            lambda uc, at, *args: returned.append((at, vm.return_runtime.dispatches)),
            begin=address, end=address)
    result = vm.run(compiled.entry, case(), 1000)
    assert result.outcome == 'return', result.detail
    assert (result.regs[3], result.regs[2], result.regs[1]) == (1, 1, 0x03010e59)
    assert [event for event in result.events if event[0] == 'w'] == [
        ('w', emu.STACK_TOP-4, 4, 0x7e5234),
        ('w', emu.STACK_TOP-8, 4, 0x7857d9),
    ]
    assert returned == [(0x7857d9, 1), (0x7e5234, 2)]
    assert {b.kind for b in compiled.image.return_bindings} == {'lifted-direct'}
    assert vm.return_runtime.pending is None and not vm.return_runtime.frames


def test_recursive_lifted_call_keeps_three_distinct_active_slots(tmp_path):
    owner, call, continuation = 0x90cfc0, 0x90d06e, 0x90d073
    _pinned_direct(owner, call, owner)
    bodies = {owner: ('test ecx, ecx\n jz done\n dec ecx\n'
                      'v_0090d06e:\n call _cm_fn_0090cfc0\n'
                      'v_0090d073:\n inc ebx\n done:\n ret')}
    compiled, vm = _transport_program(tmp_path, owner, bodies, (call,))
    depths = []
    vm.uc.hook_add(unicorn.UC_HOOK_CODE,
        lambda uc, *args: depths.append(len(vm.return_runtime.frames)),
        begin=call, end=call)
    incoming = emu.Case({'ecx': 3, 'eflags': 0x202}, [0]*10, bytes(emu.ARENA_SIZE))
    result = vm.run(compiled.entry, incoming, 1000)
    assert result.outcome == 'return', result.detail
    assert result.regs[1] == 0 and result.regs[3] == 3
    assert depths == [0, 1, 2]
    assert [event for event in result.events if event[0] == 'w'] == [
        ('w', emu.STACK_TOP-offset, 4, continuation) for offset in (4, 8, 12)]
    assert vm.return_runtime.dispatches == 3
    assert vm.return_runtime.pending is None and not vm.return_runtime.frames


@pytest.mark.parametrize('crossed_owner', [False, True])
def test_adjacent_lifted_calls_require_the_same_owner_at_shared_original_address(tmp_path, crossed_owner):
    owner, first, shared, last = 0x452710, 0x45285c, 0x452861, 0x452866
    _pinned_direct(owner, first, 0x5cc7d0)
    _pinned_direct(owner, shared, 0x5a8f60)
    bodies = {
        owner: ('v_0045285c:\n call _cm_fn_005cc7d0\n'
                'v_00452861:\n call _cm_fn_005a8f60\n'
                'v_00452866:\n ret'),
        0x5cc7d0: 'inc ebx\n ret',
        0x5a8f60: 'add ebx, 2\n ret',
    }
    compiled, vm = _transport_program(tmp_path, owner, bodies, (first, shared))
    if crossed_owner:
        address, binding = next((at, binding) for at, binding in vm.return_runtime.lifted_calls.items()
                                if binding.original_call == shared)
        # A negative control for the return/next-CALL ambiguity, not a valid declaration.
        vm.return_runtime.lifted_calls[address] = replace(binding, owner=0x5cc7d0)
    result = vm.run(compiled.entry, case(), 1000)
    if crossed_owner:
        assert result.outcome == 'escape' and result.detail == (shared,)
        assert vm.return_runtime.dispatches == 1
        assert vm.return_runtime.rejection == 'original continuation has no matching active call'
        assert result.regs[3] == 1
    else:
        assert result.outcome == 'return', result.detail
        assert result.regs[3] == 3
        assert vm.return_runtime.dispatches == 2
        assert vm.return_runtime.pending is None and not vm.return_runtime.frames
        assert [event for event in result.events if event[0] == 'w'] == [
            ('w', emu.STACK_TOP-4, 4, shared),
            ('w', emu.STACK_TOP-4, 4, last),
        ]


def test_lifted_indirect_and_c_direct_calls_share_one_owned_lifo(tmp_path):
    _pinned_direct(0x7e6130, 0x7e6133, 0x7e6150)
    _pinned_direct(0x785790, 0x7857d4, 0x944cff)
    assert image.primary().read(0x7e6189, 2) == b'\xff\x10'
    bodies = {
        0x7e6130: ('v_007e6133:\n call _cm_fn_007e6150\n'
                   'v_007e6138:\n inc ebx\n ret'),
        0x7e6150: ('cmre_indirect_007e6189_007e618b:\n'
                   '.byte 0xe9\n .long 0x007e6189 - . - 4\n'
                   'cmre_return_007e6189_007e618b:\n inc edx\n ret'),
        0x785790: ('cmre_call_007857d4_007857d9_00944cff:\n'
                   'push 0x007857d9\n .byte 0xe9\n .long _cm_fn_00944cff - . - 4\n'
                   'cmre_return_007857d4_007857d9:\n inc ecx\n ret'),
        0x944cff: 'ret',
    }
    compiled, vm = _transport_program(tmp_path, 0x7e6130, bodies, (0x7e6133,))
    arena = (0x785790).to_bytes(4, 'little') + bytes(emu.ARENA_SIZE-4)
    incoming = emu.Case({'eax': emu.ARENA, 'eflags': 0x202}, [0]*10, arena)
    result = vm.run(compiled.entry, incoming, 1000)
    assert result.outcome == 'return', result.detail
    assert (result.regs[3], result.regs[2], result.regs[1]) == (1, 1, 1)
    assert {b.kind for b in compiled.image.return_bindings} == {'lifted-direct', 'indirect', 'direct'}
    assert [event for event in result.events if event[0] == 'w'] == [
        ('w', emu.STACK_TOP-4, 4, 0x7e6138),
        ('w', emu.STACK_TOP-8, 4, 0x7e618b),
        ('w', emu.STACK_TOP-12, 4, 0x7857d9),
    ]
    operand = result.events.index(('r', emu.ARENA, 4, 0x785790))
    pushed = result.events.index(('w', emu.STACK_TOP-8, 4, 0x7e618b))
    assert operand < pushed
    assert vm.return_runtime.dispatches == 3
    assert vm.return_runtime.pending is None and not vm.return_runtime.frames


@pytest.mark.parametrize('fault', ['return-slot-write', 'callee-fetch'])
def test_lifted_actual_call_fault_matches_original_reference_state(tmp_path, fault):
    owner, call, continuation, callee = 0x7e5160, 0x7e522f, 0x7e5234, 0x785790
    _pinned_direct(owner, call, callee)
    assert call//emu.PAGE != callee//emu.PAGE
    bodies = {owner: ('nop\n v_007e522f:\n call _cm_fn_00785790\n'
                      'v_007e5234:\n ret')}
    compiled, recovered = _transport_program(tmp_path, owner, bodies, (call,))
    original = emu.Machine(image.primary())
    if fault == 'return-slot-write':
        # Configure both executions before the actual CALL. The recovered NOP
        # gives the setup hook a point before transport captures the owned slot.
        for vm, at in ((original, call), (recovered, compiled.entry)):
            vm.uc.hook_add(unicorn.UC_HOOK_CODE,
                lambda uc, *args: uc.reg_write(R.UC_X86_REG_ESP, emu.STACK), begin=at, end=at)
    else:
        for vm in (original, recovered):
            vm.uc.mem_protect(callee & ~(emu.PAGE-1), emu.PAGE, unicorn.UC_PROT_READ)
    initial = case()
    reference = original.run(call, initial, 1000)
    candidate = recovered.run(compiled.entry, initial, 1000)
    assert candidate.outcome == reference.outcome == 'fault'
    assert candidate.detail == reference.detail
    assert candidate.regs == reference.regs
    assert candidate.events == reference.events
    assert candidate.memory == reference.memory
    if fault == 'return-slot-write':
        assert candidate.detail == ((unicorn.UC_MEM_WRITE_UNMAPPED, emu.STACK-4, 4), call)
    else:
        assert candidate.detail[0][0] == unicorn.UC_MEM_FETCH_PROT
        assert candidate.detail[0][1] == candidate.detail[1] == callee
        assert ('w', emu.STACK_TOP-4, 4, continuation) in candidate.events
        assert bytes(recovered.uc.mem_read(emu.STACK_TOP-4, 4)) == continuation.to_bytes(4, 'little')
    assert recovered.return_runtime.rejection is None
    assert recovered.return_runtime.dispatches == 0
    assert not recovered.return_runtime.frames
