"""Synthetic observer states test the diagnostic contract, not recovered game logic."""
from types import SimpleNamespace

import pytest

from cmre import composition, composition_diagnostic as diagnostic, difftest, emu, recover, semantic

A, B = 0x400000, 0x400100
LINK = 0x6e5d1000


def result(outcome='return', *, pc=A, access=19, address=0x3001000, size=4, value=7,
           saved=11, memory=None, events=(), fault_mapping=None):
    registers = (value, 0, 0, saved, emu.STACK_TOP + 4, 12, 13, 14, 0x202)
    detail = ((access, address, size), pc) if outcome == 'fault' else ('instruction budget',pc) if outcome=='budget' else ()
    return emu.Result(outcome, detail, registers, list(events), memory or {}, {A},
                      fault_mapping=fault_mapping)


@pytest.fixture
def run_pair(monkeypatch):
    def run(original_rows, candidate_rows, *, returns='int', original_fp=(0x37f,0), candidate_fp=(0x37f,0)):
        class Machine:
            def __init__(self, _image, recovered=None, **_kwargs):
                self.rows = iter(candidate_rows if recovered else original_rows)
                self.normal_fp = candidate_fp if recovered else original_fp
                self.fault_fp = (0,0) if recovered else (0,7)
                self.uc = SimpleNamespace(hook_add=lambda *_a, **_k: None,
                    fp=candidate_fp if recovered else original_fp)
                self._escape = lambda *_args: None
            def run(self, *_args):
                row = next(self.rows)
                self.uc.fp = self.fault_fp if row.outcome=='fault' else self.normal_fp
                return row
        monkeypatch.setattr(emu, 'Machine', Machine)
        monkeypatch.setattr(diagnostic.image, 'primary', lambda: object())
        monkeypatch.setattr(difftest, 'make_case', lambda *_args: None)
        monkeypatch.setattr(difftest, 'arena_templates', lambda: ())
        monkeypatch.setattr(difftest, 'semantic_fp', lambda uc, _returns: uc.fp)
        monkeypatch.setattr(composition, '_verified_fault_locations', lambda *_args: {})
        body = SimpleNamespace(va=A, region_end=A+1,
            insns={A:SimpleNamespace(va=A,size=1,kind='plain',mnemonic='nop')})
        built = SimpleNamespace(symbols={},image=SimpleNamespace(text=SimpleNamespace(va=LINK,end=LINK+0x1000)))
        opts = SimpleNamespace(hints={},seed=1,cases=len(original_rows),budget=1000,
                               discovery=0,min_coverage=1,inputs='v2')
        return diagnostic._run(A,{A:body},built,{A:LINK},opts,returns)
    return run


def test_same_fault_effects_ignore_allocation_but_retain_abi_difference(run_pair):
    original = [result('fault',pc=A,value=111),result()]
    candidate = [result('fault',pc=LINK+50,value=222,saved=99),result()]
    report = run_pair(original,candidate)
    assert report['status'] == 'pass'
    assert report['abi_comparison_status'] == 'fail'
    assert report['abi_first_mismatches'] and not report['mismatches']


@pytest.mark.parametrize('changed', [
    {'address':0x3001004}, {'size':2}, {'access':20},
    {'memory':{0x3000000:b'changed'}},
    {'events':[('c','test-import',1,(8,))]},
])
def test_fault_near_misses_still_fail(run_pair,changed):
    report = run_pair([result('fault'),result()],
                      [result('fault',pc=LINK+50,**changed),result()])
    assert report['status'] == 'fail'
    assert report['mismatches']


@pytest.mark.parametrize('change', ['return','saved','fpcw','depth'])
def test_return_value_saved_registers_and_fp_state_remain_observable(run_pair,change):
    candidate = result(value=8) if change=='return' else result(saved=99) if change=='saved' else result()
    fp = (0x27f,0) if change=='fpcw' else (0x37f,1) if change=='depth' else (0x37f,0)
    report = run_pair([result()],[candidate],candidate_fp=fp)
    assert report['status'] == 'fail' and report['mismatches']


def test_fault_only_corpus_is_not_a_passing_diagnostic(run_pair):
    assert run_pair([result('fault')],[result('fault')])['status']=='fail'


@pytest.mark.parametrize('near_miss', [False, True])
def test_mapping_relation_keeps_raw_faults_and_its_page_proof(run_pair, near_miss):
    address = LINK + 0x100
    proof = {'schema':'cmre-fault-mapping/1','raw_access':22,'projected_access':20,
        'relation':'candidate-original-unmapped-write','applied':False,
        'original_mapping':'unmapped','address':address,'size':4,
        'pages':[{'page':LINK,'origin':'recovered-pe','original_mapped':False,
            'common_mapped':False,'initial_perms':5,'current_perms':5,
            'recovered_sections':[{'va':LINK,'end':LINK+0x1000,'writable':False}]}]}
    original = result('fault',access=20,address=address + (4 if near_miss else 0))
    candidate = result('fault',pc=LINK+50,access=22,address=address,fault_mapping=proof)
    report = run_pair([original,result()],[candidate,result()])
    assert report['status'] == ('fail' if near_miss else 'pass')
    retained = report['mapping_faults'][0]
    assert retained['proof'] == proof and retained['case'] == 0
    assert retained['original_detail'] == original.detail
    assert retained['recovered_detail'] == candidate.detail
    assert retained['relation_used'] is (not near_miss)
    assert retained['abi_relation_used'] is False


@pytest.mark.parametrize('candidate_outcome', ['budget','return'])
def test_budget_outcomes_stay_inconclusive_and_failing(run_pair,candidate_outcome):
    report = run_pair([result('budget')],[result(candidate_outcome)])
    assert report['status']=='fail' and report['inconclusive']==1


@pytest.mark.parametrize('reader', ['original', 'recovered', 'both'])
def test_selected_code_reads_cannot_pass_as_original_data(run_pair, reader):
    events = [('r',A,1,0x90)]
    report = run_pair([result(events=events if reader != 'recovered' else ())],
                      [result(events=events if reader != 'original' else ())])
    assert report['status']=='fail' and report['code_as_data_cases']==1


def test_return_declaration_uses_existing_caller_width_guard(tmp_path,monkeypatch):
    source=tmp_path/'fn_00400000.c'
    source.write_text('void cm_fn_00400000(void) {}',encoding='utf-8')
    body=SimpleNamespace(problems=[],insns={})
    monkeypatch.setattr(semantic,'eax_readers',lambda _va:[0x400200])
    monkeypatch.setattr(semantic,'edx_readers',lambda _va:[])
    with pytest.raises(ValueError,match='callers read EAX'):
        diagnostic._contracts({A:source},{A:body})


@pytest.mark.parametrize('kind,mnemonic',[('indirect_call','call'),('table_jmp','jmp'),('plain','fild')])
def test_unsupported_original_boundaries_are_explicit(tmp_path,kind,mnemonic):
    source=tmp_path/'fn_00400000.c'
    source.write_text('unsigned cm_fn_00400000(void) {return 0;}',encoding='utf-8')
    body=SimpleNamespace(problems=[],insns={A:SimpleNamespace(kind=kind,mnemonic=mnemonic)})
    with pytest.raises(ValueError,match='excludes selected indirect/x87'):
        diagnostic._contracts({A:source},{A:body})


def test_diagnostic_refuses_hidden_assembly_macro(tmp_path):
    source=tmp_path/'fn_00400000.c'
    source.write_text('#define STEP __asm__("nop")\nunsigned cm_fn_00400000(void) {STEP;return 0;}',encoding='utf-8')
    with pytest.raises(ValueError,match='plain C'):
        diagnostic._contracts({A:source},{A:SimpleNamespace(problems=[],insns={})})


@pytest.mark.parametrize('changed', ['assembly', 'dependency-drift'])
def test_expanded_O2_inspection_is_fail_closed_without_game_assets(tmp_path, monkeypatch, changed):
    source = tmp_path/'fn_00400000.c'
    source.write_text('unsigned cm_fn_00400000(void) {return 0;}')
    calls = []
    clean, expanded = 'unsigned cm_fn_00400000(void) {return 0;}', 'void f(void) {__asm__("nop");}'
    def preprocess(argv):
        opt = next(word for word in argv if word in ('-O0', '-O2'))
        calls.append(opt)
        return SimpleNamespace(returncode=0, stdout=clean if opt=='-O0' else expanded, stderr='')
    monkeypatch.setattr(diagnostic.env,'tool',lambda _name:'fake-gcc')
    monkeypatch.setattr(diagnostic.env,'run_tool',preprocess)
    monkeypatch.setattr(recover,'_semantic_contract',lambda *_args:('int',[]))
    hashes = {'-O0':diagnostic.env.digest_bytes(clean.encode()),
              '-O2':diagnostic.env.digest_bytes((clean if changed=='dependency-drift' else expanded).encode())}
    with pytest.raises(ValueError,match='plain C' if changed=='assembly' else 'dependency proof'):
        diagnostic._plain_translations({A:source},{source.name:{'preprocessed':hashes}},{A:'int'})
    assert calls == ['-O0','-O2']


def test_cli_reports_cannot_enter_composition_gate_namespace(tmp_path,monkeypatch,capsys):
    from cmre import campaign,env
    monkeypatch.setattr(env,'ANALYSIS',tmp_path)
    monkeypatch.setattr(campaign,'load',lambda _path:{})
    monkeypatch.setattr(diagnostic,'diagnose',lambda *_args:{
        'status':'pass','observed_composed_calls_at_both_optimizations':True})
    assert diagnostic._command(SimpleNamespace(manifest='unused',cases=None,json=False))==0
    import json
    shown=json.loads(capsys.readouterr().out)
    assert shown['diagnostic_only'] and not shown['composition_gate_passed']
    assert not shown['milestone_accepted']
    assert len(list((tmp_path/'cmre/composition-diagnostics').glob('*.json')))==1
    assert not (tmp_path/'cmre/compositions').exists()
