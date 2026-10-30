"""Recheck frozen primary/GDI map bytes and pin their support tools in fresh evidence."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import capstone
import dump_rng_slice as pe
from prepare_runtime import ROOT, GDI_SHA256


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    out = args.output.resolve()
    if out == ROOT / 'CM0102/Analysis' or not out.is_relative_to(ROOT / 'CM0102/Analysis'):
        parser.error('Fresh output beneath Analysis required')
    out.mkdir(exist_ok=False)
    assert __debug__ and pe.digest(pe.TARGET) == pe.digest(pe.FALLBACK) == pe.SHA256
    copy = ROOT / 'CM0102/Analysis/rng-gdi-map-b/cm0102_GDI.exe'
    assert pe.digest(copy) == pe.digest(ROOT / 'CM0102/Game/cm0102_GDI.exe') == GDI_SHA256
    primary, sections = pe.load_pe(pe.TARGET)
    native, native_sections = pe.load_pe(copy)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    checks = []
    records = 0
    for report_rel, script_rel in (
        ('CM0102/Analysis/rng-gdi-map-b/report.json', 'scripts/map_rng_gdi.py'),
        ('CM0102/Analysis/phase-gdi-map-b/report.json', 'CM0102/Analysis/map-phase-gdi.py')):
        path = ROOT / report_rel
        report = json.loads(path.read_text(encoding='utf-8'))
        assert report['status'] == 'pass' and pe.digest(ROOT / script_rel) == report['script_sha256']
        assert report.get('primary_sha256', report.get('input_primary_sha256')) == pe.SHA256
        assert report.get('gdi_sha256', report.get('input_gdi_sha256')) == GDI_SHA256
        bodies = report.get('functions', [report])
        for body in bodies:
            a = int(body.get('primary_va', body.get('primary_fragment_va')), 16)
            b = int(body.get('gdi_va', body.get('gdi_fragment_va')), 16)
            size = body['size']
            original = pe.read_va(primary, sections, a, size)
            mapped = pe.read_va(native, native_sections, b, size)
            assert hashlib.sha256(original).hexdigest() == body.get('primary_sha256', body.get('primary_fragment_sha256'))
            assert hashlib.sha256(mapped).hexdigest() == body.get('gdi_sha256', body.get('gdi_fragment_sha256'))
            decoded = list(md.disasm(original, a))
            native_decoded = list(md.disasm(mapped, b))
            assert len(decoded) == len(native_decoded) == len(body['instructions'])
            assert sum(i.size for i in decoded) == sum(i.size for i in native_decoded) == size
            for row, x, y in zip(body['instructions'], decoded, native_decoded):
                assert int(row['primary_va'], 16) == x.address and int(row['gdi_va'], 16) == y.address
                assert row['primary_bytes'] == x.bytes.hex() and row['gdi_bytes'] == y.bytes.hex()
                assert row['primary_instruction'] == x.mnemonic + ' ' + x.op_str
                assert row['gdi_instruction'] == y.mnemonic + ' ' + y.op_str
            records += len(decoded)
        for constant in report.get('data_constants', []):
            a = pe.read_va(primary, sections, int(constant['primary_va'], 16), constant['size'])
            b = pe.read_va(native, native_sections, int(constant['gdi_va'], 16), constant['size'])
            assert a == b and hashlib.sha256(a).hexdigest() == constant['sha256']
        checks.append(dict(report=report_rel, report_sha256=pe.digest(path), script=script_rel,
            script_sha256=pe.digest(ROOT / script_rel)))
    report = dict(status='pass', command=sys.argv, python=sys.version, capstone=capstone.__version__,
        script_sha256=pe.digest(Path(__file__)), checked_instruction_records=records, maps=checks,
        input_primary_sha256=pe.SHA256, input_gdi_sha256=GDI_SHA256,
        address_convention='preferred-image VA in each binary, image base 0x00400000',
        support_inputs={str(p.relative_to(ROOT)).replace('\\', '/'): pe.digest(p) for p in
            (ROOT / 'scripts/dump_rng_slice.py', ROOT / 'scripts/prepare_runtime.py')},
        capstone_module_sha256=pe.digest(Path(capstone.__file__)),
        scope='Fresh check of reported source bytes/constants and current tool pins; original reports preserved',
        limitations=['No live text attestation or atomic memory snapshot',
            'No external callee bodies or whole-game parity validated'])
    (out / 'report.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('PASS fresh map-byte audit:', records, 'instruction records')


if __name__ == '__main__':
    main()
