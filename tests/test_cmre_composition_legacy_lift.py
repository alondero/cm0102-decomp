"""PROPOSAL ONLY: explicit legacy lift companion composition regressions.

Assumed API: composition.test(manifest, overrides=None, *, lift_companions=...)
where values are explicit selected preferred-image VAs. Adapt report key names to
implementation, but never weaken the asserted tier/identity/transport semantics.
All synthetic caller code below is test scaffolding, not recovered game source.
"""
from dataclasses import replace
from pathlib import Path
import struct
import unicorn
import pytest
from cmre import build,campaign,composition,emu,env,image,ledger,lift

CALLER=0x008aff50
LEGACY=0x00417c10
OTHER=0x00945333
CALL_SITE=0x008aff7c
RETURN_SITE=0x008aff81

CALLER_SOURCE=r'''#include "cmre.h"
u32 __attribute__((naked)) cm_fn_008aff50(void) {
    __asm__("movl $0x03002000, %ecx\n"
            "pushl $0x03001000\n"
            "pushl $0x03003000\n"
            CM_ORIGINAL_CALL(008aff7c,008aff81,00417c10)
            "ret\n");
}
'''

@pytest.fixture
def setup(tmp_path,monkeypatch):
    monkeypatch.setattr(env,'C_DIR',tmp_path)
    hints=tmp_path/'hints';hints.mkdir();monkeypatch.setattr(env,'HINTS_DIR',hints)
    caller=tmp_path/'fn_008aff50.c';caller.write_text(CALLER_SOURCE,encoding='utf8')
    # `fn_00417c10.c` deliberately does not exist: it is represented by the
    # frozen legacy ledger and explicit generated lift, never an implicit C file.
    manifest=campaign.create('legacy-mixed',[CALLER,LEGACY])
    return caller,manifest


def _rejected(action):
    try:
        result=action()
    except (ValueError,OSError,build.BuildError):
        return
    assert result['status']!='pass', result
    assert not result.get('observed_composed_calls_at_both_optimizations',False), result


def _real_transport_probe(monkeypatch):
    """Probe actual linked code despite mocking corpus results for speed.

    The synthetic caller invokes the original-declared call site with real ECX
    and stack values. Confirm selected extent traps, two distinct object entries,
    original numeric return word, copied date words and RET8. Not game evidence.
    """
    seen=[]
    def probe(va,bodies,built,symbols,opts):
        binding=next(b for b in built.image.return_bindings if b.original_call==CALL_SITE)
        assert binding.original_return==RETURN_SITE
        assert binding.original_callee==LEGACY
        assert binding.callee_entry==symbols[LEGACY]!=LEGACY
        assert built.image.text.va<=symbols[CALLER]<built.image.text.end
        assert built.image.text.va<=symbols[LEGACY]<built.image.text.end
        if va==CALLER:
            regs={k:0 for k in emu.REG_NAMES};regs['eflags']=0x202
            arena=bytearray(emu.ARENA_SIZE)
            struct.pack_into('<HHI',arena,0x1000,0x1234,0x5678,0x10203040)
            case=emu.Case(regs,[0]*10,bytes(arena))
            vm=emu.Machine(image.primary(),built.image)
            for body in bodies.values():
                vm.uc.hook_add(unicorn.UC_HOOK_CODE,vm._escape,begin=body.va,end=body.region_end-1)
            visits=[]
            vm.uc.hook_add(unicorn.UC_HOOK_CODE,lambda *args:visits.append(1),begin=symbols[LEGACY],end=symbols[LEGACY])
            result=vm.run(symbols[CALLER],case,2000)
            assert result.outcome=='return',result
            assert len(visits)==1
            assert ('w',emu.STACK_TOP-12,4,RETURN_SITE) in result.events
            assert bytes(vm.uc.mem_read(emu.ARENA+0x2000,8))==struct.pack('<HHI',0x1234,0x5678,0x10203040)
            assert bytes(vm.uc.mem_read(emu.ARENA+0x3000,8))==struct.pack('<HHI',0x1234,0x5678,0x10203040)
            assert result.regs[4]==emu.STACK_TOP+4
            seen.append((binding.original_call,binding.callee_entry))
        return {'status':'pass','observed_other_selected_entries':(['fn_00417c10'] if va==CALLER else []),
                'selected_entry_visits':{'fn_00417c10':1}}
    monkeypatch.setattr(composition,'_run',probe)
    return seen


def test_explicit_legacy_lift_binds_real_compiled_assignment_and_reports_tier(setup,monkeypatch):
    caller,manifest=setup
    seen=_real_transport_probe(monkeypatch)
    source_before=env.digest(caller)
    report=composition.test(manifest,{'cases':1},lift_companions=(LEGACY,))
    assert report['status']=='pass',report
    assert report['observed_composed_calls_at_both_optimizations'] is True
    # Four calls: both optimizations run twice; a test stub supplies statuses,
    # so this is NOT a 600-case or milestone proof.
    assert len(seen)==4
    tiers=report['execution_tiers']
    assert tiers['fn_008aff50']['execution_tier']=='c'
    assert tiers['fn_00417c10']['execution_tier']=='lift-companion'
    assert tiers['fn_00417c10']['provenance']=='fresh-original-derived-legacy'
    assert tiers['fn_00417c10']['ledger']=='recovery/date/ledger.json'
    assert tiers['fn_00417c10']['source_sha256']
    assert report['milestone_accepted'] is False
    assert report['source_sha256']['fn_008aff50.c']==source_before
    assert env.digest(caller)==source_before and not report['changed_inputs']
    for result in report['builds'].values():
        calls=[b for b in result['return_transport']['bindings'] if b['original_call']==CALL_SITE]
        assert len(calls)==1 and calls[0]['callee_entry']!=LEGACY
        assert calls[0]['callee_entry']==int(result['compiled_symbols']['fn_00417c10'],16)


def test_no_implicit_legacy_source_fallback(setup):
    _,manifest=setup
    _rejected(lambda:composition.test(manifest,{'cases':1}))


@pytest.mark.parametrize('variants',[(LEGACY,LEGACY),(OTHER,),(0x004142e0,)])
def test_reject_duplicate_unselected_or_unproven_lift_variant(setup,variants):
    _,manifest=setup
    _rejected(lambda:composition.test(manifest,{'cases':1},lift_companions=variants))


def test_reject_forged_legacy_original_extent_hash(setup,monkeypatch):
    _,manifest=setup
    original=ledger.legacy_entries
    def forged():
        return [replace(e,sha256='0'*64) if e.va==LEGACY else e for e in original()]
    monkeypatch.setattr(ledger,'legacy_entries',forged)
    _rejected(lambda:composition.test(manifest,{'cases':1},lift_companions=(LEGACY,)))


def test_reject_forged_generated_instruction_source(setup,monkeypatch):
    _,manifest=setup
    original=lift.render
    def forged(body):
        value=original(body)
        return value.replace('mov eax, dword ptr [esp + 0xc]','xor eax, eax') if body.va==LEGACY else value
    monkeypatch.setattr(lift,'render',forged)
    _rejected(lambda:composition.test(manifest,{'cases':1},lift_companions=(LEGACY,)))


@pytest.mark.parametrize('relative',['recovery/date/ledger.json','recovery/date/cm_date.c','recovery/date/cm_date.h'])
def test_legacy_dependency_drift_invalidates_report(setup,monkeypatch,relative):
    _,manifest=setup
    active={'value':False}
    digest=env.digest
    watched=(env.ROOT/relative).resolve()
    def changed(path):
        return '0'*64 if active['value'] and Path(path).resolve()==watched else digest(path)
    monkeypatch.setattr(env,'digest',changed)
    def mutation(*args):
        active['value']=True
        return {'status':'pass','observed_other_selected_entries':[],'selected_entry_visits':{}}
    monkeypatch.setattr(composition,'_run',mutation)
    report=composition.test(manifest,{'cases':1},lift_companions=(LEGACY,))
    assert report['status']=='fail'
    assert report['changed_inputs'], report
    assert any('legacy' in item or relative in item for item in report['changed_inputs']),report


def test_selected_nonlegacy_lift_is_not_implicitly_admitted(setup):
    # Initial scope is the explicitly reviewed 00417c10 legacy companion only.
    # 00945333 needs additional indirect-call transport and cannot be generalized.
    _,_=setup
    manifest=campaign.create('unproven-indirect-lift',[CALLER,OTHER])
    _rejected(lambda:composition.test(manifest,{'cases':1},lift_companions=(OTHER,)))


def test_reject_legacy_source_that_differs_from_recorded_validation(setup,monkeypatch):
    _,manifest=setup
    digest=env.digest
    watched=(env.ROOT/'recovery/date/cm_date.c').resolve()
    monkeypatch.setattr(env,'digest',lambda path:'0'*64 if Path(path).resolve()==watched else digest(path))
    _rejected(lambda:composition.test(manifest,{'cases':1},lift_companions=(LEGACY,)))
