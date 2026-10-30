"""Isolated original instructions test VM execution ownership, not game logic."""
import json
from dataclasses import replace
from types import SimpleNamespace

import pytest
import unicorn
from unicorn import x86_const as R

from cmre import build, composition, difftest, emu, env, image, lift, recover, x87_transport
from test_cmre_bounded_tables import table_hints

OWNER = 0x00784100
SITE = 0x007847e8
NEXT = 0x007847ec


@pytest.fixture
def x87_hints(tmp_path, monkeypatch):
    directory = tmp_path / 'hints'
    directory.mkdir(exist_ok=True)
    declarations = {f'0x{site:08x}': {'bytes': lift.discover(OWNER).insns[site].raw.hex(),
                                     'evidence': 'Pinned original root instruction; isolated test scaffolding.'}
                    for site in (SITE, 0x7847f4, 0x784813, 0x784817, 0x78481d, 0x784821)}
    (directory / 'fn_00784100.json').write_text(json.dumps({'original_x87': declarations}))
    monkeypatch.setattr(env, 'HINTS_DIR', directory)
    return directory


def source(site=SITE):
    original = lift.discover(OWNER).insns[site]
    raw = ','.join(hex(b) for b in original.raw)
    return f'''#include "cmre.h"
void __attribute__((naked)) cm_fn_00784100(void) {{
 __asm__("v_{site:08x}:\\ncmre_x87_{site:08x}:\\n.byte {raw}\\n"
         "cmre_x87_return_{site:08x}_{site+original.size:08x}:\\nret\\n");
}}
'''


def compile_source(tmp_path, text=None, opt='-O0'):
    path = tmp_path / 'fp.c'
    path.write_text(source() if text is None else text)
    return build.build(path, 'cm_fn_00784100', tmp_path, opt)


def case():
    regs = dict.fromkeys(emu.REG_NAMES, 0)
    regs['eflags'] = 0x202
    return emu.Case(regs, [0] * 6 + [12345] + [0] * 3, bytes(emu.ARENA_SIZE))


def fpstate(vm):
    names = ['FP'+str(i) for i in range(8)] + ['FPCW', 'FPSW', 'FPTAG', 'FIP', 'FDP', 'MXCSR']
    return {name: vm.uc.reg_read(getattr(R, 'UC_X86_REG_'+name)) for name in names}


def machine(compiled):
    return emu.Machine(image.primary(), compiled.image, watch=(OWNER, 0x784eb0), escape=True)


@pytest.mark.usefixtures('x87_hints')
@pytest.mark.parametrize('opt', ['-O0', '-O2'])
def test_original_site_updates_saved_fip_without_host_normalization(tmp_path, opt):
    compiled = compile_source(tmp_path, opt=opt)
    original, candidate = emu.Machine(image.primary()), machine(compiled)
    # The original side intentionally executes one instruction only. The
    # synthetic compiled body then returns; compare FP effects and actual read.
    a = original.run(SITE, case(), 1)
    b = candidate.run(compiled.entry, case(), 100)
    assert a.outcome == 'budget' and b.outcome == 'return'
    assert fpstate(original)['FIP'] == SITE
    assert fpstate(candidate) == fpstate(original)
    assert a.events[0] == b.events[0]


@pytest.mark.usefixtures('x87_hints')
@pytest.mark.parametrize('site', [SITE, 0x7847f4, 0x784813, 0x784817, 0x78481d, 0x784821])
def test_all_declared_operations_have_actual_original_fp_and_memory_effects(tmp_path, site):
    compiled = compile_source(tmp_path, source(site))
    a, b = emu.Machine(image.primary()), machine(compiled)
    ra, rb = a.run(site, case(), 1), b.run(compiled.entry, case(), 100)
    assert ra.outcome == 'budget' and rb.outcome == 'return'
    assert fpstate(a) == fpstate(b)
    assert ra.events == rb.events[:len(ra.events)]
    assert bytes(a.uc.mem_read(emu.STACK, emu.STACK_SIZE)) == bytes(b.uc.mem_read(emu.STACK, emu.STACK_SIZE))
    assert b.x87_runtime.dispatches == 1 and b.x87_runtime.pending is None


@pytest.mark.usefixtures('x87_hints')
@pytest.mark.parametrize('word', [0x007f, 0x027f, 0x037f, 0x0340, 0x007e])
def test_explicit_control_words_remain_cpu_inputs(tmp_path, word):
    compiled = compile_source(tmp_path)
    fixture = replace(case(), fpcw=word)
    a, b = emu.Machine(image.primary()), machine(compiled)
    a.run(SITE, fixture, 1)
    assert b.run(compiled.entry, fixture, 100).outcome == 'return'
    assert fpstate(a) == fpstate(b)


@pytest.mark.usefixtures('x87_hints')
@pytest.mark.parametrize('entry', [SITE, NEXT])
def test_unowned_original_arrival_is_not_whitelisted(tmp_path, entry):
    vm = machine(compile_source(tmp_path))
    assert vm.run(entry, case(), 100).outcome == 'escape'
    assert vm.x87_runtime.dispatches == 0


@pytest.mark.usefixtures('x87_hints')
@pytest.mark.parametrize('which', ['original_instruction', 'compiled_instruction', 'original_successor', 'compiled_continuation'])
def test_changed_instruction_or_continuation_is_rejected(tmp_path, which):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    binding = compiled.image.x87_bindings[0]
    address = getattr(binding, which)
    # Change an immediate/displacement or a one-byte RET to another valid opcode.
    raw = bytes(vm.uc.mem_read(address, 1))
    vm.uc.mem_write(address, bytes([raw[0] ^ 1]))
    result = vm.run(compiled.entry, case(), 100)
    assert result.outcome == 'escape'
    assert 'changed' in vm.x87_runtime.rejection
    assert not result.events


@pytest.mark.usefixtures('x87_hints')
@pytest.mark.parametrize('origin,target', [(False, NEXT+3), (True, NEXT+3), (False, emu.STOP), (True, emu.STOP)])
def test_wrong_arrival_successor_and_unfinished_completion_reject(tmp_path, origin, target):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    at = SITE if origin else compiled.entry
    handle = vm.uc.hook_add(unicorn.UC_HOOK_CODE,
        lambda uc, address, size, user: uc.reg_write(R.UC_X86_REG_EIP, target), begin=at, end=at)
    assert vm.run(compiled.entry, case(), 100).outcome == 'escape'
    assert vm.x87_runtime.rejection
    vm.uc.hook_del(handle)
    assert vm.run(compiled.entry, case(), 100).outcome == 'return'
    assert vm.x87_runtime.dispatches == 1


@pytest.mark.usefixtures('x87_hints')
def test_actual_budget_interruption_resets_before_reuse(tmp_path):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    assert vm.run(compiled.entry, case(), 1).outcome == 'budget'
    assert vm.x87_runtime.pending
    assert vm.run(SITE, case(), 100).outcome == 'escape'
    assert vm.x87_runtime.pending is None
    assert vm.run(compiled.entry, case(), 100).outcome == 'return'


@pytest.mark.usefixtures('x87_hints')
@pytest.mark.parametrize('site,address,perms', [
    (SITE, emu.STACK_TOP+0x1c, unicorn.UC_PROT_EXEC),
    (0x7847f4, emu.STACK_TOP+0x24, unicorn.UC_PROT_READ | unicorn.UC_PROT_EXEC),
    (0x784813, emu.STACK_TOP+0x1c, unicorn.UC_PROT_EXEC),
    (0x784817, 0x968940, unicorn.UC_PROT_EXEC),
    (0x78481d, emu.STACK_TOP+0x24, unicorn.UC_PROT_EXEC),
])
def test_raw_memory_faults_preserve_fp_state_and_reset(tmp_path, site, address, perms):
    compiled = compile_source(tmp_path, source(site))
    a, b = emu.Machine(image.primary()), machine(compiled)
    for vm in (a, b):vm.uc.mem_protect(address & ~0xfff, 0x1000, perms)
    ra, rb = a.run(site, case(), 100), b.run(compiled.entry, case(), 100)
    assert ra.outcome == rb.outcome == 'fault'
    assert ra.detail == rb.detail and ra.regs == rb.regs
    assert ra.events == rb.events and ra.memory == rb.memory
    assert fpstate(a) == fpstate(b)
    assert bytes(a.uc.mem_read(emu.STACK, emu.STACK_SIZE)) == bytes(b.uc.mem_read(emu.STACK, emu.STACK_SIZE))
    assert b.x87_runtime.pending
    restored = unicorn.UC_PROT_READ | unicorn.UC_PROT_EXEC
    if emu.STACK <= address < emu.STACK+emu.STACK_SIZE:restored |= unicorn.UC_PROT_WRITE
    b.uc.mem_protect(address & ~0xfff, 0x1000, restored)
    assert b.run(site, case(), 100).outcome == 'escape'
    assert b.x87_runtime.pending is None
    assert b.run(compiled.entry, case(), 100).outcome == 'return'


@pytest.mark.usefixtures('x87_hints')
def test_original_instruction_fetch_fault_keeps_pending_then_resets(tmp_path):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    vm.uc.mem_protect(SITE & ~0xfff, 0x1000, unicorn.UC_PROT_READ)
    result = vm.run(compiled.entry, case(), 100)
    assert result.outcome == 'fault' and result.detail[-1] == SITE
    assert vm.x87_runtime.pending[0] == 'instruction'
    vm.uc.mem_protect(SITE & ~0xfff, 0x1000, unicorn.UC_PROT_READ | unicorn.UC_PROT_EXEC)
    assert vm.run(SITE, case(), 100).outcome == 'escape'
    assert vm.run(compiled.entry, case(), 100).outcome == 'return'


@pytest.mark.usefixtures('x87_hints')
def test_synthetic_vm_error_during_pending_instruction_resets(tmp_path):
    compiled = compile_source(tmp_path)
    vm = machine(compiled)
    def interrupt(uc, address, size, user):
        # Test lifecycle under a VM error, without claiming native x87 exception semantics.
        raise unicorn.UcError(unicorn.UC_ERR_EXCEPTION)
    handle = vm.uc.hook_add(unicorn.UC_HOOK_CODE, interrupt, begin=SITE, end=SITE)
    assert vm.run(compiled.entry, case(), 100).outcome == 'error'
    assert vm.x87_runtime.pending
    vm.uc.hook_del(handle)
    assert vm.run(SITE, case(), 100).outcome == 'escape'
    assert vm.run(compiled.entry, case(), 100).outcome == 'return'


@pytest.mark.usefixtures('x87_hints')
@pytest.mark.parametrize('old,new', [
    ('cmre_x87_007847e8:', 'cmre_x87_007847E8:'),
    ('cmre_x87_007847e8:', 'cmre_x87_007847e9:'),
    ('cmre_x87_return_007847e8_007847ec', 'cmre_x87_return_007847e8_007847ed'),
    ('cmre_x87_return_007847e8_007847ec:', 'missing_continuation:'),
    ('.byte 0xdb,0x44,0x24,0x1c', '.byte 0xdb,0x44,0x24,0x18'),
])
def test_malformed_unowned_changed_or_missing_binding_rejects(tmp_path, old, new):
    assert old in source()
    with pytest.raises(build.BuildError, match='x87 transport'):
        compile_source(tmp_path, source().replace(old, new))


def test_parent_declaration_is_required(tmp_path):
    with pytest.raises(build.BuildError, match='x87 transport'):
        compile_source(tmp_path)


@pytest.mark.usefixtures('x87_hints')
def test_unmarked_copy_does_not_gain_original_pc_execution(tmp_path):
    text=source().replace('cmre_x87_007847e8:', 'ordinary_fp:').replace('cmre_x87_return_007847e8_007847ec:', 'ordinary_next:')
    compiled=compile_source(tmp_path,text)
    vm=machine(compiled)
    assert not compiled.image.x87_bindings and vm.x87_runtime is None
    assert vm.run(SITE,case(),100).outcome=='escape'
    assert vm.run(compiled.entry,case(),100).outcome=='return'
    assert fpstate(vm)['FIP']!=SITE


def test_worker_cannot_change_original_x87_bindings():
    with pytest.raises(ValueError,match='worker hints'):
        difftest.validate_hints({'original_x87':{}},worker=True)


@pytest.mark.parametrize('declarations', [[], {'0x007847e8': {}},
    {'0x007847E8': {'bytes': 'db44241c', 'evidence': 'original'}},
    {'0x007847e8': {'bytes': 'DB44241C', 'evidence': 'original'}},
    {'0x007847e8': {'bytes': 'db44241c', 'evidence': ''}}])
def test_parent_declaration_schema_rejects_ambiguous_or_unpinned_values(declarations):
    with pytest.raises(ValueError, match='original_x87'):
        difftest.validate_hints({'original_x87': declarations})


@pytest.mark.usefixtures('x87_hints')
def test_marker_inside_native_instruction_is_not_an_owned_boundary(tmp_path):
    # The original FILD bytes are now the immediate of a native MOV instruction.
    text = source().replace('v_007847e8:', '.byte 0xb8\\nv_007847e8:')
    with pytest.raises(build.BuildError, match='owned boundaries'):
        compile_source(tmp_path, text)


@pytest.mark.usefixtures('x87_hints')
def test_marker_cannot_belong_to_another_native_owner(tmp_path):
    text = source().replace('cm_fn_00784100(void)', 'cm_fn_00785d40(void)')
    text += 'void __attribute__((naked)) cm_fn_00784100(void) { __asm__("ret"); }'
    with pytest.raises(build.BuildError, match='owned boundaries'):
        compile_source(tmp_path, text)


def test_supported_opcodes_remain_bounded_to_declared_forms():
    import capstone
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    # Isolated decoder controls, not recovered engine behavior.
    for raw in ('d800', 'db28', 'd918', 'dd38', 'df20', 'd9e8', '90'):
        instruction = next(decoder.disasm(bytes.fromhex(raw), 0x1000))
        assert not x87_transport._supported(instruction), instruction.mnemonic


@pytest.mark.usefixtures('x87_hints')
def test_single_c_cannot_admit_only_one_of_six_declared_original_sites(tmp_path):
    path = tmp_path/'fp.c'; path.write_text(source())
    report = recover.test_c(OWNER, {'cases': 1}, fresh=True, source=path)
    assert report['status'] == 'fail'
    assert all(row['status'] == 'build-failed' and 'exactly every selected' in row['detail']
               for row in report['builds'].values())


@pytest.mark.parametrize('path', ['run', 'explain', 'composition'])
@pytest.mark.parametrize('changed', [False, True])
def test_validation_paths_compare_numeric_fip_even_when_ordinary_results_match(monkeypatch, path, changed):
    """Comparator scaffolding: equal ordinary results, one numeric FIP difference."""
    result = emu.Result('return', (), (0,)*9, [], {}, {OWNER})
    count = [0]
    def vm(*args, **kwargs):
        index = count[0]; count[0] += 1
        return SimpleNamespace(run=lambda *a: result, _escape=None,
            uc=SimpleNamespace(hook_add=lambda *a, **k: None,
                reg_read=lambda reg: SITE+int(changed and index==1) if reg==R.UC_X86_REG_FIP else 0))
    monkeypatch.setattr(emu, 'Machine', vm)
    monkeypatch.setattr(difftest, 'arena_templates', lambda: ())
    monkeypatch.setattr(difftest, 'make_case', lambda *a: case())
    built = SimpleNamespace(symbols={}, entry=env.LINK_BASE, image=SimpleNamespace(
        x87_bindings=(object(),), text=SimpleNamespace(va=env.LINK_BASE, end=env.LINK_BASE+0x1000)))
    opts = difftest.Options(1,100,0,0,0.6,1,{})
    if path == 'composition':
        body = lift.discover(OWNER)
        report = composition._run(OWNER,{OWNER:body},built,{OWNER:env.LINK_BASE},opts)
        difference = report['mismatches'][0] if report['mismatches'] else {}
    elif path == 'explain':
        report = difftest.explain(OWNER,(OWNER,OWNER+1),{OWNER},built,{},opts,'abi',case_index=0)
        difference = report['difference']
    else:
        report = difftest.run(OWNER,(OWNER,OWNER+1),{OWNER},built,{},opts,'abi')
        difference = report['mismatches'][0] if report['mismatches'] else {}
    assert bool(difference) == changed
    if changed:
        assert difference['field'] == 'fp' and difference['registers'] == ['FIP']
        assert difference['original']['FIP'] == SITE
        assert difference['recovered']['FIP'] == SITE+1


@pytest.mark.parametrize('path', ['run', 'composition'])
def test_original_fp_state_participates_in_validation_digest(monkeypatch, path):
    """Equal-pair policy scaffold isolates the original FP digest contribution."""
    result = emu.Result('return', (), (0,)*9, [], {}, {OWNER})
    initial = [SITE]
    monkeypatch.setattr(emu, 'Machine', lambda *a, **k: SimpleNamespace(
        run=lambda *a: result, _escape=None, uc=SimpleNamespace(hook_add=lambda *a, **k: None,
        reg_read=lambda reg: initial[0] if reg==R.UC_X86_REG_FIP else 0)))
    monkeypatch.setattr(difftest, 'arena_templates', lambda: ())
    monkeypatch.setattr(difftest, 'make_case', lambda *a: None)
    built = SimpleNamespace(symbols={}, entry=env.LINK_BASE, image=SimpleNamespace(
        x87_bindings=(object(),), text=SimpleNamespace(va=env.LINK_BASE, end=env.LINK_BASE+0x1000)))
    opts = difftest.Options(1,100,0,0,0.6,1,{})
    def run():
        if path == 'composition':
            return composition._run(OWNER,{OWNER:lift.discover(OWNER)},built,{OWNER:env.LINK_BASE},opts)
        return difftest.run(OWNER,(OWNER,OWNER+1),{OWNER},built,{},opts,'abi')
    first = run(); initial[0] += 1; second = run()
    assert not first['mismatches'] and not second['mismatches']
    assert first['result_digest'] != second['result_digest']


def combined_source():
    return source()+'''void __attribute__((naked)) cm_fn_0094f0f0(void) {
 __asm__("v_0094f13d:\\ncmre_jump_0094f13d:\\n.byte 0xff,0x24,0x85,0x50,0xf1,0x94,0x00\\n"
         "v_0094f160:\\nret\\nv_0094f18c:\\nret\\nv_0094f1b0:\\nret\\n");
}
'''


def test_actual_idle_indexed_jump_binding_coexists(tmp_path, table_hints, x87_hints):
    compiled=compile_source(tmp_path,combined_source())
    vm=machine(compiled)
    assert vm.jump_runtime and vm.x87_runtime
    assert vm.run(compiled.entry,case(),100).outcome=='return'
    assert vm.x87_runtime.dispatches==1 and vm.jump_runtime.pending is None
    fixture=case();fixture.regs['eax']=1
    assert vm.run(compiled.symbols['_cm_fn_0094f0f0'],fixture,100).outcome=='return'
    assert vm.jump_runtime.dispatches==1 and vm.x87_runtime.pending is None


def test_pending_indexed_jump_cannot_overlap_x87(tmp_path, table_hints, x87_hints):
    compiled=compile_source(tmp_path,combined_source());vm=machine(compiled)
    def collide(uc,address,size,user):vm.jump_runtime.pending=object()
    handle=vm.uc.hook_add(unicorn.UC_HOOK_CODE,collide,begin=compiled.entry,end=compiled.entry)
    assert vm.run(compiled.entry,case(),100).outcome=='escape'
    assert 'overlapping' in vm.x87_runtime.rejection
    vm.uc.hook_del(handle)
    assert vm.run(compiled.entry,case(),100).outcome=='return'


def caller_source():
    return source()+'''void __attribute__((naked)) cm_fn_00785d40(void) {
 __asm__(CM_ORIGINAL_CALL(00785fd4,00785fd9,00784100) "ret\\n");
}
'''


@pytest.mark.usefixtures('x87_hints')
def test_active_call_frame_coexists_and_returns_through_owned_slot(tmp_path):
    compiled=compile_source(tmp_path,caller_source());vm=machine(compiled)
    result=vm.run(compiled.symbols['_cm_fn_00785d40'],case(),100)
    assert result.outcome=='return'
    assert vm.x87_runtime.dispatches==1 and vm.return_runtime.dispatches==1
    assert not vm.return_runtime.frames


@pytest.mark.usefixtures('x87_hints')
def test_pending_call_cannot_overlap_x87(tmp_path):
    compiled=compile_source(tmp_path,caller_source());vm=machine(compiled)
    def collide(uc,address,size,user):vm.return_runtime.pending=object()
    handle=vm.uc.hook_add(unicorn.UC_HOOK_CODE,collide,begin=compiled.entry,end=compiled.entry)
    assert vm.run(compiled.entry,case(),100).outcome=='escape'
    assert 'overlapping' in vm.x87_runtime.rejection
    vm.uc.hook_del(handle)
    assert vm.run(compiled.entry,case(),100).outcome=='return'
