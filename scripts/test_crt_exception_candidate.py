"""Compare an unaccepted exception closure under arbitrary synthetic callback contracts.

This is a CPU differential harness. Its scripted RtlUnwind outcomes are not a
Windows unwind implementation; no live exception or guard-page parity is claimed.
"""
import argparse
import hashlib
import itertools
import json
from pathlib import Path
import random
import shutil
import struct
import subprocess
import sys

import capstone
import unicorn
from unicorn.x86_const import *
import dump_rng_slice as pe

STACK, STACK_SIZE = 0x2000000, 0x20000
ARENA, ARENA_SIZE = 0x400000, 0x4000
TEB, STOP, API = 0x5000000, 0x3000000, 0x6000000
RECORD, TABLE, EXCEPTION, CONTEXT, DISPATCH = (ARENA + x for x in (0x200, 0x400, 0x600, 0x800, 0xa00))
REGS = (UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EBX,
    UC_X86_REG_EBP, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_ESP, UC_X86_REG_EFLAGS)


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


class Machine:
    def __init__(self, blocks):
        self.uc = unicorn.Uc(unicorn.UC_ARCH_X86, unicorn.UC_MODE_32)
        pages = {(address + offset) & ~0xfff for address, blob in blocks for offset in range(len(blob))}
        for page in sorted(pages):
            self.uc.mem_map(page, 0x1000)
        for address, blob in blocks:
            self.uc.mem_write(address, blob)
        for base, size in ((STACK, STACK_SIZE), (ARENA, ARENA_SIZE), (TEB, 0x2000),
                (STOP, 0x1000), (API, 0x1000), (0x967000, 0x1000), (0xad6000, 0x1000)):
            assert base not in pages
            self.uc.mem_map(base, size)
        def descriptor(base, access):
            limit = 0xfffff
            return ((limit & 0xffff) | ((base & 0xffff) << 16) | (((base >> 16) & 255) << 32) |
                (access << 40) | (((limit >> 16) & 15) << 48) | (0xc << 52) | (((base >> 24) & 255) << 56))
        self.uc.mem_write(TEB + 0x1000, struct.pack('<QQQQ', 0, descriptor(0, 0x9b), descriptor(0, 0x93), descriptor(TEB, 0x93)))
        self.uc.reg_write(UC_X86_REG_GDTR, (0, TEB + 0x1000, 31, 0))
        self.uc.reg_write(UC_X86_REG_CS, 8)
        for reg in (UC_X86_REG_DS, UC_X86_REG_ES, UC_X86_REG_SS):
            self.uc.reg_write(reg, 16)
        self.uc.reg_write(UC_X86_REG_FS, 24)
        self.uc.mem_write(0x967158, struct.pack('<I', API))
        self.callbacks = {API: 'scripted_RtlUnwind'}
        for index in range(3):
            self.callbacks[API + 0x40 + index * 0x20] = ('filter', index)
            self.callbacks[API + 0x50 + index * 0x20] = ('handler', index)
        for address in self.callbacks:
            self.uc.mem_write(address, b'\xc3')
        self.uc.hook_add(unicorn.UC_HOOK_MEM_READ | unicorn.UC_HOOK_MEM_WRITE, self.access)
        self.uc.hook_add(unicorn.UC_HOOK_MEM_INVALID, self.fault)
        self.uc.hook_add(unicorn.UC_HOOK_CODE, self.code)

    def access(self, uc, access, address, size, value, user):
        if access == unicorn.UC_MEM_READ:
            self.events.append(('read', address, size, bytes(uc.mem_read(address, size)).hex()))
        else:
            self.events.append(('write', address, size, value))

    def fault(self, uc, access, address, size, value, user):
        self.events.append(('fault', access, address, size, value))
        return False

    def code(self, uc, address, size, user):
        self.executed.add(address)
        if address not in self.callbacks:
            return
        sp = uc.reg_read(UC_X86_REG_ESP)
        kind = self.callbacks[address]
        values = struct.unpack('<5I', uc.mem_read(sp, 20))
        event = dict(callback=kind, sp=sp, frame=uc.reg_read(UC_X86_REG_EBP),
            stack=list(values), registers=[uc.reg_read(r) for r in REGS],
            record=bytes(uc.mem_read(RECORD - 4, 28)).hex(),
            fs=bytes(uc.mem_read(TEB, 4)).hex())
        self.callbacks_seen.append(event)
        if kind == 'scripted_RtlUnwind':
            if self.options['api_stop']:
                event['outcome'] = 'arbitrary callback stops execution'
                uc.emu_stop()
                return
            cleanup, result = 20, self.options['api_result']
        else:
            family, index = kind
            if family == 'handler' and self.options['handler_stop']:
                event['outcome'] = 'arbitrary callback stops execution'
                uc.emu_stop()
                return
            cleanup = 4
            result = self.options['filters'][index] if family == 'filter' else 0x10203040
            destination = ARENA + 0x1000 + 4 * len(self.callbacks_seen)
            value = ((index + 1) << 24) | (0xf100 if family == 'filter' else 0xfa00) | len(self.callbacks_seen)
            uc.mem_write(destination, struct.pack('<I', value))
            event['scripted_write'] = [destination, value]
        event['outcome'] = 'arbitrary return with explicit volatile-register clobbers'
        uc.reg_write(UC_X86_REG_EAX, result & 0xffffffff)
        uc.reg_write(UC_X86_REG_ECX, 0xa1b2c3d4)
        uc.reg_write(UC_X86_REG_EDX, 0x55667788)
        uc.reg_write(UC_X86_REG_ESP, sp + cleanup)
        uc.reg_write(UC_X86_REG_EIP, values[0])

    def run(self, entry, options):
        self.options = options
        self.uc.mem_write(STACK, bytes([options['pattern']]) * STACK_SIZE)
        self.uc.mem_write(ARENA, b'\xa5' * ARENA_SIZE)
        self.uc.mem_write(0xad6000, b'\x6b' * 0x1000)
        self.uc.mem_write(TEB, b'\x00' * 0x1000)
        # Synthetic caller-owned frame and scopes; these are not OS-delivered records.
        self.uc.mem_write(RECORD - 4, struct.pack('<7I', 0xa5a5a5a5, 0x12345678,
            0x94bf5c, TABLE, options['level'] & 0xffffffff, 0x11111111, 0x22222222))
        for index, parent in enumerate((-1, 0, 1)):
            filt = API + 0x40 + index * 0x20 if options['filter_mask'] & (1 << index) else 0
            self.uc.mem_write(TABLE + index * 12, struct.pack('<iII', parent, filt, API + 0x50 + index * 0x20))
        self.uc.mem_write(EXCEPTION, struct.pack('<5I', 0xe06d7363, options['exception_flags'], 0, entry, 0))
        self.uc.mem_write(DISPATCH, struct.pack('<I', 0xeeeeeeee))
        fs_record = ARENA + 0xc00
        self.uc.mem_write(fs_record, struct.pack('<4I', 0x12345678, options['chain_handler'],
            options['chain_level'] & 0xffffffff, RECORD))
        self.uc.mem_write(TEB, struct.pack('<I', fs_record if options['fs_valid'] else 0x12345678))
        frame = RECORD + 0x10
        if entry == 0x94bf5c:
            arguments = [EXCEPTION, RECORD, CONTEXT, DISPATCH]
        elif entry in (0x9451dc, 0x9532a0):
            arguments = [RECORD] if entry == 0x9451dc else [RECORD, STOP, EXCEPTION, options['api_result'] & 0xffffffff]
        elif entry == 0x9451fc:
            arguments = [EXCEPTION, RECORD, CONTEXT, DISPATCH]
        elif entry == 0x94521e:
            arguments = [RECORD, options['target'] & 0xffffffff]
        elif entry in (0x9452a9, 0x9452b2):
            arguments = [options['notify_arg']]
        else:
            assert entry == 0x945286
            arguments = []
        sp = STACK + STACK_SIZE - 0x1000 - options['alignment']
        self.uc.mem_write(sp, struct.pack('<' + 'I' * (len(arguments) + 1), STOP, *arguments))
        initial = (0x33445566, 0x12345678, 0x55667788, 0x89abcdef, frame,
            0x2468ace0, 0xfedcba98, sp, options['flags'])
        for register, value in zip(REGS, initial):
            self.uc.reg_write(register, value)
        self.events, self.callbacks_seen, self.executed = [], [], set()
        error = None
        try:
            self.uc.emu_start(entry, STOP, count=2000)
        except unicorn.UcError as exc:
            error = exc.errno
        return dict(error=error, eip=self.uc.reg_read(UC_X86_REG_EIP),
            registers=[self.uc.reg_read(r) for r in REGS],
            stack_sha256=hashlib.sha256(self.uc.mem_read(STACK, STACK_SIZE)).hexdigest(),
            arena=bytes(self.uc.mem_read(ARENA, ARENA_SIZE)).hex(),
            notification=bytes(self.uc.mem_read(0xad6000, 0x1000)).hex(),
            fs=bytes(self.uc.mem_read(TEB, 0x1000)).hex(),
            events=self.events, callbacks=self.callbacks_seen, executed=sorted(self.executed))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--candidate', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    out, candidate = args.output.resolve(), args.candidate.resolve()
    area = pe.ROOT / 'CM0102/Analysis'
    assert out != area and out.is_relative_to(area) and candidate.is_relative_to(area)
    assert __debug__ and pe.digest(pe.TARGET) == pe.digest(pe.FALLBACK) == pe.SHA256
    generation = read(candidate / 'report.json')
    assert generation['input_sha256'] == pe.SHA256 and not generation['accepted_into_maintained_ledger']
    assert all(pe.digest(candidate / name) == digest for name, digest in generation['artifacts'].items())
    out.mkdir(exist_ok=False)
    toolchain = Path('C:/ProgramData/mingw64/mingw64/bin')
    report = dict(status='incomplete', command=sys.argv, python=sys.version, unicorn=unicorn.__version__,
        capstone=capstone.__version__, input_sha256=pe.SHA256,
        address_convention='Original and compiled candidate at identical preferred-image VAs, base 0x00400000',
        generation_sha256=pe.digest(candidate / 'report.json'),
        sources={p.relative_to(pe.ROOT).as_posix(): pe.digest(p) for p in
            (candidate / 'cm_crt_exception.S', Path(__file__), Path(pe.__file__))},
        tools={name: dict(sha256=pe.digest(toolchain / name),
            version=subprocess.check_output([str(toolchain / name), '--version'], text=True))
            for name in ('gcc.exe', 'as.exe', 'ld.exe', 'nm.exe')},
        module_hashes={name: pe.digest(Path(module.__file__)) for name, module in
            (('capstone', capstone), ('unicorn', unicorn))},
        identified=True, decompiled=True, compiled=False, validated=False, integrated=False,
        accepted_into_maintained_ledger=False,
        limitations=['Arbitrary scripted stdcall RtlUnwind return/stop outcomes, not Windows unwinding',
            'Caller filters/finally/recovery callbacks scripted under explicit clobber/write contracts',
            'Synthetic flat 32-bit segments and FS registration; no OS exception delivery or guard growth',
            'Finite states, malformed records and bounded instruction execution; no universal proof',
            'No live integration, mutable-global initialization or complete locale composition'])
    commands = [
        [str(toolchain / 'gcc.exe'), '-m32', '-c', str(candidate / 'cm_crt_exception.S'), '-o', str(out / 'exception.o')]]
    layout = ['SECTIONS { .text 0x00945000 : SUBALIGN(1) {',
        ' exception.o(.text$cm_crt_exception_closure)',
        '} /DISCARD/ : { *(.data) *(.bss) *(.rdata$zzz) } }']
    (out / 'layout.ld').write_text('\n'.join(layout) + '\n', encoding='utf-8')
    commands.append([str(toolchain / 'ld.exe'), '-mi386pe', '--image-base', '0x400000',
        '--entry', '_cm_crt_locale_shared_dispatch', '--disable-reloc-section',
        '-T', 'layout.ld', 'exception.o', '-o', 'candidate.exe'])
    def save_report():
        report['artifacts'] = {p.name: pe.digest(p) for p in out.iterdir() if p.is_file() and p.name != 'report.json'}
        (out / 'report.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    try:
        report['build_commands'] = commands
        save_report()
        for index, command in enumerate(commands):
            completed = subprocess.run(command, cwd=out, capture_output=True, text=True)
            (out / ('build-%d.txt' % index)).write_text(completed.stdout + completed.stderr, encoding='utf-8')
            completed.check_returncode()
        report['compiled'] = True
        native_data, native_sections = pe.load_pe(out / 'candidate.exe')
        original_data, original_sections = pe.load_pe(candidate / 'cm0102.exe')
        rows = read(candidate / 'instructions.json')
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        original_blocks, native_blocks = [], []
        checked = []
        nm = subprocess.check_output([str(toolchain / 'nm.exe'), '-n', str(out / 'candidate.exe')], text=True)
        (out / 'symbols.txt').write_text(nm, encoding='utf-8')
        symbols = {line.split()[2]: int(line.split()[0], 16) for line in nm.splitlines() if len(line.split()) == 3}
        for region in generation['regions']:
            va, size = int(region['va'], 16), region['size']
            assert symbols['_' + region['symbol']] == va
            original = pe.read_va(original_data, original_sections, va, size)
            native = pe.read_va(native_data, native_sections, va, size)
            native_instructions = list(md.disasm(native, va))
            selected = [r for r in rows if r['owner'] == region['symbol']]
            assert len(native_instructions) == len(selected)
            for instruction, row in zip(native_instructions, selected):
                assert instruction.address == int(row['va'], 16) == symbols[row['label']]
                assert instruction.mnemonic + ' ' + instruction.op_str == row['instruction'], (row, instruction.mnemonic, instruction.op_str)
            original_blocks.append((va, original))
            native_blocks.append((va, native))
            checked.append(dict(va=hex(va), size=size, native_sha256=hashlib.sha256(native).hexdigest(),
                original_sha256=hashlib.sha256(original).hexdigest(), byte_identical=native == original))
        report['compiled_regions'] = checked
        reference, recovered = Machine(original_blocks), Machine(native_blocks)
        defaults = dict(level=2, target=-1, filter_mask=7, filters=(0, 0, 0), exception_flags=0,
            api_result=0x13579bdf, api_stop=False, handler_stop=False, chain_handler=0x9451fc,
            chain_level=2, fs_valid=True, notify_arg=0x101, alignment=0, flags=2, pattern=0xcc)
        cases = []
        entries = (0x94bf5c, 0x9451dc, 0x9451fc, 0x94521e, 0x945286, 0x9452a9, 0x9452b2, 0x9532a0)
        for entry, flags, alignment in itertools.product(entries, (2, 3, 0x402, 0x803), range(4)):
            cases.append((entry, dict(flags=flags, alignment=alignment)))
        for exception_flags, level, mask, filters in itertools.product((0, 1, 2, 4, 6, 8, 0xffffffff),
                (-1, 0, 1, 2), (0, 1, 3, 5, 7), ((0, 0, 0), (-1, 0, 0), (0, -1, 0),
                    (0, 0, -1), (1, 0, 0), (0, 1, 0), (0, 0, 1), (0x80000000, 0, 0))):
            cases.append((0x94bf5c, dict(exception_flags=exception_flags, level=level, filter_mask=mask, filters=filters)))
        for level, target, mask in itertools.product((-1, 0, 1, 2), (-1, 0, 1, 2), range(8)):
            cases.append((0x94521e, dict(level=level, target=target, filter_mask=mask)))
        for flags, fs_valid, handler, level in itertools.product((0, 2, 4, 6, 0xffffffff),
                (False, True), (0x9451fc, 0, 0x9451fd), (-1, 0, 2, 0x7fffffff)):
            cases.append((0x945286, dict(exception_flags=flags, fs_valid=fs_valid, chain_handler=handler, chain_level=level)))
            cases.append((0x9451fc, dict(exception_flags=flags, fs_valid=fs_valid, chain_handler=handler, chain_level=level)))
        for entry, api_stop, handler_stop, level in itertools.product((0x94bf5c, 0x9451dc, 0x9532a0, 0x94521e),
                (False, True), (False, True), (0, 1, 2)):
            cases.append((entry, dict(api_stop=api_stop, handler_stop=handler_stop, level=level, filters=(1, 1, 1), filter_mask=3)))
        rng = random.Random(0x9451fc)
        for _ in range(300):
            cases.append((rng.choice(entries), dict(flags=rng.choice((2, 3, 0x402, 0x803)),
                level=rng.choice((-1, 0, 1, 2)), target=rng.choice((-1, 0, 1, 2)),
                filter_mask=rng.randrange(8), filters=tuple(rng.choice((0, -1, 1, 0x80000000, 0x7fffffff)) for _ in range(3)),
                exception_flags=rng.getrandbits(32), api_result=rng.getrandbits(32),
                chain_handler=rng.choice((0x9451fc, 0, 0x9451fd)), chain_level=rng.getrandbits(32),
                pattern=rng.randrange(256), alignment=rng.randrange(16))))
        executed, outcomes, callback_counts = set(), {}, {}
        with (out / 'cases.jsonl').open('w', encoding='utf-8') as stream:
            for index, (entry, changes) in enumerate(cases):
                options = dict(defaults, **changes)
                expected, actual = reference.run(entry, options), recovered.run(entry, options)
                if expected != actual:
                    (out / 'failure.json').write_text(json.dumps(dict(index=index, entry=hex(entry),
                        options=options, original=expected, recovered=actual), indent=2) + '\n', encoding='utf-8')
                    raise AssertionError('Differential mismatch at case %d' % index)
                executed.update(actual['executed'])
                kind = 'fault:%s' % actual['error'] if actual['error'] is not None else 'eip:%08x' % actual['eip']
                outcomes[kind] = outcomes.get(kind, 0) + 1
                for event in actual['callbacks']:
                    name = str(event['callback'])
                    callback_counts[name] = callback_counts.get(name, 0) + 1
                fingerprint = hashlib.sha256(json.dumps(actual, sort_keys=True).encode()).hexdigest()
                stream.write(json.dumps(dict(index=index, entry=hex(entry), options=options,
                    outcome=kind, complete_comparison_sha256=fingerprint)) + '\n')
        original_addresses = {int(r['va'], 16) for r in rows}
        report.update(status='pass', cases=len(cases), outcomes=outcomes, callback_counts=callback_counts,
            lexical_instructions=len(rows), executed_original_instructions=len(executed & original_addresses),
            uncovered_instruction_vas=[hex(va) for va in sorted(original_addresses - executed)],
            validated=True, validation_scope='Finite synthetic CPU/callback corpus only')
        assert pe.digest(pe.TARGET) == pe.digest(pe.FALLBACK) == pe.SHA256
        shutil.copyfile(Path(__file__), out / Path(__file__).name)
        print('PASS exception candidate:', len(cases), 'cases,', report['executed_original_instructions'], '/', len(rows), 'instructions')
    except BaseException as error:
        report['error'] = repr(error)
        raise
    finally:
        save_report()


if __name__ == '__main__':
    main()
