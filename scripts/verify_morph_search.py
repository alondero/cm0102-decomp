"""Readonly gates for full morphology/search composition; missing full reports are not acceptance."""
import argparse,capstone,hashlib,json,platform,struct,sys
from pathlib import Path
import dump_rng_slice as pe
import verify_stack_probe as probe
import verify_crt_search as search
import verify_morph_parent as parent
ROOT=Path(__file__).resolve().parents[1]
LEDGER=ROOT/'recovery/lang/morph-search-ledger.json'
PRIOR=ROOT/'recovery/lang/morph-relinked-ledger.json'
def read(p):return json.loads(p.read_text(encoding='utf-8'))
def checked(p,h):assert pe.digest(p)==h,str(p)
def check_link(folder,r):
    data,sections=pe.load_pe(folder/'cm_lang_morph.exe')
    def code(va,size):return pe.read_va(data,sections,va-0x1000000+0x400000,size)
    linked_probe=r['linked_probe'];assert linked_probe==read(folder/'probe-link.json')
    assert linked_probe['ledger_sha256']==pe.digest(probe.LEDGER)
    assert linked_probe['source_sha256']==pe.digest(ROOT/linked_probe['source'])
    entry=r['recovered_entry'];site=int(linked_probe['call_site'],16);target=int(linked_probe['target'],16)
    call=code(site,5);assert site==entry+5 and call.hex()==linked_probe['call_bytes'] and call[0]==0xe8
    assert site+5+struct.unpack('<i',call[1:])[0]==target
    raw=code(target,47);assert hashlib.sha256(raw).hexdigest()==linked_probe['sha256']==read(probe.LEDGER)['compiled']['sha256']
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
    instructions=list(md.disasm(raw,target));assert len(instructions)==17
    assert linked_probe['instructions']==[dict(va=f'0x{i.address:08x}',bytes=i.bytes.hex(),instruction=i.mnemonic+' '+i.op_str)for i in instructions]
    assert probe.normalized(linked_probe['instructions'],target)==probe.normalized(read(probe.LEDGER)['original_instructions'],0x945610)
    link=r['linked_search'];assert link==read(folder/'search-link.json') and link['ledger_sha256']==pe.digest(search.LEDGER)
    for p,h in link['sources'].items():checked(ROOT/p,h)
    original=read(search.LEDGER)['original_instructions'];compiled=link['instructions']
    assert len(original)==len(compiled)==link['instruction_records']==155
    assert (link['original_instruction_bytes'],link['compiled_instruction_bytes'])==(321,318)
    mapping={int(n['va'],16):int(n['original_va'],16)for n in compiled}
    for old,new in zip(original,compiled):
        assert old['va']==new['original_va'];address=int(new['va'],16);raw=code(address,len(bytes.fromhex(new['bytes'])))
        assert raw.hex()==new['bytes'];ins,=list(md.disasm(raw,address));operand=ins.op_str
        assert ins.mnemonic+' '+operand==new['instruction']
        if ins.group(capstone.CS_GRP_JUMP):operand=f'0x{mapping[ins.operands[0].imm]:x}'
        assert ins.mnemonic+' '+operand==old['instruction']
    assert sum(len(bytes.fromhex(n['bytes']))for n in compiled)==318
    assert mapping[int(link['strstr_entry'],16)]==0x945a60 and mapping[int(link['strchr_entry'],16)]==0x946540
def verify_runs():
    assert __debug__ and pe.digest(pe.TARGET)==pe.digest(pe.FALLBACK)==pe.SHA256
    probe.verify();search.verify();prior=read(PRIOR)
    old_sources=prior['validation_runs'][0]['source_hashes']
    for run in prior['validation_runs']:
        rp=ROOT/run['report'];checked(rp,run['sha256']);r=read(rp)
        assert r['status']=='pass' and r['cases']==parent.EXPECTED and r['sources']==old_sources
        for p,h in r['sources'].items():checked(ROOT/p,h)
        for p,h in r['artifacts'].items():checked(rp.parent/p,h)
    old_snapshot=ROOT/prior['source_snapshot']['path'];checked(old_snapshot/'manifest.json',prior['source_snapshot']['manifest_sha256'])
    for p,h in old_sources.items():checked(old_snapshot/p,h)
    ap=ROOT/'CM0102/Analysis/morph-relinked-acceptance-audit-a/audit.json';a=read(ap)
    assert a['status']=='pass' and a['ledger_sha256']==pe.digest(PRIOR)
    checked(ROOT/'scripts/verify_morph_relinked.py',a['script_sha256'])
    prepared_path=ROOT/'CM0102/Analysis/morph-search-relink-prepared-a/report.json';prepared=read(prepared_path)
    checked(ROOT/prepared['command'][0],prepared['script_sha256']);assert prepared['prior_acceptance_audit_sha256']==pe.digest(ap)
    for p,h in {**prepared['inputs'],**prepared['outputs']}.items():checked(ROOT/p,h)
    expected=(ROOT/'recovery/lang/cm_lang_morph.c').read_text(encoding='utf-8').replace('#include "cm_lang_morph_tables.h"',
        '#include "cm_lang_morph_tables.h"\n#include "../crt/cm_crt_search.h"').replace('#define FIND ((find_fn)0x00945a60u)','#define FIND cm_crt_strstr')
    assert expected==(ROOT/'recovery/lang/cm_lang_morph_search.c').read_text(encoding='utf-8')
    runs=[];reports=[];controls=[]
    for opt in ('O0','O2'):
        rp=ROOT/f'CM0102/Analysis/morph-search-final-a-{opt}/report.json';r=read(rp)
        assert r['status']=='pass' and r['optimization']==opt and r['input_sha256']==pe.SHA256
        assert r['quick'] is False and r['focused_search'] is False and r['cases']==parent.EXPECTED
        assert r['recovered_probe_calls']==323545 and len(r['sources'])==17
        assert r['original_executed_code_scope']=='CRT fixture entry hooks only; original application, stack-probe and string-search instructions prohibited'
        for p,h in r['sources'].items():checked(ROOT/p,h)
        for p,h in r['artifacts'].items():checked(rp.parent/p,h)
        for name,tool in r['tools'].items():checked(Path(r['build_commands'][0][0]).parent/name,tool['sha256'])
        shared=set(old_sources)&set(r['sources']);assert len(shared)==13
        assert all(r['sources'][p]==old_sources[p]for p in shared)
        assert r['evidence']['regions']==prior['original_regions'] and r['evidence']['tables']==prior['original_dispatch_tables']
        check_link(rp.parent,r)
        assert set(r['search_metrics'])==set(r['cases'])
        for group,row in r['search_metrics'].items():
            assert row['cases']==r['cases'][group] and 0<=row['cases_executing_search']<=row['cases']
            assert row['compiled_body_starts']==row['compiled_search_calls']>=row['cases_executing_search']
            if group!='two_find_return_and_property_mutation':assert row['forced_search_fixture_calls']==0
        forced=r['search_metrics']['two_find_return_and_property_mutation']
        assert forced==dict(cases=6,cases_executing_search=0,compiled_search_calls=0,compiled_body_starts=0,forced_search_fixture_calls=32)
        assert sum(row['compiled_search_calls']for row in r['search_metrics'].values())>=5883
        cp=ROOT/f'CM0102/Analysis/morph-search-{opt}-control-a/run.json';control=read(cp)
        assert control['status']=='pass' and control['exit_code']==0 and control['report_status']=='pass'
        assert control['report_sources_match'] and control['report_full_scope'] and control['current_sources_match'] and control['cases']==323545
        checked(ROOT/'CM0102/Analysis/run-morph-search.py',control['script_sha256'])
        assert control['sources']==r['sources'] and control['harness_sha256']==r['sources']['recovery/lang/test_cm_lang_morph_search.py']
        assert control['report_sha256']==pe.digest(rp)
        for p,h in control['artifacts'].items():checked(cp.parent/p,h)
        reports.append(r);runs.append(dict(report=rp.relative_to(ROOT).as_posix(),sha256=pe.digest(rp),optimization=opt,cases=323545,source_hashes=r['sources']))
        controls.append(dict(report=cp.relative_to(ROOT).as_posix(),sha256=pe.digest(cp)))
    assert reports[0]['sources']==reports[1]['sources'] and reports[0]['search_metrics']==reports[1]['search_metrics'] and reports[0]['evidence']==reports[1]['evidence']
    return dict(prior=prior,prior_ledger_sha256=pe.digest(PRIOR),reports=runs,controllers=controls,sources=reports[0]['sources'],
        metrics=reports[0]['search_metrics'],prepared_report_sha256=pe.digest(prepared_path))
def verify():
    e=verify_runs();ledger=read(LEDGER)
    assert ledger['kind']=='full_application_with_recovered_probe_and_search'
    assert ledger['input_sha256']==pe.SHA256 and ledger['validation_runs']==e['reports'] and ledger['search_metrics']==e['metrics']
    checked(ROOT/ledger['acceptance_provenance']['script'],ledger['acceptance_provenance']['script_sha256'])
    assert ledger['prior_probe_relink']['sha256']==e['prior_ledger_sha256'] and ledger['search_relink_provenance']['sha256']==e['prepared_report_sha256']
    assert ledger['search_run_controllers']==e['controllers']
    for key in ('byte_accounting','original_regions','original_dispatch_tables','static_exports','static_inspections','recovered_support','recovered_support_b',
                'unchanged_full_ledgers','preserved_partial_acceptance','generation_provenance'):
        assert ledger[key]==e['prior'][key]
    expected_function=dict(e['prior']['functions'][0]);expected_function['source']='recovery/lang/cm_lang_morph_search.c'
    expected_function['validation_scope']+='; compiled search on unforced native calls, three steady CRT fixtures and six separately counted artificial find-boundary cases; reference find fixture remains explicit'
    assert ledger['functions']==[expected_function] and not expected_function['integrated']
    assert ledger['dependencies']['recovered_stack_probe']==e['prior']['dependencies']['recovered_stack_probe']
    expected_fixtures=dict(e['prior']['dependencies']['crt_fixtures']);del expected_fixtures['0x00945a60']
    assert ledger['dependencies']['crt_fixtures']==expected_fixtures
    assert ledger['dependencies']['artificial_search_fixture_cases']==dict(group='two_find_return_and_property_mutation',cases=6,calls=32)
    assert ledger['dependencies']['reference_search']=='Existing find boundary fixture retained on reference side; independent maintained search acceptance verifies original instruction/access/fault scope'
    assert ledger['dependencies']['recovered_string_search']['ledger_sha256']==pe.digest(search.LEDGER)
    limitations=read(ROOT/e['reports'][0]['report'])['limitations']+[
        'Accepted compiler probe CPU evidence does not validate Windows guard-page growth or SEH',
        'Same application VA as prior morphology ledgers; do not count it twice',
        'No executable patch, M2 replay, M4 integration or M6 standalone acceptance',
        'Six artificial forced-find cases intercept compiled entry and are not compiled search executions',
        'Reference side retains prior find fixture; separate search acceptance supplies original instruction/access/fault evidence',
        'Search composition adds no application entry; parent original VA counted once']
    assert ledger['limitations']==limitations
    snapshot=ROOT/ledger['source_snapshot']['path'];checked(snapshot/'manifest.json',ledger['source_snapshot']['manifest_sha256'])
    manifest=read(snapshot/'manifest.json');assert manifest['sources']==e['sources'] and manifest['reports']==e['reports']
    for p,h in manifest['sources'].items():checked(snapshot/p,h)
    return dict(input_sha256=pe.SHA256,cases_per_build=323545,frozen_sources=17,compiled_search_calls=sum(r['compiled_search_calls']for r in e['metrics'].values()),
        cases_executing_search=sum(r['cases_executing_search']for r in e['metrics'].values()),forced_search_fixture_cases=6,forced_search_fixture_calls=32,
        native_original_application_probe_or_search_instructions=False,steady_crt_fixtures=3,integrated=False,standalone_build=False)
def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    out=args.output.resolve();analysis=ROOT/'CM0102/Analysis';assert out!=analysis and out.is_relative_to(analysis);out.mkdir(exist_ok=False)
    audit=dict(status='incomplete',command=sys.argv,python=sys.version,platform=platform.platform(),script_sha256=pe.digest(Path(__file__)),ledger_sha256=pe.digest(LEDGER))
    try:audit.update(verify());audit['status']='pass'
    except Exception as exc:audit['error']=repr(exc);raise
    finally:(out/'audit.json').write_text(json.dumps(audit,indent=2)+'\n',encoding='utf-8')
    print('PASS full search composition:323545/build17frozen;three CRT fixtures plus six artificial boundary cases')
if __name__=='__main__':main()
