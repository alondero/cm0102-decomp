"""Export a byte-checked lexical closure of the locale wrappers' exception helper."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import sys
import capstone
import dump_rng_slice as pe

REGIONS = (
    ('locale_shared_dispatch', 0x94bf5c, 189),
    ('global_unwind_wrapper', 0x9451dc, 32),
    ('collided_unwind_callback', 0x9451fc, 34),
    ('local_scope_unwind', 0x94521e, 104),
    ('current_scope_test', 0x945286, 35),
    ('notify_shared_entries', 0x9452a9, 33),
    ('unwind_import_thunk', 0x9532a0, 6),
)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    out = args.output.resolve()
    if out == pe.ROOT / 'CM0102/Analysis' or not out.is_relative_to(pe.ROOT / 'CM0102/Analysis'):
        parser.error('Fresh output beneath Analysis required')
    assert __debug__ and pe.digest(pe.TARGET) == pe.digest(pe.FALLBACK) == pe.SHA256
    out.mkdir(exist_ok=False)
    copy = out / 'cm0102.exe'
    shutil.copyfile(pe.TARGET, copy)
    assert pe.digest(copy) == pe.SHA256
    data, sections = pe.load_pe(copy)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    regions = []
    for name, va, size in REGIONS:
        raw = pe.read_va(data, sections, va, size)
        instructions = list(md.disasm(raw, va))
        assert sum(i.size for i in instructions) == size
        rows = []
        for ins in instructions:
            row = dict(va=hex(ins.address), bytes=ins.bytes.hex(),
                instruction=ins.mnemonic + ' ' + ins.op_str)
            if ins.group(capstone.CS_GRP_CALL) or ins.group(capstone.CS_GRP_JUMP):
                row['control_target'] = ins.op_str
            rows.append(row)
        (out / f'{name}.bin').write_bytes(raw)
        regions.append(dict(name=name, va=hex(va), size=size,
            file_offset=hex(pe.va_to_offset(sections, va)), sha256=hashlib.sha256(raw).hexdigest(),
            instructions=rows, identified=True, decompiled=False, compiled=False,
            validated=False, integrated=False,
            scope='Decoded lexical range, including alternate/shared entries; no behavioral validation'))
    # Read the imported name from the original PE descriptors; infer no import behavior.
    pe_offset = struct.unpack_from('<I', data, 0x3c)[0]
    import_rva = struct.unpack_from('<I', data, pe_offset + 24 + 104)[0]
    imports = {}
    for index in range(1024):
        descriptor = pe.read_va(data, sections, pe.IMAGE_BASE + import_rva + index * 20, 20)
        original, _, _, name_rva, first = struct.unpack('<5I', descriptor)
        if not any(descriptor):
            break
        module = pe.ascii_at(data, sections, pe.IMAGE_BASE + name_rva)
        for slot in range(65536):
            value = struct.unpack('<I', pe.read_va(data, sections, pe.IMAGE_BASE + (original or first) + 4 * slot, 4))[0]
            if value == 0:
                break
            name = f'ordinal:{value & 0xffff}' if value & 0x80000000 else pe.ascii_at(data, sections, pe.IMAGE_BASE + value + 2)
            imports[pe.IMAGE_BASE + first + 4 * slot] = module + '::' + name
    assert imports[0x967158].lower() == 'kernel32.dll::rtlunwind'
    scope_tables = []
    for va, count in ((0x971600, 2), (0x971960, 1)):
        raw = pe.read_va(data, sections, va, count * 12)
        scope_tables.append(dict(va=hex(va), bytes=raw.hex(), sha256=hashlib.sha256(raw).hexdigest(),
            rows=[dict(enclosing=parent, filter_va=hex(filt), handler_va=hex(handler))
                for parent, filt, handler in struct.iter_unpack('<iII', raw)]))
    report = dict(status='pass', command=sys.argv, python=sys.version, capstone=capstone.__version__,
        input='CM0102/Game/cm0102.exe', input_sha256=pe.SHA256, copied_input_sha256=pe.digest(copy),
        address_convention='preferred-image VA, image base 0x00400000; file offsets separately labelled',
        script_sha256=pe.digest(Path(__file__)), support_script_sha256=pe.digest(Path(pe.__file__)),
        capstone_module_sha256=pe.digest(Path(capstone.__file__)), regions=regions,
        alternate_notify_entry_va='0x9452b2', scope_tables=scope_tables,
        unwind_import=dict(thunk_va='0x9532a0', iat_va='0x967158', name=imports[0x967158],
            behavior='External dependency; PE import identity alone establishes no unwind contract'),
        notification_data=dict(va='0xad6bfc', initial_bytes=pe.read_va(data, sections, 0xad6bfc, 16).hex()),
        limitations=['No recovery ledger promotion or replacement',
            'Indirect filters/finally/recovery callbacks remain caller-defined',
            'Windows exception entry, unwind, nonlocal return, guard pages and nested faults remain unvalidated'])
    (out / 'report.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    assert pe.digest(pe.FALLBACK) == pe.digest(copy) == pe.SHA256
    print('PASS exception dependency map:', sum(size for _, _, size in REGIONS),
        'bytes,', sum(len(r['instructions']) for r in regions), 'instructions; behavior unvalidated')


if __name__ == '__main__':
    main()
