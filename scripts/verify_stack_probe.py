"""Read-only acceptance audit of recovered register-ABI compiler stack probe."""
from __future__ import annotations
import argparse,hashlib,json,platform,sys
from pathlib import Path
import capstone
import dump_rng_slice as pe
ROOT=Path(__file__).resolve().parents[1]
LEDGER=ROOT/'recovery/crt/stack-probe-ledger.json'
EXPECTED=dict(mapped_stack=2442,random_mapped_stack=512,first_unmapped_fault=30,first_unmapped_return_write=36)
def read(path):return json.loads(path.read_text(encoding='utf-8'))
def checked(path,identity):assert pe.digest(path)==identity,str(path)
def normalized(rows,base):
    result=[]
    for row in rows:
        mnemonic,operand=row['instruction'].split(' ',1)
        if mnemonic in ('jb','jae'):operand=str(int(operand,16)-base)
        result.append((mnemonic,operand))
    return result
def verify():
    assert __debug__ and pe.digest(pe.TARGET)==pe.digest(pe.FALLBACK)==pe.SHA256
    ledger=read(LEDGER)
    assert ledger['input_sha256']==pe.SHA256 and ledger['kind']=='compiler_runtime_support'
    assert ledger['counted_as_application']is False and ledger['adopted_by_morphology']is False
    function,=ledger['functions']
    assert function['va']=='0x00945610' and function['size']==function['instruction_bytes']==47
    assert function['instruction_records']==17 and function['category']=='CRT/compiler'
    assert all(function[k]is True for k in ('identified','decompiled','compiled','validated'))and function['integrated']is False
    checked(ROOT/ledger['acceptance_provenance']['script'],ledger['acceptance_provenance']['script_sha256'])
    run,=ledger['validation_runs'];rp=ROOT/run['report'];checked(rp,run['sha256']);report=read(rp)
    assert report['status']=='pass'and report['input_sha256']==pe.SHA256
    assert report['cases']==EXPECTED and sum(EXPECTED.values())==run['cases']==3020
    assert len(report['sources'])==3 and report['sources']==run['source_hashes']
    assert report['compiled']==ledger['compiled']
    for name,identity in report['sources'].items():checked(ROOT/name,identity)
    for name,identity in report['artifacts'].items():checked(rp.parent/name,identity)
    for name,row in report['tools'].items():checked(Path('C:/ProgramData/mingw64/mingw64/bin')/name,row['sha256'])
    snapshot=ROOT/ledger['source_snapshot']['path'];mp=snapshot/'manifest.json'
    checked(mp,ledger['source_snapshot']['manifest_sha256']);manifest=read(mp)
    assert manifest['report']==run['report'] and manifest['report_sha256']==run['sha256']and manifest['sources']==report['sources']
    for name,identity in manifest['sources'].items():checked(snapshot/name,identity)
    provenance=ledger['generation_provenance'];pp=ROOT/provenance['report'];checked(pp,provenance['sha256']);prepared=read(pp)
    checked(ROOT/prepared['command'][0],prepared['script_sha256'])
    for name,identity in prepared['outputs'].items():checked(ROOT/name,identity)
    checked(ROOT/prepared['input_driver'],prepared['input_driver_sha256'])
    candidate_audit=ROOT/provenance['candidate_audit'];checked(candidate_audit,provenance['candidate_audit_sha256'])
    ca=read(candidate_audit);assert ca['status']=='pass'
    checked(ROOT/ca['report'],ca['report_sha256']);checked(ROOT/'CM0102/Analysis/audit-stack-probe-candidate.py',ca['script_sha256'])
    generation=ROOT/'CM0102/Analysis/stack-probe-native-candidate-a';gr=read(generation/'report.json')
    checked(generation/'report.json',report['generation_sha256'])
    checked(ROOT/'CM0102/Analysis/prepare-stack-probe-candidate.py',gr['script_sha256'])
    for name,identity in gr['artifacts'].items():checked(generation/name,identity)
    checked(ROOT/function['source'],gr['artifacts']['cm_stack_probe.S'])
    before=(ROOT/prepared['input_driver']).read_text(encoding='utf-8')
    expected=before.replace('"""Compare isolated native probe registers, ordered accesses and first CPU faults."""',
        '"""Compare maintained native probe registers, ordered accesses and first CPU faults."""')
    expected=expected.replace("source=candidate/'cm_stack_probe.S';obj=out/'cm_stack_probe.o';binary=out/'cm_stack_probe.exe'",
        "source=Path(__file__).with_name('cm_stack_probe.S');obj=out/'cm_stack_probe.o';binary=out/'cm_stack_probe.exe'\nassert pe.digest(source)==generation['artifacts']['cm_stack_probe.S']")
    assert expected==(ROOT/ledger['harness']).read_text(encoding='utf-8')
    original=read(generation/'original-instructions.json');compiled=read(rp.parent/'compiled-instructions.json')
    assert original==ledger['original_instructions']and compiled==ledger['compiled_instructions']
    assert len(original)==len(compiled)==17
    data,sections=pe.load_pe(pe.TARGET);blob=pe.read_va(data,sections,0x945610,47)
    assert hashlib.sha256(blob).hexdigest()==function['sha256']==gr['original']['sha256']
    assert blob==bytes.fromhex(''.join(r['bytes']for r in original))
    compiled_blob=bytes.fromhex(''.join(r['bytes']for r in compiled))
    assert len(compiled_blob)==47 and hashlib.sha256(compiled_blob).hexdigest()==ledger['compiled']['sha256']
    entry=int(ledger['compiled']['entry'],16);binary_data,binary_sections=pe.load_pe(rp.parent/'cm_stack_probe.exe')
    assert pe.read_va(binary_data,binary_sections,entry-0x1000000+0x400000,47)==compiled_blob
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
    for rows,raw,address in ((original,blob,0x945610),(compiled,compiled_blob,entry)):
        instructions=list(md.disasm(raw,address));assert len(instructions)==17
        assert [dict(va=f'0x{i.address:08x}',bytes=i.bytes.hex(),instruction=i.mnemonic+' '+i.op_str)for i in instructions]==rows
    assert normalized(original,0x945610)==normalized(compiled,entry)
    return dict(input_sha256=pe.SHA256,instruction_bytes=47,instruction_records=17,cases=EXPECTED,frozen_inputs=3,
        report=run,source_snapshot=ledger['source_snapshot'],compiled=ledger['compiled'],category='CRT/compiler',
        counted_as_application=False,live_integrated=False,windows_guard_page_or_seh_validated=False,
        morphology_adoption='Requires separate relink acceptance; this helper ledger does not claim it')
def main():
    if not __debug__:raise SystemExit('Assertions required')
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    out=args.output.resolve();analysis=ROOT/'CM0102/Analysis';assert out!=analysis and out.is_relative_to(analysis)
    out.mkdir(parents=True,exist_ok=False)
    audit=dict(status='incomplete',command=sys.argv,python=sys.version,platform=platform.platform(),
        capstone=capstone.__version__,script_sha256=pe.digest(Path(__file__)),ledger_sha256=pe.digest(LEDGER))
    try:audit.update(verify());audit['status']='pass'
    except Exception as exc:audit['error']=repr(exc);raise
    finally:(out/'audit.json').write_text(json.dumps(audit,indent=2)+'\n',encoding='utf-8')
    print('PASS maintained compiler probe: 47 bytes, 17 instructions, 3020 cases, 3 frozen inputs')
if __name__=='__main__':main()
