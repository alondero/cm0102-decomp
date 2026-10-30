"""Budget cutoffs count executed instructions, not owned VM redirects."""
from pathlib import Path

import pytest
import unicorn
from unicorn import x86_const as R

from cmre import build, emu, env, image, return_transport

CALL, CONTINUATION, CALLEE = 0x007E522F, 0x007E5234, 0x00785790


def _symbol(compiled, name):
    return next(address for symbol, address in compiled.symbols.items()
                if symbol.lstrip('_') == name)


@pytest.fixture(scope='module')
def toy_bodies(tmp_path_factory):
    built = {}
    bodies = {
        'finite': 'incl %ebx; ret',
        'infinite': '1: incl %ebx; jmp 1b',
        'fault': 'movl (0), %eax; ret',
        'mixed': ('cmre_call_007857d4_007857d9_00944cff:; pushl $0x007857d9; '
                  '.byte 0xe9; .long _cm_fn_00944cff - . - 4; '
                  'cmre_return_007857d4_007857d9:; incl %ebx; ret'),
    }
    bodies['legacy-direct'] = bodies['mixed']
    for name, callee in bodies.items():
        directory = tmp_path_factory.mktemp(name)
        path = directory / 'toy.c'
        path.write_text('''unsigned __attribute__((naked)) cm_fn_007e5160(void) {
            __asm__("v_007e522f:\\n call _cm_fn_00785790\\n"
                    "v_007e5234:\\n incl %eax\\n ret\\n");
        }
        void __attribute__((naked)) cm_fn_00785790(void) {
            __asm__("''' + callee + '''\\n");
        }
        void __attribute__((naked)) cm_fn_00944cff(void) { __asm__("ret\\n"); }
        ''', encoding='utf-8')
        compiled = build.build(path, 'cm_fn_007e5160', directory, '-O0')
        site = _symbol(compiled, 'v_007e522f')
        target = _symbol(compiled, 'cm_fn_00785790')
        if name != 'legacy-direct':
            compiled.image.return_bindings += (return_transport.Binding(
                0x007E5160, CALL, CONTINUATION, CALLEE, site, site + 5, target,
                image.primary().read(CALL, 5).hex(), compiled.image.read(site, 5).hex(),
                kind='lifted-direct', entry_bindings=((CALLEE, target),)),)
        built[name] = compiled
    return built


def _case():
    return emu.Case({'eflags': 0x202}, [0] * 10, bytes(emu.ARENA_SIZE))


def _machine(compiled):
    return emu.Machine(image.primary(), compiled.image,
                       watch=(CALL, CONTINUATION + 1), escape=True)


def test_imports_use_the_current_workspace():
    assert env.ROOT == Path(__file__).resolve().parents[1]


def test_unicorn_native_count_charges_redirected_hook_arrivals():
    uc = unicorn.Uc(unicorn.UC_ARCH_X86, unicorn.UC_MODE_32)
    base = 0x10000000
    uc.mem_map(base, 0x1000)
    uc.mem_write(base, b'\x40\x40\x40\x40')
    seen = []

    def redirect(machine, address, size, data):
        seen.append(address)
        if address == base:
            machine.reg_write(R.UC_X86_REG_EIP, base + 2)

    uc.hook_add(unicorn.UC_HOOK_CODE, redirect)
    uc.emu_start(base, base + 4, count=2)
    assert seen == [base, base + 2]
    assert uc.reg_read(R.UC_X86_REG_EAX) == 1
    assert uc.reg_read(R.UC_X86_REG_EIP) == base + 3


@pytest.mark.parametrize('budget', range(1, 7))
def test_every_call_and_ret_cutoff_charges_only_executed_instructions(toy_bodies, budget):
    compiled = toy_bodies['finite']
    machine = _machine(compiled)
    result = machine.run(compiled.entry, _case(), budget)
    assert result.outcome == ('return' if budget >= 5 else 'budget'), result
    assert result.regs[0] == (1 if budget >= 4 else 0)
    assert result.regs[3] == (1 if budget >= 2 else 0)
    assert result.regs[4] == emu.STACK_TOP + (4 if budget >= 5 else -4 if budget < 3 else 0)
    assert [event for event in result.events if event[0] == 'w'] == [
        ('w', emu.STACK_TOP - 4, 4, CONTINUATION)]
    if budget in (1, 3):
        assert result.detail == (CALLEE if budget == 1 else CONTINUATION,)
    elif budget in (2, 4):
        following = (_symbol(compiled, 'cm_fn_00785790') + 1 if budget == 2
                     else _symbol(compiled, 'v_007e5234') + 1)
        assert result.detail == (following,)
    assert machine._budget_steps == min(budget, 5)


@pytest.mark.parametrize('budget', [1, 2, 3, 8, 31])
def test_finite_caller_with_infinite_callee_cannot_evade_budget(toy_bodies, budget):
    compiled = toy_bodies['infinite']
    machine = _machine(compiled)
    result = machine.run(compiled.entry, _case(), budget)
    assert result.outcome == 'budget', result
    assert result.regs[3] == budget // 2
    assert machine._budget_steps == budget


@pytest.mark.parametrize('budget', [1, 2, 3])
def test_faulting_instruction_runs_only_when_its_budget_slot_is_available(toy_bodies, budget):
    compiled = toy_bodies['fault']
    machine = _machine(compiled)
    result = machine.run(compiled.entry, _case(), budget)
    assert result.outcome == ('budget' if budget == 1 else 'fault'), result
    assert result.regs[0] == 0
    if budget > 1:
        assert result.detail == ((unicorn.UC_MEM_READ_UNMAPPED, 0, 4),
                                 _symbol(compiled, 'cm_fn_00785790'))


def test_zero_instruction_redirect_cycle_hits_separate_native_guard(toy_bodies, monkeypatch):
    compiled = toy_bodies['finite']
    machine = _machine(compiled)
    sites = (compiled.entry, compiled.entry + 5)

    def redirect(address, size):
        target = sites[1] if address == sites[0] else sites[0]
        machine.uc.reg_write(R.UC_X86_REG_EIP, target)
        machine.return_runtime.redirect = (address, target)
        return True

    monkeypatch.setattr(machine.return_runtime, 'step', redirect)
    result = machine.run(compiled.entry, _case(), 1)
    assert result.outcome == 'error'
    assert result.detail[0] == 'native-step guard exhausted'
    assert machine._budget_steps == 0
    assert result.events == []


def test_logical_budget_and_pending_transfers_reset_between_cases(toy_bodies):
    compiled = toy_bodies['finite']
    machine = _machine(compiled)
    for budget in (1, 5, 3, 5):
        result = machine.run(compiled.entry, _case(), budget)
        assert result.outcome == ('return' if budget == 5 else 'budget'), result
        assert machine._budget_steps == budget
        assert len([event for event in result.events if event[0] == 'w']) == 1


@pytest.mark.parametrize('budget', range(1, 10))
def test_mixed_lifted_and_c_direct_bindings_charge_native_push_and_jump(toy_bodies, budget):
    compiled = toy_bodies['mixed']
    machine = _machine(compiled)
    result = machine.run(compiled.entry, _case(), budget)
    assert result.outcome == ('return' if budget >= 8 else 'budget'), result
    assert machine._budget_steps == min(budget, 8)
    assert result.regs[0] == (1 if budget >= 7 else 0)
    assert result.regs[3] == (1 if budget >= 5 else 0)
    writes = [('w', emu.STACK_TOP - 4, 4, CONTINUATION)]
    if budget >= 2:
        writes.append(('w', emu.STACK_TOP - 8, 4, 0x007857D9))
    assert [event for event in result.events if event[0] == 'w'] == writes
    if budget == 4:
        assert result.detail == (0x007857D9,)


def test_unowned_arrival_remains_escape_and_is_not_a_guard_error(toy_bodies):
    compiled = toy_bodies['finite']
    machine = _machine(compiled)
    result = machine.run(CONTINUATION, _case(), 1)
    assert result.outcome == 'escape'
    assert result.detail == (CONTINUATION,)
    assert result.events == []


@pytest.mark.parametrize('budget', range(1, 8))
def test_c_only_direct_transport_keeps_existing_native_arrival_budget(toy_bodies, budget):
    compiled = toy_bodies['legacy-direct']
    machine = emu.Machine(image.primary(), compiled.image)
    result = machine.run(_symbol(compiled, 'cm_fn_00785790'), _case(), budget)
    assert not machine.logical_budget
    assert result.outcome == ('return' if budget >= 6 else 'budget'), result
    assert result.regs[3] == (1 if budget >= 5 else 0)
    assert machine._budget_steps == 0


@pytest.fixture(scope='module')
def legacy_indirect(tmp_path_factory):
    directory = tmp_path_factory.mktemp('legacy-indirect')
    path = directory / 'toy.c'
    path.write_text('''#include "cmre.h"
    void __attribute__((naked)) cm_fn_007e6150(void) {
        __asm__(CM_ORIGINAL_INDIRECT_CALL(007e6189,007e618b)
                "incl %ebx\\n ret\\n");
    }
    void __attribute__((naked)) cm_fn_00944cff(void) { __asm__("ret\\n"); }
    ''', encoding='utf-8')
    return build.build(path, 'cm_fn_007e6150', directory, '-O0')


@pytest.mark.parametrize('budget', range(1, 9))
def test_c_only_indirect_transport_keeps_existing_native_arrival_budget(legacy_indirect, budget):
    import struct

    machine = emu.Machine(image.primary(), legacy_indirect.image)
    arena = struct.pack('<I', 0x00944CFF) + bytes(emu.ARENA_SIZE - 4)
    case = emu.Case({'eax': emu.ARENA, 'eflags': 0x202}, [0] * 10, arena)
    result = machine.run(legacy_indirect.entry, case, budget)
    assert not machine.logical_budget
    assert result.outcome == ('return' if budget >= 7 else 'budget'), result
    assert result.regs[3] == (1 if budget >= 6 else 0)
    writes = [] if budget == 1 else [('w', emu.STACK_TOP - 4, 4, 0x007E618B)]
    assert [event for event in result.events if event[0] == 'w'] == writes
    assert machine._budget_steps == 0
