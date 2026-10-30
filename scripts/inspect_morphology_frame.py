"""Hash-locked original morphology frame/argument observation, not recovered source.

Executes only original prologue and stack probe in Unicorn. It stops before
language dispatch; no CRT or application behavior is substituted. Preserved
executables and exports are read only. Fresh output beneath local Analysis.
"""
from __future__ import annotations
import argparse
import csv
import hashlib
import json
import platform
import struct
import sys
from pathlib import Path
import capstone
import unicorn
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX,
    UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EFLAGS)
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'recovery/date'))
import test_cm_date as date
pe = date.pe
EXPORTS = {'morphology':'lang-loader-static-b', 'probe':'morph-probe-static-a'}
ENTRY, PROBE, POST_PROBE, DISPATCH, OUTPUT = 0x6701F0, 0x945610, 0x6701FA, 0x670271, 0xB64804
SPAN = 0x2818
def sha(blob):
    return hashlib.sha256(blob).hexdigest()
def evidence(out):
    data,sections = pe.load_pe(pe.TARGET)
    md = capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
    md.detail = True
    records, exports = {}, {}
    for kind,name in EXPORTS.items():
        directory = ROOT / 'CM0102/Analysis' / name
        report = json.loads((directory/'run.json').read_text(encoding='utf-8'))
        assert report['input_sha256']==pe.SHA256 and report['exit_code']==0
        assert all(pe.digest(directory/artifact)==expected for artifact,expected in report['outputs'].items())
        exports[directory.relative_to(ROOT).as_posix()] = pe.digest(directory/'run.json')
        rows=[]
        with (directory/'selected-disassembly.tsv').open() as stream:
            for row in csv.DictReader(stream,delimiter='\t'):
                if int(row['function_va'],16) != (ENTRY if kind=='morphology' else PROBE):
                    continue
                va,blob = int(row['instruction_va'],16),bytes.fromhex(row['bytes'])
                assert pe.read_va(data,sections,va,len(blob))==blob
                decoded=list(md.disasm(blob,va))
                assert len(decoded)==1 and decoded[0].size==len(blob)
                rows.append(row)
        identity=sha(''.join(r['instruction_va']+' '+r['bytes']+'\n' for r in rows).encode())
        records[kind]=dict(entry_va=f'0x{ENTRY if kind=="morphology" else PROBE:08x}',
            instructions=len(rows),instruction_bytes=sum(len(bytes.fromhex(r['bytes'])) for r in rows),
            ordered_instruction_identity=identity,identified=True,decompiled=False,compiled=False,validated=False,integrated=False)
        (out/(kind+'-instructions.json')).write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
    assert records['morphology']['instructions']==17394 and records['morphology']['instruction_bytes']==49771
    assert records['probe']['instruction_bytes']==47
    return records,exports
class Machine(date.Machine):
    def __init__(self):
        super().__init__(pe.TARGET)
        self.uc.hook_add(unicorn.UC_HOOK_MEM_READ,self.read)
        self.uc.hook_add(unicorn.UC_HOOK_CODE,self.at_stage,begin=POST_PROBE,end=POST_PROBE)
        self.uc.hook_add(unicorn.UC_HOOK_CODE,self.at_stage,begin=DISPATCH,end=DISPATCH)
    def write(self,uc,access,address,size,value,user):
        assert date.STACK<=address and address+size<=date.STACK+0x10000 or OUTPUT<=address and address+size<=OUTPUT+0x800, (address,size)
    def read(self,uc,access,address,size,value,user):
        pc=uc.reg_read(UC_X86_REG_EIP)
        if pc in (0x945627,0x945634):
            self.probes.append((address,size))
    def at_stage(self,uc,address,size,user):
        if address==POST_PROBE:
            assert uc.reg_read(UC_X86_REG_ESP)==self.entry_sp-0x2808
            assert uc.reg_read(UC_X86_REG_EAX)==POST_PROBE
            assert uc.reg_read(UC_X86_REG_ECX)==0x31313131
            assert struct.unpack('<I',uc.mem_read(self.entry_sp-0x280C,4))[0]==POST_PROBE
            self.post_probe=True
        else:
            uc.emu_stop()
    def observe(self,word,alignment=0,output_alias=False,null=False):
        self.entry_sp=date.STACK+0xF000
        self.prepare(bytes(date.GLOBAL_SIZE),bytes(0x100))
        self.uc.mem_write(OUTPUT,b'original shared text\0'+b'X'*32)
        pointer=0 if null else OUTPUT if output_alias else 0x300000+alignment
        if not null and not output_alias:
            # Only the source slice matters; the original mapper has a full page.
            self.uc.mem_write(pointer,word+b'\0')
        args=(pointer,0x11223344,0x55667788,0x12340005,0xFFFF0102)
        self.uc.mem_write(self.entry_sp,struct.pack('<6I',date.STOP,*args))
        for index,reg in enumerate(date.CALLEE_SAVED):
            self.uc.reg_write(reg,0x51515151+index)
        self.uc.reg_write(UC_X86_REG_ESP,self.entry_sp)
        self.uc.reg_write(UC_X86_REG_EAX,0x12345678)
        self.uc.reg_write(UC_X86_REG_ECX,0x31313131)
        self.uc.reg_write(UC_X86_REG_EDX,0xABCDEF01)
        self.uc.reg_write(UC_X86_REG_EFLAGS,2)
        self.probes,self.post_probe=[],False
        if null:
            self.uc.mem_unmap(0,0x1000)
            try:
                self.uc.emu_start(ENTRY,date.STOP,count=20000)
            except unicorn.UcError as exc:
                assert exc.errno==unicorn.UC_ERR_READ_UNMAPPED
                assert self.uc.reg_read(UC_X86_REG_EIP)==0x670235
                return dict(status='observed original unmapped-null read before null guard',fault_va='0x00670235')
            finally:
                self.uc.mem_map(0,0x1000)
            raise AssertionError('Null source unexpectedly reached dispatch')
        self.uc.emu_start(ENTRY,date.STOP,count=20000)
        assert self.uc.reg_read(UC_X86_REG_EIP)==DISPATCH and self.post_probe
        assert self.probes==[(self.entry_sp-0x1000,4),(self.entry_sp-0x2000,4),(self.entry_sp-0x2808,4)]
        frame=self.entry_sp-SPAN
        assert self.uc.reg_read(UC_X86_REG_ESP)==frame
        assert struct.unpack('<5I',self.uc.mem_read(frame+0x281C,20))==args
        expected=bytearray(b'\xcc'*SPAN)
        def put(offset,blob):
            expected[offset:offset+len(blob)]=blob
        put(SPAN-4,struct.pack('<I',POST_PROBE))
        put(SPAN-8,struct.pack('<I',0x31313131))
        # PUSH EBX,EBP,ESI,EDI after the relocated probe return is consumed.
        for offset,value in ((0,0x51515153),(4,0x51515152),(8,0x51515154),(12,0x51515151)):
            put(offset,struct.pack('<I',value))
        put(0x10,struct.pack('<II',0,1))
        put(0x18,b'\0');put(0x818,b'\0')
        put(0x1818,bytes(2048))
        copied=b'\0' if output_alias else word+b'\0'
        put(0x1018,copied)
        actual=bytes(self.uc.mem_read(frame,SPAN))
        assert actual==expected, (len(word),alignment,output_alias)
        assert bytes(self.uc.mem_read(OUTPUT,1))==b'\0'
        assert bytes(self.uc.mem_read(self.entry_sp,24))==struct.pack('<6I',date.STOP,*args)
        return dict(source_bytes=len(word),alignment=alignment,output_alias=output_alias,
                    copied_bytes=len(copied),frame_sha256=sha(actual))
def main():
    if not __debug__:
        raise SystemExit('Assertions required')
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',required=True,type=Path)
    parser.add_argument('--quick',action='store_true')
    args=parser.parse_args()
    out,analysis=args.output.resolve(),(ROOT/'CM0102/Analysis').resolve()
    if out==analysis or not out.is_relative_to(analysis):
        parser.error('output must be fresh beneath CM0102/Analysis')
    assert pe.digest(pe.TARGET)==pe.digest(pe.FALLBACK)==pe.SHA256
    out.mkdir(parents=True,exist_ok=False)
    sources={p.relative_to(ROOT).as_posix():pe.digest(p) for p in (Path(__file__),Path(date.__file__),ROOT/'scripts/dump_rng_slice.py')}
    records,exports=evidence(out)
    machine=Machine()
    cases=[]
    for length in range(0,2048,127 if args.quick else 1):
        for alignment in range(4):
            cases.append(machine.observe(bytes(1+i%255 for i in range(length)),alignment))
    cases.append(machine.observe(b'',output_alias=True))
    null=machine.observe(b'',null=True)
    assert pe.digest(pe.TARGET)==pe.digest(pe.FALLBACK)==pe.SHA256
    assert all(pe.digest(ROOT/name)==value for name,value in sources.items())
    (out/'observations.json').write_text(json.dumps(cases,indent=2)+'\n',encoding='utf-8')
    report=dict(status='pass',command=sys.argv,quick=args.quick,input_sha256=pe.SHA256,
        address_convention='preferred-image VA, base 0x00400000; incoming ESP=E, saved-register base P=E-0x2818',
        python=sys.version,platform=platform.platform(),unicorn=unicorn.__version__,capstone=capstone.__version__,
        sources=sources,exports=exports,functions=records,normal_prefix_observations=len(cases),null_observation=null,
        argument_offsets_from_P=['0x281c','0x2820','0x2824','0x2828','0x282c'],
        limitations=['Original prefix execution only; no compiled morphology/probe source or whole-routine validation',
            'Valid 0..2047-byte inputs and selected shared-output alias, DF clear, mapped stack; no guard-page OS/SEH validation',
            'Remaining language dispatch, CRT calls, private-frame behavior and full output remain unrecovered'],
        artifacts={p.name:pe.digest(p) for p in out.iterdir() if p.is_file()})
    (out/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(status='pass',normal_prefix_observations=len(cases),null_observation=null),indent=2))
if __name__=='__main__':
    main()
