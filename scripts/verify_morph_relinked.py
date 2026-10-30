"""Audit morphology relink acceptance while preserving the prior full-body ledger."""
from __future__ import annotations
import argparse,hashlib,json,platform,struct,sys
from pathlib import Path
import capstone
import dump_rng_slice as pe
import verify_morph_parent as prior
import verify_stack_probe as probe
ROOT=Path(__file__).resolve().parents[1]
LEDGER=ROOT/'recovery/lang/morph-relinked-ledger.json'
read=prior.read
def checked(path,identity):assert pe.digest(path)==identity,str(path)
def main():
    if not __debug__:raise SystemExit('Assertions required')
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    out=args.output.resolve();analysis=ROOT/'CM0102/Analysis';assert out!=analysis and out.is_relative_to(analysis)
    out.mkdir(parents=True,exist_ok=False)
    audit=dict(status='incomplete',command=sys.argv,python=sys.version,platform=platform.platform(),
        capstone=capstone.__version__,script_sha256=pe.digest(Path(__file__)),ledger_sha256=pe.digest(LEDGER))
    try:
        assert pe.digest(pe.TARGET)==pe.digest(pe.FALLBACK)==pe.SHA256
        ledger=read(LEDGER);assert ledger['kind']=='full_application_routine_with_recovered_compiler_support'
        assert ledger['input_sha256']==pe.SHA256
        checked(ROOT/ledger['acceptance_provenance']['script'],ledger['acceptance_provenance']['script_sha256'])
        pin=ledger['prior_full_acceptance'];old_path=ROOT/pin['ledger'];checked(old_path,pin['sha256'])
        checked(ROOT/pin['verifier'],pin['verifier_sha256']);old=read(old_path)
        old_reports=[]
        for run in old['validation_runs']:
            _,r=prior.previous.checked_report(run,ROOT,source_count=14)
            assert run['cases']==323545 and r['cases']==prior.EXPECTED;old_reports.append(r)
        assert len(old_reports)==2 and old_reports[0]['sources']==old_reports[1]['sources']
        prior.checked_generation(old,old_reports[0]);history=prior.checked_partial(old)
        probe_evidence=probe.verify()
        prepared_pin=ledger['relink_provenance'];pp=ROOT/prepared_pin['report'];checked(pp,prepared_pin['sha256'])
        prepared=read(pp);checked(ROOT/prepared['command'][0],prepared['script_sha256'])
        assert prepared['prior_morphology_ledger_sha256']==pin['sha256']
        assert prepared['probe_ledger_sha256']==pe.digest(probe.LEDGER)
        for name,identity in {**prepared['inputs'],**prepared['outputs']}.items():checked(ROOT/name,identity)
        oldasm=(ROOT/'recovery/lang/cm_lang_morph.S').read_text(encoding='utf-8')
        expected=oldasm.replace('original stack probe, immutable data and explicit CRT boundaries remain.',
            'recovered stack probe linked; immutable data and explicit CRT boundaries remain.').replace('call 0x00945610','call _cm_stack_probe')
        assembly=(ROOT/'recovery/lang/cm_lang_morph_relinked.S').read_text(encoding='utf-8')
        assert expected==assembly and assembly.count('call _cm_stack_probe')==1 and 'call 0x'not in assembly
        function,=ledger['functions'];original_function,=old['functions']
        for key,value in original_function.items():
            if key not in ('entry_source','validation_scope'):assert function[key]==value
        assert function['entry_source']=='recovery/lang/cm_lang_morph_relinked.S'
        assert function['validated']and not function['integrated']
        for key in ('byte_accounting','original_regions','original_dispatch_tables','static_exports','static_inspections',
                'recovered_support','recovered_support_b','unchanged_full_ledgers','preserved_partial_acceptance','generation_provenance'):
            assert ledger[key]==old[key]
        dependency=ledger['dependencies']['recovered_stack_probe']
        assert dependency['ledger_sha256']==pe.digest(probe.LEDGER)
        assert dependency['source_sha256']==pe.digest(ROOT/dependency['source'])
        assert dependency['original_instruction_sha256']==read(probe.LEDGER)['functions'][0]['sha256']
        assert 'original_stack_probe'not in ledger['dependencies']
        assert ledger['dependencies']['crt_fixtures']==old['dependencies']['crt_fixtures']
        assert ledger['dependencies']['original_executable_instructions'].startswith('None')
        md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
        data,sections=pe.load_pe(pe.TARGET)
        for region in ledger['original_regions']:
            assert hashlib.sha256(pe.read_va(data,sections,int(region['va'],16),region['size'])).hexdigest()==region['sha256']
        runs=[];links=[]
        for run in ledger['validation_runs']:
            path,report=prior.previous.checked_report(run,ROOT,source_count=15)
            assert run['cases']==323545 and report['cases']==prior.EXPECTED
            assert report['recovered_probe_calls']==323545
            assert report['original_executed_code_scope']=='CRT fixture entry hooks only; original application and stack-probe instructions prohibited'
            assert report['evidence']==old_reports[0]['evidence']
            assert report['tools']==old_reports[0]['tools']
            shared={k:v for k,v in old_reports[0]['sources'].items()if k not in
                ('recovery/lang/cm_lang_morph.S','recovery/lang/test_cm_lang_morph.py')}
            assert all(report['sources'][k]==v for k,v in shared.items())
            assert set(report['sources'])==set(shared)|{
                'recovery/lang/cm_lang_morph_relinked.S','recovery/lang/test_cm_lang_morph_relinked.py','recovery/crt/cm_stack_probe.S'}
            link=read(path.parent/'probe-link.json');assert link==report['linked_probe']
            assert link['ledger_sha256']==pe.digest(probe.LEDGER)
            assert link['source_sha256']==dependency['source_sha256']
            assert link['size']==47 and link['sha256']==read(probe.LEDGER)['compiled']['sha256']
            binary_data,binary_sections=pe.load_pe(path.parent/'cm_lang_morph.exe')
            entry=report['recovered_entry'];target=int(link['target'],16);site=int(link['call_site'],16)
            assert site==entry+5
            call=pe.read_va(binary_data,binary_sections,site-0x1000000+0x400000,5)
            assert call.hex()==link['call_bytes']and call[0]==0xe8 and site+5+struct.unpack('<i',call[1:])[0]==target
            blob=pe.read_va(binary_data,binary_sections,target-0x1000000+0x400000,47)
            assert hashlib.sha256(blob).hexdigest()==link['sha256']
            decoded=list(md.disasm(blob,target));assert len(decoded)==17
            assert [dict(va=f'0x{i.address:08x}',bytes=i.bytes.hex(),instruction=i.mnemonic+' '+i.op_str)for i in decoded]==link['instructions']
            assert probe.normalized(link['instructions'],target)==probe.normalized(read(probe.LEDGER)['original_instructions'],0x945610)
            links.append(link);runs.append(report)
        assert len(runs)==2 and {r['optimization']for r in runs}=={'O0','O2'}
        assert runs[0]['sources']==runs[1]['sources']
        controller_pin=ledger['run_controller'];cp=ROOT/controller_pin['report'];checked(cp,controller_pin['sha256'])
        controller=read(cp)
        assert controller['status']=='pass'and controller['exit_code']==0 and controller['report_status']=='pass'
        checked(ROOT/'CM0102/Analysis/run-morph-relinked-O0-b.py',controller['script_sha256'])
        assert controller['harness_sha256']==runs[0]['sources']['recovery/lang/test_cm_lang_morph_relinked.py']
        assert controller['report_sha256']==ledger['validation_runs'][0]['sha256']
        for name,identity in controller['artifacts'].items():checked(cp.parent/name,identity)
        for name,identity in controller['prior_run']['artifacts'].items():checked(ROOT/controller['prior_run']['path']/name,identity)
        assert controller['prior_run']['status']=='interrupted_without_final_report'
        snapshot=ROOT/ledger['source_snapshot']['path'];mp=snapshot/'manifest.json'
        checked(mp,ledger['source_snapshot']['manifest_sha256']);manifest=read(mp)
        assert manifest['reports']==ledger['validation_runs']and manifest['sources']==runs[0]['sources']
        for name,identity in manifest['sources'].items():checked(snapshot/name,identity)
        entries=[]
        for name,identity in ledger['unchanged_full_ledgers'].items():checked(ROOT/name,identity);entries+=read(ROOT/name)['functions']
        entries+=ledger['functions']+read(probe.LEDGER)['functions']
        assert len(entries)==len({f['va']for f in entries})==47
        assert sum(f.get('category','application')=='application'for f in entries)==45
        assert sum(f.get('category','').startswith('CRT')for f in entries)==2
        assert all(f['validated']and not f['integrated']for f in entries)
        assert pe.digest(pe.TARGET)==pe.digest(pe.FALLBACK)==pe.SHA256
        audit.update(status='pass',input_sha256=pe.SHA256,cases_per_build=323545,frozen_sources=15,
            prior_full_acceptance=pin,preserved_partial_acceptance=history,compiler_probe=probe_evidence,linked_probe=links,
            full_application_entries=45,separate_crt_entries=2,integrated_entries=0,
            original_application_or_probe_instructions_executed=False,crt_fixtures_explicit=True,standalone_build=False)
    except Exception as exc:audit['error']=repr(exc);raise
    finally:(out/'audit.json').write_text(json.dumps(audit,indent=2)+'\n',encoding='utf-8')
    print('PASS relinked morphology:323545/build;15 frozen sources;45 applications+2 CRT;no original application/probe instructions')
if __name__=='__main__':main()
