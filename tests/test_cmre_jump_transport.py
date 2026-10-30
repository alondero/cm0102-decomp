"""Synthetic bodies exercise owned indexed-JMP execution, not game semantics.

All opcode/table declarations are checked against the pinned original copy
routine. Native target bodies deliberately contain RET only; they are scaffolding.
"""
import pytest
import unicorn
from unicorn import x86_const as R

from cmre import build, emu, env, image, imports, lift, recover
from test_cmre_bounded_tables import table_hints

OWNER, SITE = 0x0094f0f0, 0x0094f13d
SLOT = 0x0094f154
TARGETS = (0x0094f160, 0x0094f18c, 0x0094f1b0)

pytestmark = pytest.mark.usefixtures('table_hints')
SITES = (0x94f125, 0x94f13d, 0x94f144, 0x94f14c, 0x94f182, 0x94f1a8,
         0x94f1c2, 0x94f22f, 0x94f2a7, 0x94f2b2, 0x94f2cd, 0x94f2d4,
         0x94f2fe, 0x94f328, 0x94f35a, 0x94f3c7)


def source():
    return '''#include "cmre.h"
void __attribute__((naked)) cm_fn_0094f0f0(void) {
    __asm__("v_0094f13d:\\n cmre_jump_0094f13d:\\n"
            ".byte 0xff,0x24,0x85,0x50,0xf1,0x94,0x00\\n"
            "v_0094f160:\\n ret\\n"
            "v_0094f18c:\\n ret\\n"
            "v_0094f1b0:\\n ret\\n");
}
'''


def compile_source(tmp_path, text=None, opt='-O0'):
    path = tmp_path / 'jump.c'
    path.write_text(source() if text is None else text, encoding='utf-8')
    return build.build(path, 'cm_fn_0094f0f0', tmp_path, opt)


def case(index=1):
    regs = dict.fromkeys(emu.REG_NAMES, 0)
    regs.update(eax=index, eflags=0x202)
    return emu.Case(regs, [0] * 10, bytes(emu.ARENA_SIZE))


def machine(compiled):
    return emu.Machine(image.primary(), compiled.image,
                       watch=(OWNER, lift.discover(OWNER).region_end), escape=True)


@pytest.mark.parametrize('opt', ['-O0', '-O2'])
def test_actual_table_read_redirects_to_owned_native_target(tmp_path, opt):
    compiled = compile_source(tmp_path, opt=opt)
    assert len(compiled.image.jump_bindings) == 1
    vm = machine(compiled)
    for index, target in enumerate(TARGETS, 1):
        result = vm.run(compiled.entry, case(index), 100)
        assert result.outcome == 'return', result
        assert ('r', SLOT + 4 * (index - 1), 4, target) in result.events
        assert vm.jump_runtime.dispatches == 1
        assert vm.jump_runtime.pending is None
        assert bytes(vm.uc.mem_read(SITE, 7)) == image.primary().read(SITE, 7)


@pytest.mark.parametrize('index', [0, 4])
def test_outside_declared_physical_slots_rejects_before_read(tmp_path, index):
    vm = machine(compile_source(tmp_path))
    result = vm.run(vm.recovered.text.va, case(index), 100)
    assert result.outcome == 'escape'
    assert result.events == []


def test_wrapped_index_alias_is_same_physical_slot(tmp_path):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    assert vm.run(compiled.entry, case(1 + (1 << 30)), 100).outcome == 'return'
    assert vm.jump_runtime.dispatches == 1


@pytest.mark.parametrize('entry', [SITE, *TARGETS])
def test_unowned_original_site_and_target_are_not_whitelisted(tmp_path, entry):
    vm = machine(compile_source(tmp_path))
    assert vm.run(entry, case(), 100).outcome == 'escape'


def test_matching_target_without_cpu_operand_read_is_rejected(tmp_path):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    vm.uc.hook_add(unicorn.UC_HOOK_CODE,
                   lambda uc, address, size, data: uc.reg_write(R.UC_X86_REG_EIP, TARGETS[0]),
                   begin=compiled.entry, end=compiled.entry)
    result = vm.run(compiled.entry, case(), 100)
    assert result.outcome == 'escape'
    assert not result.events
    assert vm.jump_runtime.dispatches == 0


def test_pending_read_fault_resets_before_next_run(tmp_path):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    page = SLOT & ~0xfff
    vm.uc.mem_protect(page, 0x1000, unicorn.UC_PROT_EXEC)
    first = vm.run(compiled.entry, case(), 100)
    assert first.outcome == 'fault'
    assert vm.jump_runtime.pending is not None
    vm.uc.mem_protect(page, 0x1000, unicorn.UC_PROT_READ | unicorn.UC_PROT_EXEC)
    second = vm.run(TARGETS[0], case(), 100)
    assert second.outcome == 'escape'
    assert vm.jump_runtime.pending is None
    assert vm.jump_runtime.dispatches == 0


@pytest.mark.parametrize('target', [emu.STOP, emu.ARENA, emu.TRAP])
def test_stop_bypass_target_fetch_fault_and_import_arrival(tmp_path, target):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    vm.uc.mem_write(SLOT, target.to_bytes(4, 'little'))
    result = vm.run(compiled.entry, case(), 100)
    assert result.outcome == ('fault' if target == emu.ARENA else 'escape')
    assert vm.jump_runtime.dispatches == 0
    if target == emu.STOP:
        assert vm.jump_runtime.pending is not None
        assert result.regs[4] == emu.STACK_TOP
    assert not [e for e in result.events if e[0] in ('w', 'c')]


def test_changed_native_bytes_reject_before_operand_read(tmp_path):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    vm.uc.mem_write(compiled.entry + 3, b'\x00')
    result = vm.run(compiled.entry, case(), 100)
    assert result.outcome == 'escape'
    assert not result.events


@pytest.mark.parametrize('old,new', [
    ('cmre_jump_0094f13d', 'cmre_jump_0094F13D'),
    ('cmre_jump_0094f13d', 'cmre_jump_0094f13e'),
    ('v_0094f160:', 'missing_target:'),
    ('0x50,0xf1,0x94,0x00', '0x54,0xf1,0x94,0x00'),
    ('v_0094f160:\\n ret', 'v_0094f160 = v_0094f13d + 1\\n ret'),
])
def test_malformed_unowned_missing_or_mid_instruction_bindings_reject(tmp_path, old, new):
    with pytest.raises(build.BuildError, match='jump transport'):
        compile_source(tmp_path, source().replace(old, new))


def test_no_binding_preserves_ordinary_original_escape(tmp_path):
    compiled = compile_source(tmp_path, source().replace('cmre_jump_0094f13d:', 'ordinary_jump:'))
    vm = machine(compiled)
    assert not compiled.image.jump_bindings
    assert vm.jump_runtime is None
    assert vm.run(compiled.entry, case(), 100).outcome == 'escape'


def site_source(site):
    body = lift.discover(OWNER)
    rec = body.insns[site]
    ins = next(lift._cs().disasm(rec.raw, site, count=1))
    first, low = rec.table_anchor
    # The fixture owns the complete declarations; the instruction identifies
    # exactly its real target slots, including signed/nonzero domains.
    import json
    hints = json.loads((env.HINTS_DIR / 'fn_0094f0f0.json').read_text())
    high = hints['lift_jump_tables'][f'0x{site:08x}']['index_range'][1]
    targets = sorted({image.primary().dword(first + 4*n) for n in range(high-low+1)})
    text = '#include "cmre.h"\nvoid __attribute__((naked)) cm_fn_0094f0f0(void) { __asm__("'
    text += f'v_{site:08x}:\\n cmre_jump_{site:08x}:\\n .byte ' + ','.join(hex(b) for b in rec.raw) + '\\n'
    text += ''.join(f'v_{target:08x}:\\n ret\\n' for target in targets) + '"); }\n'
    return text, ins.reg_name(ins.operands[0].mem.index), low, first


@pytest.mark.parametrize('site', SITES)
def test_every_declared_site_executes_its_actual_slots_and_wrap_alias(tmp_path, site):
    text, register, low, first = site_source(site)
    compiled = compile_source(tmp_path, text)
    vm = machine(compiled)
    binding = compiled.image.jump_bindings[0]
    assert binding.original_jump == site
    for index in (low, low + (1 << 30)):
        fixture = case()
        fixture.regs[register] = index
        result = vm.run(compiled.entry, fixture, 100)
        assert result.outcome == 'return'
        assert ('r', first, 4, image.primary().dword(first)) in result.events
        assert vm.jump_runtime.dispatches == 1


@pytest.mark.parametrize('site', SITES)
@pytest.mark.parametrize('fault', ['read', 'fetch'])
def test_all_sites_preserve_raw_operand_and_target_fetch_faults(tmp_path, site, fault):
    text, register, low, first = site_source(site)
    compiled = compile_source(tmp_path, text)
    fixture = case()
    fixture.regs[register] = low
    original = emu.Machine(image.primary())
    candidate = machine(compiled)
    for vm in (original, candidate):
        if fault == 'read':
            vm.uc.mem_protect(first & ~0xfff, 0x1000, unicorn.UC_PROT_EXEC)
        else:
            vm.uc.mem_write(first, emu.ARENA.to_bytes(4, 'little'))
    a = original.run(site, fixture, 100)
    b = candidate.run(compiled.entry, fixture, 100)
    assert a.outcome == b.outcome == 'fault'
    assert a.detail[0] == b.detail[0]
    assert a.detail[1] == (site if fault == 'read' else emu.ARENA)
    assert a.regs == b.regs and a.events == b.events and a.memory == b.memory
    assert bytes(original.uc.mem_read(emu.STACK, emu.STACK_SIZE)) == bytes(candidate.uc.mem_read(emu.STACK, emu.STACK_SIZE))
    assert candidate.jump_runtime.pending is not None


def test_operand_read_ownership_requires_frozen_source_pc(tmp_path):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    # Runtime-level near miss: an actual read in another compiled instruction
    # cannot qualify this pending transfer, even at the correct slot/width.
    vm.uc.reg_write(R.UC_X86_REG_EAX, 1)
    assert vm.jump_runtime.step(compiled.entry, 7)
    vm.uc.reg_write(R.UC_X86_REG_EIP, compiled.entry + 7)
    vm.jump_runtime.observe_read(SLOT, 4, TARGETS[0])
    assert vm.jump_runtime.pending.target is None
    assert not vm.jump_runtime.step(TARGETS[0], 1)


def test_c_cache_and_report_pin_parent_jump_contract(tmp_path, monkeypatch, table_hints):
    path, hints = table_hints
    monkeypatch.setattr(env, 'RUNS', tmp_path / 'runs')
    source_path = tmp_path / 'candidate.c'
    source_path.write_text(source())
    first = recover.test_c(OWNER, {'cases': 2}, source=source_path)
    import json
    hints['lift_jump_tables']['0x0094f13d']['evidence'] += ' Reviewed source-PC observation.'
    path.write_text(json.dumps(hints))
    second = recover.test_c(OWNER, {'cases': 2}, source=source_path)
    assert first['key'] != second['key']
    assert first['jump_contract']['sha256'] != second['jump_contract']['sha256']
    assert second['jump_contract']['sha256'] == env.digest(path)
    for mode in second['builds'].values():
        assert mode['jump_transport']['bindings'][0]['original_jump'] == SITE


@pytest.mark.parametrize('change', ['target', 'native'])
def test_changed_target_or_native_transfer_after_read_rejects(tmp_path, change):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    def change_after_observation(uc, access, address, size, value, data):
        if address == SLOT and size == 4:
            if change == 'target':
                uc.mem_write(SLOT, TARGETS[1].to_bytes(4, 'little'))
            else:
                uc.mem_write(compiled.entry + 3, b'\x00')
    vm.uc.hook_add(unicorn.UC_HOOK_MEM_READ, change_after_observation)
    assert vm.run(compiled.entry, case(), 100).outcome == 'escape'
    assert vm.jump_runtime.dispatches == 0


def caller_source():
    return '''
void __attribute__((naked)) cm_fn_00948bbb(void) {
    __asm__("pushl $0\\n pushl $0\\n pushl $0\\n"
            CM_ORIGINAL_CALL(00948eaa,00948eaf,0094f0f0)
            "addl $12,%esp\\n ret\\n");
}
'''


@pytest.mark.parametrize('opt', ['-O0', '-O2'])
def test_indexed_dispatch_inside_owned_call_returns_to_active_original_word(tmp_path, opt):
    compiled = compile_source(tmp_path, source() + caller_source(), opt)
    vm = machine(compiled)
    vm.uc.hook_add(unicorn.UC_HOOK_CODE, vm._escape,
                   begin=0x00948bbb, end=lift.discover(0x00948bbb).region_end-1)
    for _ in range(2):
        result = vm.run(compiled.symbols['_cm_fn_00948bbb'], case(), 100)
        assert result.outcome == 'return', result
        assert vm.return_runtime.dispatches == vm.jump_runtime.dispatches == 1
        assert not vm.return_runtime.frames
        assert vm.jump_runtime.pending is None
        assert bytes(vm.uc.mem_read(emu.STACK_TOP-16, 4)) == (0x00948eaf).to_bytes(4, 'little')


def test_rejected_call_cannot_run_import_scaffolding_with_jump_binding(tmp_path):
    compiled = compile_source(tmp_path, source() + caller_source())
    vm = machine(compiled)
    trap = next(at for at, name in vm.trap_names.items() if name not in imports.TERMINAL)
    transfer = compiled.image.return_bindings[0].compiled_push + 5
    vm.uc.hook_add(unicorn.UC_HOOK_CODE,
                   lambda uc, at, size, data: uc.reg_write(R.UC_X86_REG_EIP, trap),
                   begin=transfer, end=transfer)
    result = vm.run(compiled.symbols['_cm_fn_00948bbb'], case(), 100)
    assert result.outcome == 'escape'
    assert vm.return_runtime.rejection
    assert not [e for e in result.events if e[0] == 'c']
    assert result.regs[0] == 1
    # This Unicorn build stops the remaining hook chain on emu_stop. The
    # callback contract must also refuse a later trap observer explicitly.
    before = list(vm.events), tuple(vm.uc.reg_read(reg) for reg in emu.REGS)
    vm._trap(vm.uc, trap, 1, None)
    assert (vm.events, tuple(vm.uc.reg_read(reg) for reg in emu.REGS)) == before


def test_marker_spelling_does_not_control_parent_contract_pin(tmp_path, monkeypatch, table_hints):
    text = source().replace('cmre_jump_', 'cmre_" "jump_')
    assert 'cmre_jump_' not in text
    path = tmp_path / 'macro.c'
    path.write_text(text)
    monkeypatch.setattr(env, 'RUNS', tmp_path / 'runs')
    report = recover.test_c(OWNER, {'cases': 2}, source=path)
    assert report['jump_contract']['sha256'] == env.digest(table_hints[0])
    assert all(b['jump_transport'] for b in report['builds'].values())


def test_single_entry_validation_rejects_an_extra_jump_owner(tmp_path, monkeypatch):
    path = tmp_path / 'extra.c'
    path.write_text(source() + caller_source())
    monkeypatch.setattr(env, 'RUNS', tmp_path / 'runs')
    report = recover.test_c(0x00948bbb, {'cases': 2}, source=path)
    assert report['status'] == 'fail'
    assert all(b['status'] == 'build-failed' and 'extra indexed-JMP owners' in b['detail']
               for b in report['builds'].values())


@pytest.mark.parametrize('overlap', ['arrival', 'native'])
def test_call_and_jump_transport_ownership_overlap_is_explicitly_excluded(tmp_path, overlap):
    from dataclasses import replace
    from cmre import jump_transport
    compiled = compile_source(tmp_path, source() + caller_source())
    binding = compiled.image.return_bindings[0]
    if overlap == 'arrival':
        binding = replace(binding, original_return=TARGETS[0])
    else:
        binding = replace(binding, compiled_push=compiled.entry, compiled_continuation=compiled.entry+10)
    # Deliberately forged CALL metadata tests the JMP binder's exclusion. It is
    # not a claim that the CALL binder would accept this synthetic declaration.
    compiled.image.return_bindings = (binding,)
    entries = {OWNER: compiled.entry, 0x00948bbb: compiled.symbols['_cm_fn_00948bbb']}
    with pytest.raises(ValueError, match='overlap'):
        jump_transport.bind(compiled.image, compiled.symbols, entries)
