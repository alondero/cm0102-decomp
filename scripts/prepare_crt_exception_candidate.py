"""Translate the identified exception-helper closure into an unaccepted x86 candidate."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import sys

import capstone
import dump_rng_slice as pe


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    out = args.output.resolve()
    assert out != pe.ROOT / 'CM0102/Analysis' and out.is_relative_to(pe.ROOT / 'CM0102/Analysis')
    assert __debug__ and pe.digest(pe.TARGET) == pe.digest(pe.FALLBACK) == pe.SHA256
    source = pe.ROOT / 'CM0102/Analysis/crt-exception-dependencies-a'
    report_path = source / 'report.json'
    discovery = json.loads(report_path.read_text(encoding='utf-8'))
    assert discovery['status'] == 'pass' and discovery['input_sha256'] == pe.SHA256
    data, sections = pe.load_pe(pe.TARGET)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    decoded = []
    regions = []
    for region in sorted(discovery['regions'], key=lambda r: int(r['va'], 16)):
        va, size = int(region['va'], 16), region['size']
        blob = pe.read_va(data, sections, va, size)
        assert hashlib.sha256(blob).hexdigest() == region['sha256'] == pe.digest(source / (region['name'] + '.bin'))
        instructions = list(md.disasm(blob, va))
        assert sum(i.size for i in instructions) == size
        assert [(hex(i.address), i.bytes.hex(), i.mnemonic + ' ' + i.op_str) for i in instructions] == [
            (r['va'], r['bytes'], r['instruction']) for r in region['instructions']]
        symbol = 'cm_crt_' + region['name']
        decoded.extend((symbol, i) for i in instructions)
        regions.append(dict(name=region['name'], symbol=symbol, va=region['va'], size=size,
            sha256=region['sha256'], instructions=len(instructions)))
    labels = {i.address: 'cm_crt_exception_i_%08x' % i.address for _, i in decoded}
    lines = ['/* Official 3.9.68 primary, SHA-256 ' + pe.SHA256,
        ' * Preferred-image VAs, base 0x00400000. Unaccepted lexical recovery.',
        ' * RtlUnwind and indirect caller callbacks retain explicit boundaries.',
        ' * No Windows exception delivery or unwind behavior is substituted. */',
        '    .intel_syntax noprefix',
        '    .section .text$cm_crt_exception_closure,"xr"']
    rows = []
    owner = None
    for symbol, ins in decoded:
        if owner != symbol:
            owner = symbol
            lines += ['    .org 0x%x' % (ins.address - 0x945000), '    .globl _' + symbol, '_' + symbol + ':']
        operand = ins.op_str
        if ins.group(capstone.CS_GRP_JUMP) or (ins.mnemonic == 'call' and ins.operands[0].type == capstone.x86.X86_OP_IMM):
            if ins.operands[0].type == capstone.x86.X86_OP_IMM:
                target = ins.operands[0].imm
                assert target in labels, hex(target)
                operand = labels[target]
        prefix = '{disp32} ' if (ins.bytes[:2] == b'\x0f\x85' or ins.bytes[:1] == b'\xe9') else ''
        assembly = prefix + ins.mnemonic + (' ' + operand if operand else '')
        lines += ['    .globl ' + labels[ins.address], labels[ins.address] + ':', '    ' + assembly]
        rows.append(dict(va=hex(ins.address), bytes=ins.bytes.hex(), instruction=ins.mnemonic + ' ' + ins.op_str,
            assembly=assembly, label=labels[ins.address], owner=symbol))
    out.mkdir(exist_ok=False)
    shutil.copyfile(pe.TARGET, out / 'cm0102.exe')
    shutil.copyfile(Path(__file__), out / Path(__file__).name)
    (out / 'cm_crt_exception.S').write_text('\n'.join(lines) + '\n', encoding='utf-8')
    (out / 'instructions.json').write_text(json.dumps(rows, indent=2) + '\n', encoding='utf-8')
    dependencies = dict(scope_tables=discovery['scope_tables'], unwind_import=discovery['unwind_import'],
        notification_data=discovery['notification_data'], alternate_entry_va='0x9452b2',
        caller_callbacks='Scope filters, finally callbacks and recovery targets remain supplied caller code.',
        residuals=['Windows dispatch/unwind/nonlocal transfer, nested faults and guard pages unvalidated',
            'Initialization and ownership of notification globals unrecovered'])
    (out / 'dependencies.json').write_text(json.dumps(dependencies, indent=2) + '\n', encoding='utf-8')
    result = dict(status='unaccepted_source_candidate', command=sys.argv, python=sys.version,
        input='CM0102/Game/cm0102.exe', input_sha256=pe.SHA256,
        address_convention='Preferred-image VA, base 0x00400000',
        script_sha256=pe.digest(Path(__file__)), discovery_sha256=pe.digest(report_path),
        support_sha256=pe.digest(Path(pe.__file__)), capstone=capstone.__version__,
        capstone_module_sha256=pe.digest(Path(capstone.__file__)), regions=regions,
        instructions=len(rows), original_bytes=sum(r['size'] for r in regions),
        assembly_layout='One section with explicit original VA offsets; COFF padding cannot separate adjacent helpers',
        identified=True, decompiled=True, compiled=False, validated=False, integrated=False,
        accepted_into_maintained_ledger=False,
        artifacts={p.name: pe.digest(p) for p in out.iterdir() if p.is_file()})
    (out / 'report.json').write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print('PASS unaccepted exception source:', result['original_bytes'], 'bytes,', len(rows), 'instructions')


if __name__ == '__main__':
    main()
