"""Read-only audit of dependency-free native substring/character search support."""
from __future__ import annotations
import argparse,hashlib,json,platform,sys
from pathlib import Path
import capstone
import dump_rng_slice as pe
ROOT=Path(__file__).resolve().parents[1]
LEDGER=ROOT/'recovery/crt/search-ledger.json'
EXPECTED=dict(character_bytes_alignment_length=33792,substring_prefix_overlap=8320,needle_haystack_alias=284,
    incoming_flags=8,empty_needle_guard=3,first_unmapped_fault=16,random_bytes=2000)
def read(p):return json.loads(p.read_text(encoding='utf-8'))
def checked(p,h):assert pe.digest(p)==h,str(p)
def verify_run(rp):
    assert __debug__ and pe.digest(pe.TARGET)==pe.digest(pe.FALLBACK)==pe.SHA256
    report=read(rp);assert report['status']=='pass'and report['input_sha256']==pe.SHA256
    assert report['cases']==EXPECTED and sum(EXPECTED.values())==44423
    assert len(report['sources'])==4
    for name,h in report['sources'].items():checked(ROOT/name,h)
    for name,h in report['artifacts'].items():checked(rp.parent/name,h)
    for name,row in report['tools'].items():checked(Path('C:/ProgramData/mingw64/mingw64/bin')/name,row['sha256'])
    candidate=ROOT/'CM0102/Analysis/crt-search-native-candidate-a';gp=candidate/'report.json'
    checked(gp,report['generation_sha256']);g=read(gp)
    assert g['input_sha256']==pe.SHA256 and g['unique_instruction_bytes']==321 and g['instruction_records']==155
    checked(ROOT/'CM0102/Analysis/prepare-crt-search-candidate.py',g['script_sha256'])
    for name,h in g['artifacts'].items():checked(candidate/name,h)
    checked(ROOT/'recovery/crt/cm_crt_search.S',g['artifacts']['cm_crt_search.S'])
    for folder,h in g['exports'].items():
        checked(ROOT/folder/'run.json',h);export=read(ROOT/folder/'run.json')
        assert export['success']and export['input_sha256']==pe.SHA256
        for name,identity in export['outputs'].items():checked(ROOT/folder/name,identity)
    prepared_path=ROOT/'CM0102/Analysis/crt-search-maintained-prepared-a/report.json';prepared=read(prepared_path)
    checked(ROOT/prepared['command'][0],prepared['script_sha256'])
    checked(ROOT/prepared['input_driver'],prepared['input_driver_sha256'])
    for name,h in prepared['outputs'].items():checked(ROOT/name,h)
    checked(ROOT/'CM0102/Analysis/crt-search-candidate-audit-a/audit.json',prepared['candidate_audit_sha256'])
    before=(ROOT/prepared['input_driver']).read_text(encoding='utf-8')
    expected=before.replace('"""Compare native search closure registers, flags, ordered accesses and CPU faults."""',
        '"""Compare maintained search closure registers, flags, ordered accesses and CPU faults."""')
    expected=expected.replace("source=candidate/'cm_crt_search.S'",
        "source=Path(__file__).with_name('cm_crt_search.S')\nassert pe.digest(source)==generation['artifacts']['cm_crt_search.S']")
    expected=expected.replace("for p in(source,Path(__file__),ROOT/'scripts/dump_rng_slice.py')",
        "for p in(source,Path(__file__).with_name('cm_crt_search.h'),Path(__file__),ROOT/'scripts/dump_rng_slice.py')")
    assert expected==(ROOT/'recovery/crt/test_cm_crt_search.py').read_text(encoding='utf-8')
    original=read(candidate/'original-instructions.json');compiled=read(rp.parent/'compiled-instructions.json')
    assert len(original)==len(compiled)==report['compiled_instruction_records']==155
    assert sum(len(bytes.fromhex(r['bytes']))for r in original)==321
    assert sum(len(bytes.fromhex(r['bytes']))for r in compiled)==report['compiled_instruction_bytes']==318
    data,sections=pe.load_pe(pe.TARGET)
    for region in g['regions']:
        blob=pe.read_va(data,sections,int(region['va'],16),region['size'])
        assert hashlib.sha256(blob).hexdigest()==region['sha256']
    binary_data,binary_sections=pe.load_pe(rp.parent/'cm_crt_search.exe')
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
    mapping={int(row['va'],16):int(row['original_va'],16)for row in compiled}
    for old,new in zip(original,compiled):
        assert old['va']==new['original_va']
        address=int(new['va'],16);blob=pe.read_va(binary_data,binary_sections,address-0x1000000+0x400000,len(bytes.fromhex(new['bytes'])))
        assert blob.hex()==new['bytes']
        ins,=list(md.disasm(blob,address));operand=ins.op_str
        assert ins.mnemonic+' '+operand==new['instruction']
        if ins.group(capstone.CS_GRP_JUMP):operand=f'0x{mapping[ins.operands[0].imm]:x}'
        assert ins.mnemonic+' '+operand==old['instruction']
        assert pe.read_va(data,sections,int(old['va'],16),len(bytes.fromhex(old['bytes']))).hex()==old['bytes']
    return report,g,original,compiled
def verify():
    ledger=read(LEDGER);assert ledger['input_sha256']==pe.SHA256 and ledger['kind']=='dependency_free_crt_search_closure'
    checked(ROOT/ledger['acceptance_provenance']['script'],ledger['acceptance_provenance']['script_sha256'])
    run,=ledger['validation_runs'];rp=ROOT/run['report'];checked(rp,run['sha256'])
    report,g,original,compiled=verify_run(rp)
    assert run['cases']==44423 and run['source_hashes']==report['sources']
    assert ledger['original_regions']==g['regions']and ledger['original_instructions']==original
    assert ledger['compiled_instructions']==compiled
    assert ledger['byte_accounting']==dict(unique_original_instruction_bytes=321,original_instruction_records=155,
        entry_hashed_bytes=316,recovered_shared_exit_bytes=5,compiled_instruction_bytes=318)
    data,sections=pe.load_pe(pe.TARGET)
    assert [(f['va'],f['size'])for f in ledger['functions']]==[('0x00945a60',128),('0x00946540',188)]
    for function in ledger['functions']:
        assert function['category']=='CRT/string'
        assert all(function[k]is True for k in ('identified','decompiled','compiled','validated'))and function['integrated']is False
        blob=pe.read_va(data,sections,int(function['va'],16),function['size'])
        assert hashlib.sha256(blob).hexdigest()==function['sha256']
    assert ledger['counted_as_application']is False and ledger['dependencies']['external_code_or_data']=='None'
    snapshot=ROOT/ledger['source_snapshot']['path'];manifest_path=snapshot/'manifest.json'
    checked(manifest_path,ledger['source_snapshot']['manifest_sha256']);manifest=read(manifest_path)
    assert manifest['sources']==report['sources']and manifest['report_sha256']==run['sha256']
    for name,h in manifest['sources'].items():checked(snapshot/name,h)
    return dict(input_sha256=pe.SHA256,cases=EXPECTED,frozen_inputs=4,
        original_instruction_bytes=321,original_instruction_records=155,compiled_instruction_bytes=318,
        recovered_entry_points=2,recovered_shared_exit_bytes=5,external_dependencies=False,
        original_executable_required_on_native_side=False,counted_as_application=False,integrated=False,
        morphology_adoption='Requires separate relink; current morphology substring entry remains a CRT fixture')
def main():
    if not __debug__:raise SystemExit('Assertions required')
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    out=args.output.resolve();analysis=ROOT/'CM0102/Analysis';assert out!=analysis and out.is_relative_to(analysis);out.mkdir(exist_ok=False)
    audit=dict(status='incomplete',command=sys.argv,python=sys.version,platform=platform.platform(),
        capstone=capstone.__version__,script_sha256=pe.digest(Path(__file__)),ledger_sha256=pe.digest(LEDGER))
    try:audit.update(verify());audit['status']='pass'
    except Exception as exc:audit['error']=repr(exc);raise
    finally:(out/'audit.json').write_text(json.dumps(audit,indent=2)+'\n',encoding='utf-8')
    print('PASS maintained CRT search:321 original bytes/155 semantics;44423 cases;4 frozen inputs;no original code/data dependencies')
if __name__=='__main__':main()
