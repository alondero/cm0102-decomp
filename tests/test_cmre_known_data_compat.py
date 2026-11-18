"""Closed known-data proof: real source-pair recognition and adversarial origins."""
import ast
import copy
import json
from pathlib import Path
import pytest
from cmre import known_data_compat as p
from cmre import env, ledger, recover, validation as v

FIXTURES = Path(__file__).parent / "fixtures"
AFTER = FIXTURES / "known_data_recover_after.py"


def pair():
    return ((FIXTURES / "known_data_recover_before.py").read_bytes().decode("utf-8"),
            AFTER.read_bytes().decode("utf-8"))


def test_exact_actual_reviewed_source_pair():
    p.check_sources(*pair())

@pytest.mark.parametrize('side', [0,1])
def test_unrecognized_source_hash_pair_rejected(side):
    sources=list(pair());sources[side]+='\n'
    with pytest.raises(ValueError,match='source pair'):p.check_sources(*sources)

@pytest.mark.parametrize('old,new',[
 ('data_spans=_data_spans(lifted)','data_spans=()'),
 ('data_spans=_data_spans(lifted)','data_spans=_data_spans(lifted), unknown=True'),
 ('data_spans=_data_spans(lifted), returns=returns','data_spans=_data_spans(lifted), returns="void"'),
 ('spans = [(d.va, d.va + len(d.raw))','spans = [(d.va + 1, d.va + len(d.raw))'),
 ("not record.get('c') and (contract := _withdrawn_c_contract(record))","record.get('c') and (contract := _withdrawn_c_contract(record))"),
])
def test_ast_backstop_rejects_guest_or_policy_changes_even_if_pair_hash_guard_were_widened(monkeypatch,old,new):
    a,b=pair();assert old in b;b=b.replace(old,new,1);monkeypatch.setattr(p,'NEW_RECOVER',env.digest_bytes(b.encode()))
    with pytest.raises(ValueError):p.check_sources(a,b)

def identities():
    a, b = pair()
    modules = next(node.value for node in ast.parse(b).body if isinstance(node, ast.Assign)
                   and any(isinstance(target, ast.Name) and target.id == 'HARNESS_MODULES'
                           for target in node.targets))
    components = {name: env.digest(Path(recover.__file__).with_name(name))
                  for name in ast.literal_eval(modules)}
    components['recover.py'] = env.digest(AFTER)
    before={'harness':p.OLD_HARNESS,'components':{**components,'recover.py':p.OLD_RECOVER},'planner':'old',
            'dispatch_sources':{'recover.py':a,'difftest.py':Path(recover.__file__).with_name('difftest.py').read_bytes().decode()},
            'target':'pinned','python':'same','dependencies':{'unicorn':'same'},'build_environment':{},'workspace':'same'}
    return before,{**copy.deepcopy(before),'harness':p.NEW_HARNESS,'components':components,'planner':'new',
                   'dispatch_sources':{**before['dispatch_sources'],'recover.py':b}}

def test_exact_guest_transition(monkeypatch):
    digest = env.digest
    def historical_digest(path):
        return digest(AFTER if path.name == 'recover.py' else path)

    monkeypatch.setattr(env, 'digest', historical_digest)
    p.transition(*identities())


def test_historical_transition_rejects_current_recover_changes():
    assert env.digest(Path(recover.__file__)) != env.digest(AFTER)
    with pytest.raises(ValueError, match='Current guest module changed'):
        p.transition(*identities())

@pytest.mark.parametrize('field,value', [('harness','unknown'),('target','changed'),('python','changed'),
 ('build_environment',{'CPATH':'unknown'}),('workspace','other'),('dependencies',{'unicorn':'different'})])
def test_global_drift_rejected(field,value):
    a,b=identities();b[field]=value
    with pytest.raises(ValueError):p.transition(a,b)

def test_classifier_or_vm_component_change_rejected():
    for name in ['difftest.py','emu.py','build.py']:
        a,b=identities();b['components'][name]='changed'
        with pytest.raises(ValueError):p.transition(a,b)

def write(path,data):
    path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(data,indent=2)+'\n')

@pytest.fixture
def world(tmp_path,monkeypatch):
    monkeypatch.setattr(env,'ROOT',tmp_path);monkeypatch.setattr(env,'C_DIR',tmp_path/'recovery/c');monkeypatch.setattr(env,'LEDGER_DIR',tmp_path/'recovery/ledger');monkeypatch.setattr(env,'RUNS',tmp_path/'runs');monkeypatch.setattr(env,'HINTS_DIR',tmp_path/'recovery/hints')
    va=0x401000;source=env.C_DIR/'fn_00401000.c';source.parent.mkdir(parents=True);source.write_text('#include "cmre.h"\nu32 cm_fn_00401000(void) { return 1; }\n')
    header=tmp_path/'recovery/include/cmre.h';header.parent.mkdir(parents=True);header.write_text('typedef unsigned int u32;\n')
    opts={'cases':600,'budget':200000,'seed':va,'discovery':24,'arg_count':10,'min_coverage':.6,'inputs':'v2','hints':{'fixtures':[]}}
    monkeypatch.setattr(v,'effective_options',lambda *_:copy.deepcopy(opts))
    monkeypatch.setattr(recover,'_semantic_contract',lambda *_:('int',[]))
    build={'files':{env.rel(source):env.digest(source),env.rel(header):env.digest(header)},'preprocessed':{'-O0':'0'*64,'-O2':'2'*64}}
    monkeypatch.setattr(recover,'c_dependencies',lambda _:copy.deepcopy(build))
    current={'options':v.compact_options(opts),'source':env.digest(source),'source_path':env.rel(source),'hints_path':'recovery/hints/fn_00401000.json','profile':'semantic','optimizations':['-O0','-O2'],'boundary':'outside callees execute pinned original code','build':build,'include_members':[env.rel(header)],'jump_contract':None,'x87_contract':None}
    before={'harness':p.OLD_HARNESS,'build_environment':{},'target':'pinned'};ex=p.execution(before)
    key=recover._key(ex,source.read_bytes().decode(),header.read_text(),env.rel(source),build,opts,['semantic','int'],recover.C_OPT_LEVELS,None,None)
    d={'profile':'semantic','returns':'int','inputs':'v2','cases':600,'seed':va,'budget':200000,'coverage':.7,'mismatches':[],'inconclusive':0,'code_as_data_cases':0,'outcomes':{'return':600},'result_digest':'d'*64}
    report={'status':'pass','tier':'c','key':key,'execution_identity':ex,'harness_sha256':p.OLD_HARNESS,'source_sha256':current['source'],'source_dependencies':build['files'],'preprocessed_sha256':build['preprocessed'],'profile':'semantic','returns':'int','problems':[],
            'builds':{opt:{'status':'pass','difftest':copy.deepcopy(d)} for opt in recover.C_OPT_LEVELS}}
    accepted={'source_sha256':current['source'],'run_key':key,'harness_sha256':p.OLD_HARNESS,'source_dependencies':build['files'],'preprocessed_sha256':build['preprocessed'],
              'validation':{opt:{k:x for k,x in d.items() if k in ['profile','returns','inputs','cases','seed','outcomes','coverage','inconclusive','result_digest']} for opt in recover.C_OPT_LEVELS}}
    record={'va':hex(va),'c':accepted};current['accepted']={'source_sha256':current['source'],'run_key':key,'harness_sha256':p.OLD_HARNESS,'sha256':v.digest(accepted)}
    write(ledger.record_path(va),record);path=tmp_path/'authenticated-report.json';write(path,report)
    index=[{'va':hex(va),'accepted_source_sha256':current['source'],'report':str(path),'report_sha256':env.digest(path)}]
    return {'va':va,'source':source,'header':header,'opts':opts,'current':current,'before':before,'report':report,'record':record,'path':path,'index':index,'prior':{'results':{}},'tmp':tmp_path}

def candidate(w):return p.candidate(w['va'],'c',w['current'],w['before'],w['index'],w['prior'])

def save_report(w):
    write(w['path'],w['report']);w['index'][0]['report_sha256']=env.digest(w['path'])

def test_authentic_zero_cad_report_proves_old_identity_without_relabel(world):
    origin,error=candidate(world);assert error is None and origin['ok']
    assert origin['origin']['harness']==p.OLD_HARNESS and origin['identity']==v.digest(p.execution(world['before']))
    assert world['report']['harness_sha256']==p.OLD_HARNESS

@pytest.mark.parametrize('field,value', [('code_as_data_cases',1),('code_as_data_cases',None),('code_as_data_cases',False),
 ('mismatches',{}),('mismatches',None),('mismatches',[{'kind':'budget-asymmetry'}]),('inconclusive',None),('inconclusive',False),('inconclusive',301)])
def test_actual_counters_and_lists_required_at_each_optimization(world,field,value):
    world['report']['builds']['-O2']['difftest'][field]=value;save_report(world)
    assert candidate(world)[0] is None

def test_missing_counter_rejected_even_if_report_reauthenticated(world):
    del world['report']['builds']['-O0']['difftest']['code_as_data_cases'];save_report(world);assert candidate(world)[0] is None

def test_report_status_label_does_not_replace_actual_build_pass(world):
    world['report']['builds']['-O0']['status']='fail';save_report(world);assert candidate(world)[0] is None

def test_post_origin_counter_mutation_is_detected_even_when_old_key_and_ledger_metrics_agree(world):
    world['report']['builds']['-O0']['difftest']['code_as_data_cases']=1;write(world['path'],world['report'])
    assert 'Authenticated' in candidate(world)[1]

@pytest.mark.parametrize('name', ['seed','discovery','budget','arg_count','inputs','hints'])
def test_complete_effective_options_key_drift_forces_execution(world,name):
    world['opts'][name]=({'fixtures':[],'eax':'ptr'} if name=='hints' else 'v1' if name=='inputs' else world['opts'][name]+1)
    world['current']['options']=v.compact_options(world['opts']);assert candidate(world)[0] is None

def test_source_change_forces_execution(world):
    world['source'].write_text('different');assert candidate(world)[0] is None

def test_selected_header_change_forces_execution(world):
    world['header'].write_text('different');assert candidate(world)[0] is None

def test_cpp_change_forces_execution(world):
    world['current']['build']['preprocessed']['-O0']='changed';assert candidate(world)[0] is None

def test_fresh_cpp_detects_new_source_directory_header_shadowing_old_selected_header(world,monkeypatch):
    shadow=world['source'].parent/'cmre.h';shadow.write_text('new selected header')
    actual=copy.deepcopy(world['current']['build']);actual['files'][env.rel(shadow)]=env.digest(shadow)
    monkeypatch.setattr(recover,'c_dependencies',lambda _:actual)
    assert 'Fresh current CPP' in candidate(world)[1]

def test_actual_preprocessing_is_requested_even_when_cached_snapshot_and_hashes_match(world,monkeypatch):
    calls=[]
    def actual(source):calls.append(source);return copy.deepcopy(world['current']['build'])
    monkeypatch.setattr(recover,'c_dependencies',actual);assert candidate(world)[0] and calls==[world['source']]

def test_profile_change_forces_execution(world):
    for d in world['record']['c']['validation'].values():d['profile']='abi'
    write(ledger.record_path(world['va']),world['record']);assert candidate(world)[0] is None

def test_new_return_contract_forces_execution(world,monkeypatch):
    monkeypatch.setattr(recover,'_semantic_contract',lambda *_:('int64',[]));assert candidate(world)[0] is None

def test_jump_or_x87_binding_change_forces_execution(world):
    for name in ['jump_contract','x87_contract']:
        world['current'][name]={'path':'hint','sha256':'changed'};assert candidate(world)[0] is None;world['current'][name]=None

def test_inactive_or_retired_c_never_reused(world):
    world['record'].pop('c');world['record']['c_retractions']=[{'unknown':'history'}];write(ledger.record_path(world['va']),world['record']);assert candidate(world)[0] is None

def test_active_c_short_circuits_unrelated_retirement_history(world,monkeypatch):
    world['record']['c_retractions']=[{'malformed':'irrelevant while Cactive'}];write(ledger.record_path(world['va']),world['record'])
    monkeypatch.setattr(recover,'_withdrawn_c_contract',lambda *_:pytest.fail('inactive policy called for active C'))
    assert candidate(world)[0] is not None

def test_missing_authenticated_origin_forces_execution(world):
    world['index'].clear();assert candidate(world)[0] is None

def test_prior_failure_on_identical_inputs_is_not_overridden(world):
    world['prior']['results']['00401000-c']={'ok':False,'dependencies':world['current']};assert candidate(world)[0] is None

@pytest.mark.parametrize('text',['__TIME__','__DATE__','__has_include("x")','x ## y','__asm__(".incbin hidden")','a\\\nsm(".include hidden")'])
def test_uncertain_compiler_and_assembler_dependencies_force_execution(world,text):
    world['source'].write_text(text);world['current']['build']['files'][env.rel(world['source'])]=env.digest(world['source'])
    with pytest.raises(ValueError):p._safe_c_build(world['current'])

@pytest.mark.parametrize('writes',[{'0x0':1,'0x00':2},{'0x0':1,'0x1':2}])
def test_json_key_sorting_cannot_hide_order_dependent_overlap(writes):
    for field in ['arena_writes','globals']:
        with pytest.raises(ValueError,match='Overlapping'):p._ordered_inputs({'hints':{'fixtures':[{field:writes}]}})

def test_budget_skip_evidence_is_preserved_not_invented(world):
    for d in world['report']['builds'].values():d['difftest']['inconclusive']=2;d['difftest']['outcomes']={'return':598,'budget':2}
    for d in world['record']['c']['validation'].values():d['inconclusive']=2;d['outcomes']={'return':598,'budget':2}
    write(ledger.record_path(world['va']),world['record']);save_report(world);origin,error=candidate(world);assert origin and not error
    assert world['report']['builds']['-O0']['difftest']['inconclusive']==2
    assert origin['inconclusive_cases']==4 and origin['differential_executions']==0

def test_same_inputs_ignores_only_acceptance_provenance(world):
    a=world['current'];b=copy.deepcopy(a);b['accepted']={'different':'metadata'};assert p.same_inputs(a,b)
    for key in ['source','profile','options','build','jump_contract']:
        changed=copy.deepcopy(b);changed[key]='changed';assert not p.same_inputs(a,changed)

def test_unknown_prefix_and_build_environment_never_migrated(world):
    world['before']['build_environment']={'CPATH':'unknown'};assert candidate(world)[0] is None

def test_missing_full_pytest_evidence_is_rejected(tmp_path,monkeypatch):
    path=tmp_path/'receipt.json';write(path,{'steps':[]});monkeypatch.setattr(p.session_evidence,'identity',lambda _: {'files':{}})
    with pytest.raises(ValueError,match='full pytest'):p._regressions(path)

def test_dedicated_schema_routes_to_own_checker(tmp_path,monkeypatch):
    from cmre import validation_compat
    path=tmp_path/'proof.json';write(path,{'schema':p.SCHEMA})
    expected=p.Scope({'00401000-c':{'identity':'old'}})
    monkeypatch.setattr(p,'check',lambda data,prior,identity:expected)
    assert validation_compat.check(path,{'old':1},{'new':2}) is expected

@pytest.fixture
def planned(world,monkeypatch):
    from cmre import validation_compat
    origin,error=candidate(world);assert origin and not error
    old_identity={'harness':p.OLD_HARNESS};new_identity={'harness':p.NEW_HARNESS}
    prior={'identity':old_identity,'results':{}} # newer accepted C absent from old inventory
    old_path=world['tmp']/'old.json';write(old_path,prior)
    proof=world['tmp']/'proof.json';write(proof,{'schema':p.SCHEMA,'before_receipt':str(old_path),'after':new_identity})
    scope=p.Scope({'00401000-c':origin})
    monkeypatch.setattr(v,'global_identity',lambda:new_identity)
    monkeypatch.setattr(v,'_prior',lambda _:(old_path,prior))
    monkeypatch.setattr(v,'snapshot',lambda va,tier,*a,**kw:copy.deepcopy(world['current']) if tier=='c' else {'source':'lift'})
    monkeypatch.setattr(v,'accepted_report',lambda *_:None)
    monkeypatch.setattr(v,'composition_impact',lambda *_:[])
    monkeypatch.setattr(v,'coverage_counts',lambda:{})
    monkeypatch.setattr(validation_compat,'check',lambda *a:scope)
    monkeypatch.setattr(env,'ANALYSIS',world['tmp']/'analysis')
    data=v.plan([world['va']],reuse=str(old_path),compatibility=str(proof),refresh=True)
    return data,scope,proof,origin

def test_real_planner_can_adopt_authenticated_origin_missing_from_prior_inventory(planned):
    data,scope,proof,origin=planned;item=data['items']['00401000-c']
    assert item['action']=='reuse' and item['previous']==origin
    assert item['evidence_adoption']['kind']=='known-data-proof'
    assert item['previous']['identity']!=v.digest(data['identity'])
    assert data['items']['00401000-lifted']['action']=='execute'
    assert 'tier outside verified compatibility scope' in data['items']['00401000-lifted']['reasons']
    assert 'global execution/harness identity changed or unavailable' not in data['items']['00401000-lifted']['reasons']

def test_supported_cli_executes_exceptions_after_verified_proof_without_global_bypass(planned,world,monkeypatch):
    from argparse import Namespace
    from types import SimpleNamespace
    from cmre import cli
    data,scope,proof,origin=planned;calls=[]
    monkeypatch.setattr(v,'plan',lambda *a,**kw:data)
    monkeypatch.setattr(ledger,'cmre_entries',lambda:[SimpleNamespace(va=world['va'])])
    def execute(item,identity,refresh):
        calls.append(item['tier'])
        return {'ok':True,'identity':identity,'dependencies':item['dependencies'],'differential_executions':1}
    monkeypatch.setattr(v,'execute',execute)
    args=Namespace(n=10**9,jobs=1,retry_failed=False,resume=None,reuse='retained',plan=False,audit=False,
                   compatibility=str(proof),output=None,refresh=True,json=False)
    assert cli.cmd_revalidate(args)==0
    assert calls==['lifted'] # proved C reused, actual exception selected for execution

def test_supported_cli_still_rejects_unproven_global_change_before_execution(planned,world,monkeypatch):
    from argparse import Namespace
    from types import SimpleNamespace
    from cmre import cli
    data,scope,proof,origin=planned;data['compatibility']=None
    data['items']['00401000-lifted']['reasons']=['global execution/harness identity changed or unavailable']
    monkeypatch.setattr(v,'plan',lambda *a,**kw:data)
    monkeypatch.setattr(ledger,'cmre_entries',lambda:[SimpleNamespace(va=world['va'])])
    monkeypatch.setattr(v,'execute',lambda *a:pytest.fail('unproven guest execution'))
    args=Namespace(n=10**9,jobs=1,retry_failed=False,resume=None,reuse='retained',plan=False,audit=False,
                   compatibility=None,output=None,refresh=True,json=False)
    with pytest.raises(ValueError,match='Unproven global harness change'):cli.cmd_revalidate(args)

@pytest.mark.parametrize('drift',['none','scope-dropped','proof-body'])
def test_finalization_rechecks_dedicated_eligibility_even_when_cached_snapshot_stays_equal(planned,monkeypatch,drift):
    data,scope,proof,origin=planned
    data['items'].pop('00401000-lifted');run=v.Run(data)
    assert run.data['results']['00401000-c']['identity']==origin['identity']
    if drift=='scope-dropped':scope.clear() # fresh CPP rejected new source-dir shadow
    if drift=='proof-body':proof.write_text('changed')
    run.finish();result=run.data['results']['00401000-c']
    assert result['ok'] is (drift=='none')
    if drift!='none':
        assert result['inconclusive'] and '00401000-c' in run.data['invalidated_during_run']

def checked_proof(world,monkeypatch):
    origin,error=candidate(world);assert origin and not error
    before=world['before'];after={'harness':p.NEW_HARNESS};prior={'identity':before,'results':{}}
    data={'schema':p.SCHEMA,'before':before,'after':after,'reviewer':'independent','reason':'strict transition',
          'checker_sha256':{n:env.digest(Path(p.__file__).with_name(n)) for n in p.CHECKERS},
          'prior_results_sha256':v.digest({}),'selection':['00401000'],'tiers':['00401000-c'],'scope':{'00401000-c':origin},
          'regressions':{'receipt':'unused'},'regression_signature':{}}
    for field,digest,contents in [('before_receipt','before_sha256',prior),('after_plan','after_sha256',{}),('origin_index','origin_index_sha256',world['index'])]:
        path=world['tmp']/f'{field}.json';write(path,contents);data[field]=str(path);data[digest]=env.digest(path)
    monkeypatch.setattr(p,'transition',lambda *_:None)
    monkeypatch.setattr(p,'_regressions',lambda _:(data['regressions'],{}))
    monkeypatch.setattr(p,'_selection',lambda:(['00401000'],{'00401000-c':(world['va'],'c')}))
    monkeypatch.setattr(v,'snapshot',lambda *a,**kw:copy.deepcopy(world['current']))
    return data,prior,after

def test_fresh_scope_loss_raises_for_planning_resume_and_finalization(world,monkeypatch):
    data,prior,after=checked_proof(world,monkeypatch)
    assert '00401000-c' in p.check(data,prior,after)
    monkeypatch.setattr(recover,'c_dependencies',lambda _:{'files':{},'preprocessed':{}})
    with pytest.raises(ValueError,match='scope lost eligibility'):p.check(data,prior,after)

def test_current_whole_selection_drift_invalidates_proof(world,monkeypatch):
    data,prior,after=checked_proof(world,monkeypatch)
    monkeypatch.setattr(p,'_selection',lambda:(['00401000','00401200'],{}))
    with pytest.raises(ValueError,match='whole inventory changed'):p.check(data,prior,after)

def test_retained_report_body_mutation_invalidates_even_before_candidate_recheck(world,monkeypatch):
    data,prior,after=checked_proof(world,monkeypatch)
    world['report']['builds']['-O2']['difftest']['code_as_data_cases']=1;write(world['path'],world['report'])
    with pytest.raises(ValueError,match='report body changed'):p.check(data,prior,after)

def test_lifted_reuse_requires_current_original_derived_render_and_validated_fallbacks(tmp_path,monkeypatch):
    from types import SimpleNamespace
    from cmre import lift,build
    source=tmp_path/'lift.S';source.write_bytes(b'original-derived')
    insn=SimpleNamespace(as_bytes=False,kind=next(iter(build.BYTE_SAFE)),substitutions=[])
    discovered=SimpleNamespace(problems=[],insns={0x401000:insn})
    monkeypatch.setattr(lift,'discover',lambda _:discovered)
    monkeypatch.setattr(build,'_lift_return_sites',lambda _:None)
    monkeypatch.setattr(lift,'render',lambda _:'original-derived')
    report={'lift':{'byte_fallbacks':['00401000']}}
    p._lift_source(0x401000,report,source);assert insn.as_bytes
    source.write_bytes(b'hand-edited')
    with pytest.raises(ValueError,match='render'):p._lift_source(0x401000,report,source)
    insn.as_bytes=False;insn.kind='unknown'
    with pytest.raises(ValueError,match='fallback'):p._lift_source(0x401000,report,source)

@pytest.mark.parametrize('field,value',[('code_as_data_cases',-1),('code_as_data_cases',601),('coverage',float('nan')),('coverage',1.01),('coverage',-1),('outcomes',{'return':-1,'fault':601})])
def test_invalid_numeric_outcomes_never_seed_compatibility(world,field,value):
    world['report']['builds']['-O0']['difftest'][field]=value
    with pytest.raises(ValueError):p.report_pass(world['report'],'c',world['opts'])

def test_lifted_metrics_must_match_independent_accepted_provenance(world,monkeypatch):
    w=world;ex=p.execution(w['before']);opts=w['opts'];source=w['source'];current=copy.deepcopy(w['current'])
    current['profile']=recover.LIFTED_PROFILE;current['optimizations']=['assembler']
    d=copy.deepcopy(w['report']['builds']['-O0']['difftest']);d['profile']=recover.LIFTED_PROFILE;d['code_as_data_cases']=2
    key=recover._key(ex,source.read_bytes().decode(),opts,recover.LIFTED_PROFILE)
    report={'tier':'lifted','status':'pass','key':key,'execution_identity':ex,'harness_sha256':p.OLD_HARNESS,
            'source_sha256':current['source'],'profile':recover.LIFTED_PROFILE,'problems':[],'difftest':d}
    record={'lifted':{'source_sha256':current['source'],'run_key':key,'validation':copy.deepcopy(d)}}
    write(ledger.record_path(w['va']),record)
    root=env.RUNS/'fn_00401000';path=root/key/'report.json';write(path,report)
    (path.parent/source.name).write_bytes(source.read_bytes())
    request=recover._key(ex,w['va'],opts,recover.LIFTED_PROFILE);write(root/f'request-{request}.json',{'key':key})
    monkeypatch.setattr(p,'_lift_source',lambda *_:None)
    prior={'results':{'00401000-lifted':{'ok':True,'dependencies':current}}}
    def actual():return p.candidate(w['va'],'lifted',current,w['before'],[],prior)
    assert actual()[0] # legitimate existing nonzero lifted CAD preserved
    assert actual()[0]['code_as_data_cases']==2
    report['difftest']['code_as_data_cases']=0;write(path,report)
    assert 'lifted metrics disagree' in actual()[1]
