"""Real getter bodies exercise separate linked C; helper wiring is test scaffolding."""
import pytest

from cmre import campaign, composition_diagnostic as diagnostic, env

A,B=0x004671b0,0x004671c0
CALLER='''#include "cmre.h"
extern void *CM_THISCALL cm_fn_004671c0(void *self);
void *CM_THISCALL cm_fn_004671b0(void *self) {
 return (void *)((u32)cm_fn_004671c0(self)-0x50u);
}
'''
CALLEE='''#include "cmre.h"
__attribute__((noinline)) void *CM_THISCALL cm_fn_004671c0(void *self) {
 return (void *)((u32)self+0x70u);
}
'''


@pytest.mark.parametrize('mode',['faithful','changed-return','original-fallback','original-byte-read'])
def test_linked_diagnostic_preserves_values_and_original_escape_guard(tmp_path,monkeypatch,mode):
    monkeypatch.setattr(env,'C_DIR',tmp_path)
    caller=CALLER
    callee=CALLEE
    if mode=='changed-return':callee=callee.replace('0x70u','0x74u')
    if mode=='original-fallback':caller=caller.replace('cm_fn_004671c0(self)',
        '((void *(__attribute__((thiscall)) *)(void *))0x004671c0u)(self)')
    if mode=='original-byte-read':callee=callee.replace(' return (void *)',
        ' (void)*(volatile u8 *)0x004671b0u;\n return (void *)')
    (tmp_path/'fn_004671b0.c').write_text(caller,encoding='utf-8')
    (tmp_path/'fn_004671c0.c').write_text(callee,encoding='utf-8')
    report=diagnostic.diagnose(campaign.create('diagnostic-link-test',[A,B]),{'cases':8})
    assert report['diagnostic_only'] and not report['composition_gate_passed']
    assert not report['milestone_accepted']
    assert report['status']==('pass' if mode=='faithful' else 'fail'),report
    if mode=='faithful':assert report['observed_composed_calls_at_both_optimizations']
    for built in report['builds'].values():
        if mode=='original-byte-read':assert built['entries']['fn_004671b0']['code_as_data_cases']
        elif mode!='faithful':assert built['entries']['fn_004671b0']['mismatches']


@pytest.mark.parametrize('conditional', [False, True])
def test_expanded_assembly_is_refused_even_when_raw_spelling_hides_it(tmp_path, monkeypatch, conditional):
    monkeypatch.setattr(env,'C_DIR',tmp_path)
    macro = '#define JOIN(a,b) a##b\n'
    statement = ' JOIN(__as,m__)("nop");\n'
    if conditional:
        statement = '#ifdef __OPTIMIZE__\n' + statement + '#endif\n'
    source = macro + CALLEE.replace(' return (void *)',statement + ' return (void *)')
    (tmp_path/'fn_004671b0.c').write_text(CALLER,encoding='utf-8')
    (tmp_path/'fn_004671c0.c').write_text(source,encoding='utf-8')
    with pytest.raises(ValueError,match='plain C'):
        diagnostic.diagnose(campaign.create('hidden-asm',[A,B]),{'cases':8})


def test_macro_return_type_cannot_hide_a_floating_contract(tmp_path, monkeypatch):
    monkeypatch.setattr(env,'C_DIR',tmp_path)
    source = '#define REAL float\n' + CALLEE.replace('void *CM_THISCALL cm_fn_004671c0',
        'REAL CM_THISCALL cm_fn_004671c0')
    (tmp_path/'fn_004671b0.c').write_text(CALLER,encoding='utf-8')
    (tmp_path/'fn_004671c0.c').write_text(source,encoding='utf-8')
    with pytest.raises(ValueError,match='floating return'):
        diagnostic.diagnose(campaign.create('hidden-float',[A,B]),{'cases':8})
