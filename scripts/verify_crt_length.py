"""Read-only audit of bounded-length candidate or maintained CRT acceptance."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import sys
from pathlib import Path

import capstone
import dump_rng_slice as pe

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = ROOT / 'CM0102/Analysis/crt-length-native-candidate-a'
EXPECTED = dict(bytes_limits_alignment=46080, incoming_flags=24,
                zero_limit_read=4, unmapped_zero_limit=3, boundary_unmapped_reads=15,
                boundary_terminated=25, stack_argument_alias=25, random_bytes_limits=2000)


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def checked(path, expected):
    if pe.digest(path) != expected:
        raise ValueError(f'Hash differs: {path}')


def check_test(path, sources):
    report = read(path)
    assert report['status'] == 'pass' and report['input_sha256'] == pe.SHA256
    assert report['cases'] == EXPECTED and sum(EXPECTED.values()) == 48176
    assert report['generation_sha256'] == pe.digest(CANDIDATE / 'report.json')
    assert report['sources'] == sources
    assert report['script_sha256'] == sources[Path(report['command'][0]).as_posix()]
    assert report['compiled_instruction_records'] == 20 and report['compiled_instruction_bytes'] == 43
    for name, digest in sources.items():
        checked(ROOT / name, digest)
    for name, digest in report['artifacts'].items():
        checked(path.parent / name, digest)
    toolchain = Path(report['build_commands'][0][0]).parent
    for name, tool in report['tools'].items():
        checked(toolchain / name, tool['sha256'])
    rows = read(CANDIDATE / 'original-instructions.json')
    compiled = read(path.parent / 'compiled-instructions.json')
    data, sections = pe.load_pe(path.parent / 'cm_crt_length.exe')
    address_map = {int(r['va'], 16): int(r['original_va'], 16) for r in compiled}
    assert len(compiled) == len(address_map) == 20
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    for original, native in zip(rows, compiled):
        address = int(native['va'], 16)
        ins = next(md.disasm(pe.read_va(data, sections, address - 0x1000000 + 0x400000, 15), address, count=1))
        assert native['original_va'] == original['va'] and native['bytes'] == ins.bytes.hex()
        operand = ins.op_str
        if ins.group(capstone.CS_GRP_JUMP):
            operand = f'0x{address_map[ins.operands[0].imm]:x}'
        assert ins.mnemonic + ' ' + operand == original['instruction']
    assert report['entries'] == dict(length=int(compiled[0]['va'], 16))
    return report


def verify_candidate():
    assert __debug__ and pe.digest(pe.TARGET) == pe.digest(pe.FALLBACK) == pe.SHA256
    generation = read(CANDIDATE / 'report.json')
    assert generation['status'] == 'native_candidate_unaccepted' and generation['input_sha256'] == pe.SHA256
    checked(ROOT / 'CM0102/Analysis/prepare-crt-length-candidate.py', generation['script_sha256'])
    for name, digest in generation['artifacts'].items():
        checked(CANDIDATE / name, digest)
    export = ROOT / 'CM0102/Analysis/crt-locale-deep-static-a'
    checked(export / 'run.json', generation['export_report_sha256'])
    exported = read(export / 'run.json')
    assert exported['success'] and exported['input_sha256'] == pe.SHA256
    for name, digest in exported['outputs'].items():
        checked(export / name, digest)
    for name, digest in exported['scripts'].items():
        checked(ROOT / name, digest)
    data, sections = pe.load_pe(pe.TARGET)
    blob = pe.read_va(data, sections, 0x9519d3, 43)
    assert generation['regions'] == [dict(va='0x009519d3', size=43, sha256=hashlib.sha256(blob).hexdigest())]
    decoded = list(capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32).disasm(blob, 0x9519d3))
    rows = read(CANDIDATE / 'original-instructions.json')
    assert len(rows) == len(decoded) == 20 and sum(i.size for i in decoded) == 43
    assert rows == [dict(va=f'0x{i.address:08x}', bytes=i.bytes.hex(), instruction=i.mnemonic + ' ' + i.op_str,
                         label=f'cm_crt_length_i_{i.address:08x}') for i in decoded]
    with (export / 'selected-disassembly.tsv').open(encoding='utf-8') as stream:
        selected = [r for r in csv.DictReader(stream, delimiter='\t') if int(r['function_va'], 16) == 0x9519d3]
    assert [(int(r['instruction_va'], 16), r['bytes'].lower()) for r in selected] == [(i.address, i.bytes.hex()) for i in decoded]
    prepared = read(ROOT / 'CM0102/Analysis/crt-length-driver-prepared-a/report.json')
    checked(ROOT / 'CM0102/Analysis/prepare-crt-length-driver.py', prepared['script_sha256'])
    checked(ROOT / prepared['input'], prepared['input_sha256'])
    checked(ROOT / prepared['output'], prepared['output_sha256'])
    audit_path = ROOT / 'CM0102/Analysis/crt-length-candidate-audit-a/audit.json'
    audit = read(audit_path)
    assert audit['status'] == 'pass' and audit['accepted_into_current_ledger'] is False
    checked(ROOT / 'CM0102/Analysis/audit-crt-length-candidate.py', audit['script_sha256'])
    report_path = ROOT / 'CM0102/Analysis/crt-length-candidate-test-a/report.json'
    checked(report_path, audit['report_sha256'])
    report = check_test(report_path, audit['sources'])
    assert report['accepted_into_current_ledger'] is False and report['adopted_by_morphology'] is False
    snapshot = ROOT / 'CM0102/Analysis/crt-length-candidate-source-used'
    checked(snapshot / 'manifest.json', audit['snapshot_manifest_sha256'])
    for name, digest in audit['sources'].items():
        checked(snapshot / name, digest)
    finish = read(ROOT / 'CM0102/Analysis/crt-length-candidate-finish-a/report.json')
    assert finish['status'] == 'pass' and finish['accepted_into_current_ledger'] is False
    checked(audit_path, finish['audit_sha256'])
    checked(report_path, finish['test_report_sha256'])
    for name, digest in finish['syntax_sources'].items():
        checked(ROOT / name, digest)
    review = read(ROOT / 'CM0102/Analysis/crt-length-candidate-finish-a/review.json')
    assert review['status'] == 'clear' and review['standards']['remaining_findings'] == review['spec']['material_findings'] == 0
    metadata = read(ROOT / 'CM0102/Analysis/crt-length-candidate-finish-a/tool-metadata.json')
    checked(Path(capstone.__file__), metadata['capstone_python_module_sha256'])
    assert metadata['capstone_version'] == report['capstone'] == capstone.__version__
    return generation, rows


def maintained_driver_text():
    """Only source location, source pins and description differ from the reviewed observer."""
    old = (ROOT / 'CM0102/Analysis/test-crt-length-candidate.py').read_text(encoding='utf-8')
    return old.replace('Compare bounded-length registers', 'Compare maintained bounded-length registers').replace(
        "toolchain=Path('C:/ProgramData/mingw64/mingw64/bin');source=candidate/'cm_crt_length.S'",
        "toolchain=Path('C:/ProgramData/mingw64/mingw64/bin');source=Path(__file__).with_name('cm_crt_length.S')\nassert pe.digest(source)==generation['artifacts']['cm_crt_length.S']").replace(
        "(source,Path(__file__),ROOT/'scripts/dump_rng_slice.py')",
        "(source,Path(__file__),ROOT/'scripts/dump_rng_slice.py',Path(__file__).with_name('cm_crt_length.h'))").replace(
        'PASS bounded-length candidate', 'PASS maintained bounded-length support')


def verify_maintained(generation, rows):
    ledger = read(ROOT / 'recovery/crt/length-ledger.json')
    assert ledger['input_sha256'] == pe.SHA256 and ledger['counted_as_application'] is False
    entry, = ledger['functions']
    assert entry['va'] == '0x009519d3' and entry['size'] == 43 and entry['sha256'] == generation['regions'][0]['sha256']
    assert {k: entry[k] for k in ('identified', 'decompiled', 'compiled', 'validated', 'integrated')} == dict(
        identified=True, decompiled=True, compiled=True, validated=True, integrated=False)
    assert ledger['original_instructions'] == rows and ledger['dependencies']['external_code_or_data'] == []
    assert (ROOT / entry['source']).read_bytes() == (CANDIDATE / 'cm_crt_length.S').read_bytes()
    assert (ROOT / ledger['harness']).read_text(encoding='utf-8') == maintained_driver_text()
    prepared_a = read(ROOT / 'CM0102/Analysis/crt-length-maintained-prepared-a/report.json')
    first_snapshot = ROOT / 'CM0102/Analysis/crt-length-maintained-first-source-used'
    prepared_b = read(ROOT / 'CM0102/Analysis/crt-length-maintained-prepared-b/report.json')
    checked(first_snapshot / 'manifest.json', prepared_b['preserved_first_manifest_sha256'])
    checked(ROOT / 'CM0102/Analysis/crt-length-maintained-prepared-a/report.json', prepared_b['prior_report_sha256'])
    for name, digest in prepared_a['outputs'].items():
        checked(first_snapshot / name, digest)
    for name, digest in prepared_b['outputs'].items():
        checked(ROOT / name, digest)
    old_header = (first_snapshot / 'recovery/crt/cm_crt_length.h').read_text(encoding='utf-8')
    assert old_header.replace('An encountered NUL is read again.', 'A NUL encountered by the loop is read again.') == (ROOT / entry['header']).read_text(encoding='utf-8')
    for run in ledger['validation_runs']:
        path = ROOT / run['report']
        checked(path, run['sha256'])
        check_test(path, run['source_hashes'])
    snapshot = ROOT / ledger['source_snapshot']['path']
    checked(snapshot / 'manifest.json', ledger['source_snapshot']['manifest_sha256'])
    for name, digest in read(snapshot / 'manifest.json')['sources'].items():
        checked(ROOT / name, digest)
        checked(snapshot / name, digest)
    for name, digest in ledger['provenance_inputs'].items():
        checked(ROOT / name, digest)
    return ledger


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--candidate-only', action='store_true')
    args = parser.parse_args()
    out = args.output.resolve()
    if out == ROOT / 'CM0102/Analysis' or not out.is_relative_to(ROOT / 'CM0102/Analysis'):
        parser.error('output must be a fresh directory beneath CM0102/Analysis')
    out.mkdir(exist_ok=False)
    result = dict(status='incomplete', command=sys.argv, python=sys.version, capstone=capstone.__version__,
                  script_sha256=pe.digest(Path(__file__)), input_sha256=pe.SHA256)
    try:
        generation, rows = verify_candidate()
        if not args.candidate_only:
            ledger = verify_maintained(generation, rows)
            result['ledger_sha256'] = pe.digest(ROOT / 'recovery/crt/length-ledger.json')
            result['validated_runs'] = len(ledger['validation_runs'])
        result.update(status='pass', candidate_evidence_verified=True, cases_per_run=48176,
                      original_bytes=43, original_instructions=20, integrated=False)
    except Exception as exc:
        result['error'] = repr(exc)
        raise
    finally:
        (out / 'audit.json').write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print('PASS bounded-length provenance and instruction audit')


if __name__ == '__main__':
    main()
