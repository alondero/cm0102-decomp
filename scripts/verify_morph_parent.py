"""Read-only acceptance audit of the complete morphology application body."""
from __future__ import annotations
import argparse,csv,hashlib,json,platform,re,sys
from pathlib import Path
import capstone
import dump_rng_slice as pe
import verify_morph_partial as previous
ROOT=Path(__file__).resolve().parents[1]
LEDGER=ROOT/'recovery/lang/morph-ledger.json'
EXPECTED=dict(previous.EXPECTED_CASES)
del EXPECTED['explicit_original_residual']
EXPECTED.update(default_neutral_dispatch=8,five_neutral_dispatch=1,
    five_candidate_prefix_properties=264,five_candidate_suffix_properties=2926,
    five_candidate_first_bytes=512,five_candidate_combinations=336,
    five_candidate_aliases=12,five_candidate_return_bits=8,five_candidate_suffix_slot=2,
    five_candidate_properties_after_character=2,five_candidate_language_change=16,
    five_candidate_output_replacement=16,five_candidate_lengths=32,five_candidate_random=1000,
    five_prefix_property_bytes=5880,five_suffix_property_bytes=65170,
    five_independent_properties=9576,five_alias_properties=624,five_alias_suffix_keys=798,
    five_long_word_lengths=8,five_callback_properties=798,default_language_bytes=984,
    default_ignored_qualifiers=9576,default_unmapped_qualifiers=24,
    default_dynamic_language=16,default_nonempty_output=16,default_first_bytes=512,
    default_random_language=256)
assert sum(EXPECTED.values())==323545 and len(EXPECTED)==152
digest=previous.digest
read=previous.read

def inspection(folder):
    r=read(folder/'report.json')
    assert r['input_sha256']==pe.SHA256
    assert digest(ROOT/r['command'][0])==r['script_sha256']
    for name,identity in r['artifacts'].items():assert digest(folder/name)==identity
    return r

def checked_partial(ledger):
    pin=ledger['preserved_partial_acceptance'];path=ROOT/pin['ledger']
    assert digest(path)==pin['sha256'] and digest(ROOT/pin['verifier'])==pin['verifier_sha256']
    old=read(path);assert old['revision']==8 and old['counted_as_full_routine']is False
    archive=ROOT/pin['prior_state'];assert digest(archive/'manifest.json')==pin['archive_manifest_sha256']
    for name,identity in read(archive/'manifest.json')['files'].items():assert digest(archive/name)==identity
    assert digest(archive/pin['ledger'])==pin['sha256']
    snapshot=ROOT/old['source_snapshot']['path']
    assert digest(snapshot/'manifest.json')==old['source_snapshot']['manifest_sha256']
    manifest=read(snapshot/'manifest.json');assert manifest['reports']==old['validation_runs']
    reports=[]
    for run in old['validation_runs']:
        _,r=previous.checked_report(run,snapshot,source_count=13)
        assert r['cases']==previous.EXPECTED_CASES and run['cases']==224181
        assert r['sources']==manifest['sources'];reports.append(r)
    assert reports[0]['sources']==reports[1]['sources']
    previous.checked_nine_adoption(old,reports[0])
    history=previous.checked_history(old);assert len(history)==7
    assert old['unchanged_full_ledgers']==ledger['unchanged_full_ledgers']
    return dict(current_partial=pin,cases_per_build=224181,earlier_stages=history)

def checked_generation(ledger,report):
    pin=ledger['generation_provenance']
    for path_key,hash_key in (('creation_report','creation_sha256'),('prepared_report','prepared_sha256'),
            ('completeness_report','completeness_sha256'),('count_report','count_sha256'),
            ('five_candidate_audit','five_candidate_audit_sha256')):
        assert digest(ROOT/pin[path_key])==pin[hash_key]
    creation_path=ROOT/pin['creation_report'];creation=inspection(creation_path.parent)
    prepared_path=ROOT/pin['prepared_report'];prepared=inspection(prepared_path.parent)
    assert prepared['creation_report_sha256']==pin['creation_sha256']
    for name,identity in creation['semantic_inputs'].items():assert digest(ROOT/name)==identity
    harness='recovery/lang/test_cm_lang_morph.py'
    for name,identity in creation['outputs'].items():
        if name==harness:assert digest(prepared_path.parent/'harness-before.py')==identity
        else:assert digest(ROOT/name)==identity
    for name,identity in prepared['outputs'].items():
        assert digest(ROOT/name)==identity and report['sources'][name]==identity
    before=(prepared_path.parent/'harness-before.py').read_text(encoding='utf-8')
    expected=before.replace("b'ÿAb'","b'\\xffAb'").replace("b'à le club'","b'\\xe0 le club'").replace("b'à Le club'","b'\\xe0 Le club'")
    expected=expected.replace("'morph-parent-completeness-a','morph-parent-created-a'):","'morph-parent-completeness-a','morph-parent-created-a','morph-parent-prepared-b'):")
    assert expected==(ROOT/harness).read_text(encoding='utf-8')
    count=read(ROOT/pin['count_report'])
    assert count['status']=='counted_only' and count['groups']==EXPECTED and count['total']==323545
    assert digest(ROOT/count['command'][0])==count['script_sha256']
    assert count['harness_sha256']==report['sources'][harness]
    full=inspection((ROOT/pin['completeness_report']).parent)
    assert full['region']=={k:ledger['functions'][0][k]for k in ('va','size','sha256')}
    assert full['instruction_records']==20658 and full['manual_instruction_bytes']==14694
    assert full['manual_instruction_records']==3264
    five=read(ROOT/pin['five_candidate_audit'])
    assert five['status']=='pass'and five['accepted_into_current_ledger']is False and five['combined_acceptance']is False
    assert digest(ROOT/five['command'][0])==five['script_sha256']
    snapshot=ROOT/five['source_snapshot']['path']
    assert digest(snapshot/'manifest.json')==five['source_snapshot']['manifest_sha256']
    manifest=read(snapshot/'manifest.json');assert manifest['sources']==five['sources']and manifest['reports']==five['reports']
    assert len(manifest['sources'])==17
    for name,identity in manifest['sources'].items():assert digest(snapshot/name)==identity
    optimizations=set()
    for run in five['reports']:
        path=ROOT/run['report'];assert digest(path)==run['sha256']
        r=read(path);assert r['status']=='pass'and sum(r['cases'].values())==run['cases']==5126
        assert r['sources']==manifest['sources']and r['tools']==five['tools']==report['tools']
        optimizations.add(r['optimization'])
        for name,identity in r['artifacts'].items():assert digest(path.parent/name)==identity
    assert optimizations=={'O0','O2'}
    generated=ROOT/'CM0102/Analysis/morph-five-generated-b';g=inspection(generated)
    assert g['status']=='generated'
    for name,identity in g['semantic_inputs'].items():assert digest(ROOT/name)==identity
    assert digest(generated/'body.inc')==report['sources']['recovery/lang/cm_lang_morph_five_body.inc']
    lowering=read(generated/'lowering.json')
    assert lowering['blocks']==455 and lowering['unmerged_blocks']==1623 and len(lowering['merged_blocks'])==1168
    assert lowering['covered_records']==4765
    assert [(r['va'],r['instruction'])for r in lowering['uncovered']]==[('0x67c2e5','cmp eax, 5'),('0x67c2e8','jne 0x670974')]
    assert len(lowering['tables'])==139
    for va,row in lowering['tables'].items():assert report['evidence']['tables'][f'0x{int(va,16):08x}']==row
    return pin

def main():
    if not __debug__:raise SystemExit('Assertions required')
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();out=args.output.resolve();analysis=ROOT/'CM0102/Analysis'
    assert out!=analysis and out.is_relative_to(analysis);out.mkdir(parents=True,exist_ok=False)
    audit=dict(status='incomplete',command=sys.argv,python=sys.version,platform=platform.platform(),
        capstone=capstone.__version__,script_sha256=digest(Path(__file__)),ledger_sha256=digest(LEDGER))
    try:
        assert digest(pe.TARGET)==digest(pe.FALLBACK)==pe.SHA256
        data,sections=pe.load_pe(pe.TARGET);ledger=read(LEDGER)
        assert ledger['input_sha256']==pe.SHA256 and ledger['kind']=='full_application_routine'
        assert len(ledger['functions'])==1
        function=ledger['functions'][0]
        assert function['va']=='0x006701f0'and function['size']==function['instruction_bytes']==64465
        assert function['instruction_records']==20658 and function['category']=='application'
        assert all(function[k]is True for k in ('identified','decompiled','compiled','validated'))and function['integrated']is False
        assert hashlib.sha256(pe.read_va(data,sections,0x6701F0,64465)).hexdigest()==function['sha256']
        assert digest(ROOT/ledger['acceptance_provenance']['script'])==ledger['acceptance_provenance']['script_sha256']
        runs=[]
        for run in ledger['validation_runs']:
            path,report=previous.checked_report(run,ROOT,source_count=14)
            assert run['cases']==323545 and report['cases']==EXPECTED
            runs.append((path,report))
        assert len(runs)==2 and {r['optimization']for _,r in runs}=={'O0','O2'}
        first=runs[0][1];e=first['evidence']
        assert all(r['sources']==first['sources']and r['evidence']==e for _,r in runs)
        assert ledger['original_regions']==e['regions']and ledger['original_dispatch_tables']==e['tables']
        assert ledger['static_exports']==e['exports']and ledger['static_inspections']==e['inspections']
        assert len(e['regions'])==23 and len(e['tables'])==364 and len(e['inspections'])==25
        regions=sorted(e['regions'],key=lambda r:int(r['va'],16))
        assert int(regions[0]['va'],16)==0x6701F0 and int(regions[-1]['va'],16)+regions[-1]['size']==0x67FDC1
        assert all(int(a['va'],16)+a['size']==int(b['va'],16)for a,b in zip(regions,regions[1:]))
        for r in regions:
            blob=pe.read_va(data,sections,int(r['va'],16),r['size'])
            assert hashlib.sha256(blob).hexdigest()==r['sha256']
        md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
        records=read(runs[0][0].parent/'region-instructions.json')
        assert records==read(runs[1][0].parent/'region-instructions.json')
        assert len(records)==len({r['va']for r in records})==20658
        assert sum(len(bytes.fromhex(r['bytes']))for r in records)==64465
        decoded=list(md.disasm(pe.read_va(data,sections,0x6701F0,64465),0x6701F0))
        assert len(decoded)==20658 and sum(i.size for i in decoded)==64465
        for r,i in zip(sorted(records,key=lambda r:int(r['va'],16)),decoded):
            assert int(r['va'],16)==i.address and r['bytes']==i.bytes.hex()
            assert r['instruction']==i.mnemonic+' '+i.op_str
        exported={}
        with (ROOT/'CM0102/Analysis/lang-loader-static-b/selected-disassembly.tsv').open()as stream:
            for row in csv.DictReader(stream,delimiter='\t'):
                if int(row['function_va'],16)==0x6701F0:exported[int(row['instruction_va'],16)]=row['bytes']
        assert len(exported)==function['ghidra_instruction_records']==17394
        assert sum(len(bytes.fromhex(b))for b in exported.values())==function['ghidra_body_bytes']==49771
        assert set(exported)<={i.address for i in decoded}
        assert all(r['ghidra_exported']==(int(r['va'],16)in exported)for r in records)
        manual=[r for r in records if not r['ghidra_exported']]
        assert manual==e['manual_instructions']and len(manual)==3264
        assert sum(len(bytes.fromhex(r['bytes']))for r in manual)==e['manual_bytes']==14694
        assert ledger['byte_accounting']==dict(ghidra_body_bytes=49771,instruction_bytes=64465,hashed_extent_bytes=64465,
            instruction_records=20658,manual_instruction_bytes=14694,manual_instruction_records=3264,
            new_language_five_instruction_bytes=14863,new_shared_guard_bytes=14)
        for va,row in e['tables'].items():
            blob=pe.read_va(data,sections,int(va,16),row['size'])
            assert blob.hex()==row['bytes']and hashlib.sha256(blob).hexdigest()==row['sha256']
        for name,identity in e['inspections'].items():
            folder=ROOT/name;assert digest(folder/'report.json')==identity;inspection(folder)
        support=[]
        for key,va in (('recovered_support','0x006820a0'),('recovered_support_b','0x00682150')):
            pin=ledger[key];assert pin==e[key]and pin['va']==va and pin['size']==171
            assert digest(ROOT/pin['ledger'])==pin['ledger_sha256']and pin['source']in first['sources']
            helper=next(r for r in read(ROOT/pin['ledger'])['functions']if r['va']==va)
            assert helper['sha256']==pin['sha256']and helper['validated']and not helper['integrated']
            assert hashlib.sha256(pe.read_va(data,sections,int(va,16),171)).hexdigest()==pin['sha256']
            support.append(pin)
        probe=ledger['dependencies']['original_stack_probe']
        assert probe['va']=='0x00945610'and probe['size']==47
        assert hashlib.sha256(pe.read_va(data,sections,0x945610,47)).hexdigest()==probe['sha256']
        assembly=(ROOT/'recovery/lang/cm_lang_morph.S').read_text()
        assert not re.search(r'jmp\s+0x',assembly)and assembly.count('call 0x00945610')==1
        assert ledger['dependencies']['original_application_fallback'].startswith('None;')
        generation=checked_generation(ledger,first);history=checked_partial(ledger)
        snapshot=ROOT/ledger['source_snapshot']['path']
        assert digest(snapshot/'manifest.json')==ledger['source_snapshot']['manifest_sha256']
        manifest=read(snapshot/'manifest.json')
        assert manifest['sources']==first['sources']and manifest['reports']==ledger['validation_runs']
        for name,identity in manifest['sources'].items():assert digest(snapshot/name)==identity
        entries=[];assert len(ledger['unchanged_full_ledgers'])==8
        for name,identity in ledger['unchanged_full_ledgers'].items():
            assert digest(ROOT/name)==identity;entries.extend(read(ROOT/name)['functions'])
        assert len(entries)==45
        entries+=ledger['functions'];assert len(entries)==len({r['va']for r in entries})==46
        assert sum(r.get('category','application')=='application'for r in entries)==45
        assert sum(r.get('category','').startswith('CRT')for r in entries)==1
        assert all(r['validated']and not r['integrated']for r in entries)
        assert digest(pe.TARGET)==digest(pe.FALLBACK)==pe.SHA256
        audit.update(status='pass',input_sha256=pe.SHA256,accepted_runs=ledger['validation_runs'],
            cases_per_build=323545,frozen_sources=14,full_instruction_bytes=64465,instruction_records=20658,
            manual_instruction_bytes=14694,manual_instruction_records=3264,table_regions=364,
            static_inspections=e['inspections'],recovered_support=support,original_probe=probe,
            generation_provenance=generation,preserved_partial_acceptance=history,
            unchanged_full_ledgers=ledger['unchanged_full_ledgers'],full_application_entries=45,
            separate_crt_entries=1,integrated_entries=0,standalone_build=False)
    except Exception as exc:audit['error']=repr(exc);raise
    finally:(out/'audit.json').write_text(json.dumps(audit,indent=2)+'\n',encoding='utf-8')
    print(f'PASS: {out}; whole application body accepted; original probe/data/CRT and integration gates explicit')

if __name__=='__main__':main()
