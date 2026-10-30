"""Readonly maintained classification acceptance and normal-return provenance checks."""
import argparse,capstone,csv,hashlib,json,platform,sys
from pathlib import Path
import dump_rng_slice as pe
ROOT=Path(__file__).resolve().parents[1]
LEDGER=ROOT/'recovery/crt/classify-ledger.json'
EXPECTED=dict(unsigned_table_fast_path=9252,callback_partial_output_mutation=1536,random_words=1000)
def read(p):return json.loads(p.read_text(encoding='utf-8'))
def checked(p,h):assert pe.digest(p)==h,str(p)
def verify_runs():
    assert __debug__ and pe.digest(pe.TARGET)==pe.digest(pe.FALLBACK)==pe.SHA256
    prepared_path=ROOT/'CM0102/Analysis/crt-classify-maintained-prepared-a/report.json';prepared=read(prepared_path)
    checked(ROOT/'CM0102/Analysis/prepare-crt-classify-maintained.py',prepared['script_sha256'])
    for p,h in prepared['outputs'].items():checked(ROOT/p,h)
    checked(ROOT/prepared['input_driver'],prepared['input_driver_sha256'])
    ap=ROOT/'CM0102/Analysis/crt-classify-candidate-audit-a/audit.json';checked(ap,prepared['candidate_audit_sha256'])
    candidate_audit=read(ap);assert candidate_audit['status']=='pass' and candidate_audit['accepted_into_current_ledger'] is False
    checked(ROOT/'CM0102/Analysis/audit-crt-classify-candidate.py',candidate_audit['script_sha256'])
    for p,h in candidate_audit['sources'].items():checked(ROOT/p,h)
    snapshot=ROOT/'CM0102/Analysis/crt-classify-candidate-source-used'
    checked(snapshot/'manifest.json',candidate_audit['snapshot_manifest_sha256'])
    for p,h in candidate_audit['sources'].items():checked(snapshot/p,h)
    candidate=ROOT/'CM0102/Analysis/crt-classify-native-candidate-a';g=read(candidate/'report.json')
    checked(ROOT/'CM0102/Analysis/prepare-crt-classify-candidate.py',g['script_sha256'])
    for p,h in g['artifacts'].items():checked(candidate/p,h)
    for name in ('cm_crt_classify.c','cm_crt_classify.S'):
        assert (ROOT/'recovery/crt'/name).read_bytes()==(candidate/name).read_bytes()
    old=(ROOT/prepared['input_driver']).read_text(encoding='utf-8')
    expected=old.replace('Compare classification frame/table semantics with explicit text-classifier fixtures.',
        'Compare maintained classification frame/table semantics with explicit text-classifier fixtures.')
    expected=expected.replace("sources=[candidate/'cm_crt_classify.c',candidate/'cm_crt_classify.S',Path(__file__),Path(date.__file__),ROOT/'scripts/dump_rng_slice.py']",
        "sources=[Path(__file__).with_name('cm_crt_classify.c'),Path(__file__).with_name('cm_crt_classify.S'),Path(__file__),Path(date.__file__),ROOT/'scripts/dump_rng_slice.py',Path(__file__).with_name('cm_crt_classify.h')]\nassert all(pe.digest(p)==generation['artifacts'][p.name]for p in sources[:2])")
    assert expected==(ROOT/'recovery/crt/test_cm_crt_classify.py').read_text(encoding='utf-8')
    data,sections=pe.load_pe(pe.TARGET);blob=pe.read_va(data,sections,0x94c322,117)
    assert hashlib.sha256(blob).hexdigest()==g['original']['sha256']
    decoded=list(capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(blob,0x94c322))
    original=read(candidate/'original-instructions.json')
    assert len(decoded)==len(original)==46 and sum(i.size for i in decoded)==117
    assert original==[dict(va=f'0x{i.address:08x}',bytes=i.bytes.hex(),instruction=i.mnemonic+' '+i.op_str)for i in decoded]
    export=ROOT/'CM0102/Analysis/morph-crt-dependencies-a';e=read(export/'run.json')
    checked(export/'run.json',g['export_report_sha256']);assert e['success'] and e['input_sha256']==pe.SHA256
    for p,h in e['outputs'].items():checked(export/p,h)
    with (export/'selected-disassembly.tsv').open(encoding='utf-8')as stream:
        rows=[r for r in csv.DictReader(stream,delimiter='\t')if int(r['function_va'],16)==0x94c322]
    assert [(int(r['instruction_va'],16),r['bytes'].lower())for r in rows]==[(i.address,i.bytes.hex())for i in decoded]
    reports=[];sources=None
    for opt in ('O0','O2'):
        rp=ROOT/f'CM0102/Analysis/crt-classify-maintained-final-a-{opt}/report.json';r=read(rp)
        assert r['status']=='pass' and r['optimization']==opt and r['input_sha256']==pe.SHA256 and r['cases']==EXPECTED
        assert len(r['sources'])==6 and r['generation_sha256']==pe.digest(candidate/'report.json')
        for p,h in r['sources'].items():checked(ROOT/p,h)
        for p,h in r['artifacts'].items():checked(rp.parent/p,h)
        for name,tool in r['tools'].items():checked(Path(r['build_commands'][0][0]).parent/name,tool['sha256'])
        if sources is None:sources=r['sources']
        else:assert sources==r['sources']
        reports.append(dict(report=rp.relative_to(ROOT).as_posix(),sha256=pe.digest(rp),optimization=opt,cases=11788,source_hashes=r['sources']))
    return dict(original=g['original'],original_instructions=original,reports=reports,sources=sources,
                prepared_report_sha256=pe.digest(prepared_path),candidate_audit_sha256=pe.digest(ap))
def verify():
    evidence=verify_runs();ledger=read(LEDGER)
    assert ledger['kind']=='frame_abi_crt_classification_with_explicit_dependency' and ledger['input_sha256']==pe.SHA256
    checked(ROOT/ledger['acceptance_provenance']['script'],ledger['acceptance_provenance']['script_sha256'])
    assert ledger['validation_runs']==evidence['reports'] and ledger['original_instructions']==evidence['original_instructions']
    assert ledger['prepared_report_sha256']==evidence['prepared_report_sha256'] and ledger['candidate_audit_sha256']==evidence['candidate_audit_sha256']
    function,=ledger['functions']
    assert (function['va'],function['size'],function['sha256'],function['category'])==('0x0094c322',117,evidence['original']['sha256'],'CRT/locale')
    assert all(function[k] is True for k in ('identified','decompiled','compiled','validated')) and function['integrated'] is False
    assert ledger['counted_as_application'] is False and ledger['adopted_by_case_conversion'] is False
    assert ledger['dependencies']['original_code_boundary_fixture']==['0x0094ed85']
    assert ledger['dependencies']['mutable_table_pointer_va']=='0x00ad8fd0'
    snapshot=ROOT/ledger['source_snapshot']['path'];checked(snapshot/'manifest.json',ledger['source_snapshot']['manifest_sha256'])
    manifest=read(snapshot/'manifest.json');assert manifest['sources']==evidence['sources'] and manifest['reports']==evidence['reports']
    for p,h in manifest['sources'].items():checked(snapshot/p,h)
    return dict(input_sha256=pe.SHA256,cases_per_build=11788,builds=['O0','O2'],frozen_inputs=6,
        original_instruction_bytes=117,original_instruction_records=46,recovered_entry_points=1,category='CRT/locale',
        counted_as_application=False,case_conversion_adoption=False,original_code_boundary_fixture=['0x0094ed85'],
        mutable_table_pointer_va='0x00ad8fd0',live_integrated=False,standalone_build=False)
def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    out=args.output.resolve();analysis=ROOT/'CM0102/Analysis';assert out!=analysis and out.is_relative_to(analysis);out.mkdir(exist_ok=False)
    audit=dict(status='incomplete',command=sys.argv,python=sys.version,platform=platform.platform(),capstone=capstone.__version__,
               script_sha256=pe.digest(Path(__file__)),ledger_sha256=pe.digest(LEDGER))
    try:audit.update(verify());audit['status']='pass'
    except Exception as exc:audit['error']=repr(exc);raise
    finally:(out/'audit.json').write_text(json.dumps(audit,indent=2)+'\n',encoding='utf-8')
    print('PASS maintained classification:11788/build,6 frozen inputs,117 bytes/46 instructions;one explicit deeper fixture')
if __name__=='__main__':main()
