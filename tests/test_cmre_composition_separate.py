"""Proposed separate-TU regressions; copy/adapt into tests after coordinator approval.

Helper contract: composition._build_separate(sources, entry_symbol, workdir, opt)
returns build.Built and raises build.BuildError; exact implementation is pending.
"""
from pathlib import Path
import struct
from types import SimpleNamespace

import pytest

from cmre import build, campaign, composition, emu, env, image, lift

A, B = 0x004671B0, 0x004671C0

# Existing real-original getter fixtures, intentionally incompatible as a shared
# translation unit. B's prototype differs only in 32-bit return interpretation;
# it is not a general license for mismatched ABI declarations.
CALLER = r'''#include "cmre.h"
#define PRIVATE_OFFSET 0x50u
static __attribute__((unused)) unsigned local_helper(void) { return PRIVATE_OFFSET; }
extern unsigned CM_THISCALL cm_fn_004671c0(void *self);
CM_THISCALL void *cm_fn_004671b0(void *self) {
    return (void *)((u32)cm_fn_004671c0(self) - PRIVATE_OFFSET);
}
'''
CALLEE = r'''#include "cmre.h"
#define PRIVATE_OFFSET 0x70u
static __attribute__((unused)) unsigned local_helper(void) { return PRIVATE_OFFSET; }
__attribute__((noinline)) CM_THISCALL void *cm_fn_004671c0(void *self) {
    return (void *)((u32)self + PRIVATE_OFFSET);
}
'''

@pytest.fixture
def source_pair(tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'C_DIR', tmp_path)
    # The real suite's conftest.py already provides this isolation.
    hints = tmp_path / 'hints'
    hints.mkdir()
    monkeypatch.setattr(env, 'HINTS_DIR', hints)
    caller = tmp_path / 'fn_004671b0.c'
    callee = tmp_path / 'fn_004671c0.c'
    caller.write_text(CALLER, encoding='utf-8')
    callee.write_text(CALLEE, encoding='utf-8')
    return caller, callee


def test_separate_units_allow_private_name_and_declaration_collisions(source_pair):
    before = {p.name: env.digest(p) for p in source_pair}
    report = composition.test(campaign.create('isolated-translation-units', [A, B]), {'cases': 12})
    assert report['status'] == 'pass', report
    assert report['observed_composed_calls_at_both_optimizations'] is True
    assert report['source_sha256'] == before
    assert not report['changed_inputs']
    for opt, result in report['builds'].items():
        assert result['entries']['fn_004671b0']['observed_other_selected_entries'] == ['fn_004671c0']
        assert all(int(address, 16) >= env.LINK_BASE for address in result['compiled_symbols'].values())
        # Final build schema must expose exact per-object and linked identities.
        assert set(result['object_sha256']) == set(before)
        assert all(len(h) == 64 for h in result['object_sha256'].values())
        assert len(result['linked_sha256']) == 64
    assert {p.name: env.digest(p) for p in source_pair} == before


@pytest.mark.parametrize('opt', ['-O0', '-O2'])
def test_local_assembly_label_inside_accepted_owner_is_not_a_function_boundary(tmp_path, opt):
    # The accepted date routine contains a real local function before its entry,
    # then a class-6 `_date_finish` label inside that entry before two VA markers.
    # Its original source bytes stay untouched; a second object satisfies the
    # separate-composition interface without introducing another date definition.
    date = env.ROOT / 'recovery/c/fn_00549ef0.c'
    other = tmp_path / 'fn_004671b0.c'
    other.write_text('void cm_fn_004671b0(void) {}\n', encoding='utf-8')
    work = tmp_path / opt[1:]
    work.mkdir()
    built = composition._build_separate([date, other], 'cm_fn_00549ef0', work, opt)
    assert built.symbols['_date_finish'] < built.symbols['v_00549f4f']
    assert built.symbols['v_00549f4f'] < built.symbols['v_00549f5a']


@pytest.mark.parametrize('opt', ['-O0', '-O2'])
@pytest.mark.parametrize('change', ['text', 'label', 'section', 'source', 'owner', 'header', 'compiler', 'assembler'])
def test_date_finish_reviewed_identity_rejects_drift(tmp_path, monkeypatch, opt, change):
    source = env.ROOT / 'recovery/c/fn_00549ef0.c'
    obj = tmp_path / 'date.o'
    cmd = [env.tool('gcc'), '-m32', '-c', str(source), '-o', str(obj), '-Wall', '-Wextra', '-Werror',
           opt, '-mno-stack-arg-probe', '-fno-asynchronous-unwind-tables', '-ffreestanding', '-fno-builtin',
           f'-I{env.ROOT / "recovery/include"}']
    result = env.run_tool(cmd)
    assert result.returncode == 0, result.stderr
    rows = composition._object_sections(obj)
    digest = env.digest(source)
    assert composition._reviewed_date_finish(0x00549ef0, digest, obj, rows, opt)
    owner = 0x00549ef0
    if change == 'text':
        actual = composition._coff_text(obj)
        monkeypatch.setattr(composition, '_coff_text', lambda _: bytes([actual[0] ^ 1]) + actual[1:])
    elif change in ('label', 'section'):
        rows = [(section + int(change == 'section'), kind, offset,
                 name + ('_changed' if change == 'label' else ''), storage)
                if name == '_date_finish' else (section, kind, offset, name, storage)
                for section, kind, offset, name, storage in rows]
    elif change == 'source':
        digest = '0' * 64
    elif change == 'owner':
        owner = 0x00549ef1
    elif change == 'header':
        original = env.digest
        monkeypatch.setattr(env, 'digest', lambda p: '0' * 64 if Path(p) == env.ROOT / 'recovery/include/cmre.h' else original(p))
    else:
        from copy import deepcopy
        identity = deepcopy(env.toolchain_identity())
        identity['gcc' if change == 'compiler' else 'as']['sha256'] = '0' * 64
        monkeypatch.setattr(env, 'toolchain_identity', lambda: identity)
    assert not composition._reviewed_date_finish(owner, digest, obj, rows, opt)


def test_date_finish_rejects_unchanged_text_with_added_relocation(tmp_path):
    source = env.ROOT / 'recovery/c/fn_00549ef0.c'
    obj = tmp_path / 'date.o'
    cmd = [env.tool('gcc'), '-m32', '-c', str(source), '-o', str(obj), '-Wall', '-Wextra', '-Werror',
           '-O0', '-mno-stack-arg-probe', '-fno-asynchronous-unwind-tables', '-ffreestanding', '-fno-builtin',
           f'-I{env.ROOT / "recovery/include"}']
    result = env.run_tool(cmd)
    assert result.returncode == 0, result.stderr
    rows = composition._object_sections(obj)
    source_hash = env.digest(source)
    assert composition._reviewed_date_finish(0x00549ef0, source_hash, obj, rows, '-O0')
    # Append a COFF DIR32 relocation at .text+3. The object text and symbol
    # rows are unchanged, but linking would alter the helper's immediate.
    data = bytearray(obj.read_bytes())
    relocation_offset = len(data)
    data.extend(struct.pack('<IIH', 3, 2, 6))
    struct.pack_into('<I', data, 20 + 24, relocation_offset)
    struct.pack_into('<H', data, 20 + 32, 1)
    obj.write_bytes(data)
    assert composition._object_sections(obj) == rows
    with pytest.raises(build.BuildError, match='relocation'):
        composition._reviewed_date_finish(0x00549ef0, source_hash, obj, rows, '-O0')


@pytest.mark.parametrize('opt', ['-O0', '-O2'])
def test_owned_original_call_targets_compiled_separate_object(tmp_path, opt):
    # These marker bytes and ownership are independently verified against the
    # primary pinned executable by the ordinary return transport binder.
    caller = tmp_path / 'fn_007e5160.c'
    callee = tmp_path / 'fn_00785790.c'
    caller.write_text(r'''#include "cmre.h"
void cm_fn_004671b0(void);
__attribute__((used)) static void outside(void) { cm_fn_004671b0(); }
unsigned __attribute__((naked)) cm_fn_007e5160(void) {
    __asm__("pushl $0x12345678\n pushl $0\n"
            CM_ORIGINAL_CALL(007e522f, 007e5234, 00785790)
            "movl -12(%esp), %eax\n ret\n");
}
''', encoding='utf-8')
    callee.write_text(r'''void __attribute__((naked)) cm_fn_00785790(void) {
    __asm__("ret $8\n");
}
''', encoding='utf-8')
    work = tmp_path / opt[1:]
    work.mkdir()
    compiled = composition._build_separate([caller, callee], 'cm_fn_007e5160', work, opt)
    entry = next(address for name, address in compiled.symbols.items()
                 if build.DECORATED.fullmatch(name) and '00785790' in name
                 and compiled.image.text.va <= address < compiled.image.text.end)
    binding = next(b for b in compiled.image.return_bindings if b.original_call == 0x007e522f)
    assert binding.callee_entry == entry
    assert binding.callee_entry != 0x00785790
    outside = next(address for name, address in compiled.symbols.items()
                   if build.DECORATED.fullmatch(name) and '004671b0' in name)
    assert outside == 0x004671b0
    regs = dict.fromkeys(emu.REG_NAMES, 0)
    regs['eflags'] = 0x202
    machine = emu.Machine(image.primary(), compiled.image,
                          watch=(0x007e5160, lift.discover(0x007e5160).region_end), escape=True)
    result = machine.run(compiled.entry, emu.Case(regs, [0] * 10, bytes(emu.ARENA_SIZE)), 1000)
    assert result.outcome == 'return', result
    assert result.regs[0] == 0x007e5234
    assert machine.return_runtime.dispatches == 1


def test_selected_unresolved_decoration_never_falls_back_to_original(tmp_path):
    caller = tmp_path / 'fn_004671b0.c'
    callee = tmp_path / 'fn_004671c0.c'
    caller.write_text(r'''#include "cmre.h"
extern void CM_STDCALL cm_fn_004671c0(unsigned);
void cm_fn_004671b0(void) { cm_fn_004671c0(1); }
''', encoding='utf-8')
    callee.write_text('void cm_fn_004671c0(unsigned x) { (void)x; }\n', encoding='utf-8')
    with pytest.raises(build.BuildError, match='selected|decorat|unresolved'):
        composition._build_separate([caller, callee], 'cm_fn_004671b0', tmp_path, '-O0')


def test_duplicate_selected_definition_is_rejected(tmp_path):
    a = tmp_path / 'fn_004671b0.c'
    b = tmp_path / 'fn_004671c0.c'
    a.write_text('void cm_fn_004671b0(void) {}\nvoid cm_fn_004671c0(void) {}\n', encoding='utf-8')
    b.write_text('void cm_fn_004671c0(void) {}\n', encoding='utf-8')
    with pytest.raises(build.BuildError):
        composition._build_separate([a, b], 'cm_fn_004671b0', tmp_path, '-O0')


def test_duplicate_original_location_label_is_rejected(tmp_path):
    a = tmp_path / 'fn_004671b0.c'
    b = tmp_path / 'fn_004671c0.c'
    a.write_text('void cm_fn_004671b0(void) { __asm__("v_004671b0: nop"); }\n', encoding='utf-8')
    b.write_text('void cm_fn_004671c0(void) { __asm__("v_004671b0: nop"); }\n', encoding='utf-8')
    with pytest.raises(build.BuildError, match='ambiguous|duplicate|label|symbol'):
        composition._build_separate([a, b], 'cm_fn_004671b0', tmp_path, '-O0')


def test_original_location_label_in_wrong_selected_source_is_rejected(tmp_path):
    a = tmp_path / 'fn_004671b0.c'
    b = tmp_path / 'fn_004671c0.c'
    a.write_text('void cm_fn_004671b0(void) {}\n', encoding='utf-8')
    b.write_text('void cm_fn_004671c0(void) { __asm__("v_004671b0: nop"); }\n', encoding='utf-8')
    with pytest.raises(build.BuildError, match='owner|location'):
        composition._build_separate([a, b], 'cm_fn_004671b0', tmp_path, '-O0')


def test_call_marker_in_other_selected_function_is_rejected(tmp_path):
    # Pinned original CALL 007e522f belongs to fn_007e5160, not the object
    # which defines fn_00785790. Its bytes and declared target are otherwise
    # valid, so admission must check the compiled marker's actual owner.
    caller = tmp_path / 'fn_007e5160.c'
    other = tmp_path / 'fn_00785790.c'
    caller.write_text('void cm_fn_007e5160(void) {}\n', encoding='utf-8')
    other.write_text(r'''#include "cmre.h"
void __attribute__((naked)) cm_fn_00785790(void) {
    __asm__("pushl $0x12345678\n pushl $0\n"
            CM_ORIGINAL_CALL(007e522f, 007e5234, 00785790)
            "ret\n");
}
''', encoding='utf-8')
    with pytest.raises(build.BuildError, match='owner'):
        composition._build_separate([caller, other], 'cm_fn_007e5160', tmp_path, '-O0')


def test_selected_entry_defined_in_other_source_is_rejected(tmp_path):
    a = tmp_path / 'fn_007e5160.c'
    b = tmp_path / 'fn_00785790.c'
    a.write_text(r'''#include "cmre.h"
void __attribute__((naked)) cm_fn_00785790(void) {
    __asm__("pushl $0x12345678\n pushl $0\n"
            CM_ORIGINAL_CALL(007e522f, 007e5234, 00785790)
            "ret\n");
}
''', encoding='utf-8')
    b.write_text('void cm_fn_007e5160(void) {}\n', encoding='utf-8')
    with pytest.raises(build.BuildError, match='owner|source|ambiguous'):
        composition._build_separate([a, b], 'cm_fn_007e5160', tmp_path, '-O0')


def test_call_marker_in_same_source_helper_is_rejected(tmp_path):
    a = tmp_path / 'fn_007e5160.c'
    b = tmp_path / 'fn_00785790.c'
    a.write_text(r'''#include "cmre.h"
__attribute__((naked, used)) static void hidden(void) {
    __asm__("pushl $0x12345678\n pushl $0\n"
            CM_ORIGINAL_CALL(007e522f, 007e5234, 00785790)
            "ret\n");
}
void cm_fn_007e5160(void) {}
''', encoding='utf-8')
    b.write_text('void cm_fn_00785790(void) {}\n', encoding='utf-8')
    with pytest.raises(build.BuildError, match='owner|function'):
        composition._build_separate([a, b], 'cm_fn_007e5160', tmp_path, '-O0')


@pytest.mark.parametrize('helper', [
    'static void hidden(void) __asm__("v_hidden");\n'
    '__attribute__((naked,used)) static void hidden(void)',
    '__attribute__((naked,used,section(".text$extra"))) static void hidden(void)',
])
def test_call_marker_in_named_or_other_section_helper_is_rejected(tmp_path, helper):
    a = tmp_path / 'fn_007e5160.c'
    b = tmp_path / 'fn_00785790.c'
    a.write_text('''#include "cmre.h"
void cm_fn_007e5160(void) {}
''' + helper + r''' {
    __asm__("pushl $0x12345678\n pushl $0\n"
            CM_ORIGINAL_CALL(007e522f, 007e5234, 00785790)
            "ret\n");
}
''', encoding='utf-8')
    b.write_text('void cm_fn_00785790(void) {}\n', encoding='utf-8')
    with pytest.raises(build.BuildError, match='owner|function'):
        composition._build_separate([a, b], 'cm_fn_007e5160', tmp_path, '-O0')


def test_call_marker_in_untyped_assembler_helper_is_rejected(tmp_path):
    a = tmp_path / 'fn_007e5160.c'
    b = tmp_path / 'fn_00785790.c'
    a.write_text(r'''#include "cmre.h"
void cm_fn_007e5160(void) {}
__asm__(".globl _hidden\n_hidden:\n pushl $0x12345678\n pushl $0\n"
        CM_ORIGINAL_CALL(007e522f, 007e5234, 00785790) "ret\n");
''', encoding='utf-8')
    b.write_text('void cm_fn_00785790(void) {}\n', encoding='utf-8')
    with pytest.raises(build.BuildError, match='owner|function'):
        composition._build_separate([a, b], 'cm_fn_007e5160', tmp_path, '-O0')


def test_call_marker_in_untyped_local_assembler_helper_is_rejected(tmp_path):
    a = tmp_path / 'fn_007e5160.c'
    b = tmp_path / 'fn_00785790.c'
    a.write_text(r'''#include "cmre.h"
void cm_fn_007e5160(void) {}
__asm__("_hidden_local:\n pushl $0x12345678\n pushl $0\n"
        CM_ORIGINAL_CALL(007e522f, 007e5234, 00785790) "ret\n");
''', encoding='utf-8')
    b.write_text('void cm_fn_00785790(void) {}\n', encoding='utf-8')
    with pytest.raises(build.BuildError, match='owner|function'):
        composition._build_separate([a, b], 'cm_fn_007e5160', tmp_path, '-O0')


def test_missing_coff_marker_metadata_is_rejected(tmp_path, monkeypatch):
    a = tmp_path / 'fn_007e5160.c'
    b = tmp_path / 'fn_00785790.c'
    a.write_text(r'''#include "cmre.h"
void cm_fn_007e5160(void) {}
__asm__(".globl _hidden\n_hidden:\n pushl $0x12345678\n pushl $0\n"
        CM_ORIGINAL_CALL(007e522f, 007e5234, 00785790) "ret\n");
''', encoding='utf-8')
    b.write_text('void cm_fn_00785790(void) {}\n', encoding='utf-8')
    actual_sections = composition._object_sections
    def omit_marker_rows(path):
        return [row for row in actual_sections(path)
                if not row[3].startswith(('v_', 'cmre_'))]
    monkeypatch.setattr(composition, '_object_sections', omit_marker_rows)
    with pytest.raises(build.BuildError, match='metadata|marker'):
        composition._build_separate([a, b], 'cm_fn_007e5160', tmp_path, '-O0')


def test_missing_coff_helper_metadata_is_rejected(tmp_path, monkeypatch):
    a = tmp_path / 'fn_007e5160.c'
    b = tmp_path / 'fn_00785790.c'
    a.write_text(r'''#include "cmre.h"
void cm_fn_007e5160(void) {}
__asm__(".globl _hidden\n_hidden:\n pushl $0x12345678\n pushl $0\n"
        CM_ORIGINAL_CALL(007e522f, 007e5234, 00785790) "ret\n");
''', encoding='utf-8')
    b.write_text('void cm_fn_00785790(void) {}\n', encoding='utf-8')
    actual_sections = composition._object_sections
    monkeypatch.setattr(composition, '_object_sections',
                        lambda path: [row for row in actual_sections(path) if row[3] != '_hidden'])
    with pytest.raises(build.BuildError, match='metadata|symbol'):
        composition._build_separate([a, b], 'cm_fn_007e5160', tmp_path, '-O0')

@pytest.fixture
def policy_sources(tmp_path, monkeypatch):
    monkeypatch.setattr(env, 'C_DIR', tmp_path)
    hints = tmp_path / 'hints'
    hints.mkdir()
    monkeypatch.setattr(env, 'HINTS_DIR', hints)
    (tmp_path / 'fn_004671b0.c').write_text('''#include "cmre.h"
extern CM_THISCALL void *cm_fn_004671c0(void *self);
CM_THISCALL void *cm_fn_004671b0(void *self) {
    return (void *)((u32)cm_fn_004671c0(self) - 0x50u);
}
''', encoding='utf-8')
    (tmp_path / 'fn_004671c0.c').write_text('''#include "cmre.h"
__attribute__((noinline)) CM_THISCALL void *cm_fn_004671c0(void *self) {
    return (void *)((u32)self + 0x70u);
}
''', encoding='utf-8')
    return tmp_path


def test_composition_uses_each_accepted_c_input_policy(policy_sources, monkeypatch):
    from cmre import recover
    monkeypatch.setattr(recover, 'c_inputs', lambda va: {A: 'v2', B: 'v1'}[va])
    actual_run = composition._run
    seen = []
    def observe(va, lifted, built, symbols, opts):
        seen.append((va, opts.inputs))
        return actual_run(va, lifted, built, symbols, opts)
    monkeypatch.setattr(composition, '_run', observe)
    report = composition.test(campaign.create('policy-test', [A, B]), {'cases': 12})
    assert report['status'] == 'pass', report
    assert report['options']['fn_004671b0']['inputs'] == 'v2'
    assert report['options']['fn_004671c0']['inputs'] == 'v1'
    assert seen.count((A, 'v2')) == 4 and seen.count((B, 'v1')) == 4
    assert len(seen) == 8


def test_composition_detects_c_input_policy_drift(policy_sources, monkeypatch):
    from cmre import recover
    policy = {A: 'v2', B: 'v1'}
    monkeypatch.setattr(recover, 'c_inputs', lambda va: policy[va])
    actual_run = composition._run
    def mutate_during_first_run(va, lifted, built, symbols, opts):
        result = actual_run(va, lifted, built, symbols, opts)
        policy[A] = 'v1'
        return result
    monkeypatch.setattr(composition, '_run', mutate_during_first_run)
    report = composition.test(campaign.create('policy-drift', [A, B]), {'cases': 12})
    assert report['options']['fn_004671b0']['inputs'] == 'v2'
    assert report['status'] == 'fail'
    assert 'fn_004671b0.c:test-inputs' in report['changed_inputs']
    assert 'fn_004671c0.c:test-inputs' not in report['changed_inputs']


def test_composition_detects_objdump_identity_drift(policy_sources, monkeypatch):
    objdump = Path(env.tool('objdump'))
    actual_digest, actual_run = env.digest, composition._run
    drifted = False
    def digest(path):
        if drifted and Path(path) == objdump:
            return '0' * 64
        return actual_digest(path)
    def mutate_during_run(va, lifted, built, symbols, opts):
        nonlocal drifted
        result = actual_run(va, lifted, built, symbols, opts)
        drifted = True
        return result
    monkeypatch.setattr(env, 'digest', digest)
    monkeypatch.setattr(composition, '_run', mutate_during_run)
    report = composition.test(campaign.create('objdump-drift', [A, B]), {'cases': 12})
    assert report['status'] == 'fail'
    assert 'objdump' in report['changed_inputs']


def test_composition_detects_its_own_source_drift(policy_sources, monkeypatch):
    source = Path(composition.__file__)
    actual_digest, actual_run = env.digest, composition._run
    initial = actual_digest(source)
    drifted = False
    def digest(path):
        if drifted and Path(path) == source:
            return '0' * 64
        return actual_digest(path)
    def mutate_during_run(va, lifted, built, symbols, opts):
        nonlocal drifted
        result = actual_run(va, lifted, built, symbols, opts)
        drifted = True
        return result
    monkeypatch.setattr(env, 'digest', digest)
    monkeypatch.setattr(composition, '_run', mutate_during_run)
    report = composition.test(campaign.create('composition-source-drift', [A, B]), {'cases': 12})
    assert report['status'] == 'fail'
    assert 'composition' in report['changed_inputs']
    assert report['composition_sha256'] == initial


def test_compose_cli_reports_build_error_as_structured_error(monkeypatch, capsys):
    monkeypatch.setattr(campaign, 'load', lambda path: {})
    def fail_build(manifest, overrides, **kwargs):
        raise build.BuildError('objdump version unavailable', 'probe failed')
    monkeypatch.setattr(composition, 'test', fail_build)
    code = composition._command(SimpleNamespace(manifest='unused', cases=None, json=False))
    import json
    response = json.loads(capsys.readouterr().out)
    assert code == 1
    assert response == {'status': 'error', 'detail': 'objdump version unavailable'}
