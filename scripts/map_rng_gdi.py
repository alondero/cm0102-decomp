"""Map only the already recovered RNG slice between preserved primary and GDI copies."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import sys
import capstone
import dump_rng_slice as pe

ROOT=Path(__file__).resolve().parents[1]
GDI_HASH='0e50572c292bdfb9de8b3e087dd29043a13bcd0985ed198c9011e5a0f694388b'
FUNCTIONS=((0x90cfc0,221),(0x90d0a0,134),(0x9469fa,10),(0x946a04,30))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();out=args.output.resolve()
    if out==ROOT/'CM0102/Analysis' or not out.is_relative_to(ROOT/'CM0102/Analysis'):
        parser.error('fresh output beneath Analysis required')
    assert __debug__ and pe.digest(pe.TARGET)==pe.digest(pe.FALLBACK)==pe.SHA256
    supplied=ROOT/'CM0102/Game/cm0102_GDI.exe';assert pe.digest(supplied)==GDI_HASH
    out.mkdir(exist_ok=False);copy=out/'cm0102_GDI.exe';shutil.copyfile(supplied,copy)
    assert pe.digest(copy)==GDI_HASH
    primary,primary_sections=pe.load_pe(pe.TARGET);gdi,gdi_sections=pe.load_pe(copy)
    text=next(s for s in gdi_sections if s['name']=='.text')
    gdiblob=gdi[text['file_offset']:text['file_offset']+text['raw_size']]
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
    correspondences={};records=[]
    def mapped_address(value):
        return any(s['va']<=value<s['va']+max(s['raw_size'],s['virtual_size'])for s in primary_sections)
    for va,size in FUNCTIONS:
        blob=pe.read_va(primary,primary_sections,va,size)
        original=list(md.disasm(blob,va));assert sum(i.size for i in original)==size
        pattern=bytearray(blob);wild=set()
        for ins in original:
            if ins.group(capstone.CS_GRP_JUMP)or ins.group(capstone.CS_GRP_CALL):
                if ins.imm_size:wild.update(range(ins.address-va+ins.imm_offset,ins.address-va+ins.imm_offset+ins.imm_size))
            for operand in ins.operands:
                if operand.type==capstone.x86.X86_OP_MEM and mapped_address(operand.mem.disp):
                    wild.update(range(ins.address-va+ins.disp_offset,ins.address-va+ins.disp_offset+ins.disp_size))
                elif operand.type==capstone.x86.X86_OP_IMM and mapped_address(operand.imm):
                    wild.update(range(ins.address-va+ins.imm_offset,ins.address-va+ins.imm_offset+ins.imm_size))
        regex=b''.join(b'.'if index in wild else re.escape(bytes([value]))for index,value in enumerate(pattern))
        matches=list(re.finditer(regex,gdiblob,re.DOTALL))
        candidates=[text['va']+match.start()for match in matches]
        if va in correspondences:
            matches=[match for match in matches if text['va']+match.start()==correspondences[va]]
        assert len(matches)==1,(hex(va),candidates)
        native_va=text['va']+matches[0].start();native_blob=gdiblob[matches[0].start():matches[0].start()+size]
        native=list(md.disasm(native_blob,native_va));assert len(native)==len(original)
        comparisons=[]
        for a,b in zip(original,native):
            assert a.mnemonic==b.mnemonic and a.size==b.size and len(a.operands)==len(b.operands)
            assert a.address-va==b.address-native_va
            for x,y in zip(a.operands,b.operands):
                assert x.type==y.type and x.size==y.size
                if x.type==capstone.x86.X86_OP_REG:assert x.reg==y.reg
                elif x.type==capstone.x86.X86_OP_MEM:
                    assert (x.mem.segment,x.mem.base,x.mem.index,x.mem.scale)==(y.mem.segment,y.mem.base,y.mem.index,y.mem.scale)
                    old,new=x.mem.disp,y.mem.disp
                    if mapped_address(old):
                        assert correspondences.get(old,new)==new;correspondences[old]=new
                    else:assert old==new
                elif x.type==capstone.x86.X86_OP_IMM:
                    old,new=x.imm,y.imm
                    if va<=old<va+size:assert old-va==new-native_va
                    elif mapped_address(old):
                        assert correspondences.get(old,new)==new;correspondences[old]=new
                    else:assert old==new
            comparisons.append(dict(primary_va=f'0x{a.address:08x}',gdi_va=f'0x{b.address:08x}',
                primary_bytes=a.bytes.hex(),gdi_bytes=b.bytes.hex(),
                primary_instruction=a.mnemonic+' '+a.op_str,gdi_instruction=b.mnemonic+' '+b.op_str))
        correspondences[va]=native_va
        records.append(dict(primary_va=f'0x{va:08x}',gdi_va=f'0x{native_va:08x}',size=size,
            masked_match_candidates=[f'0x{v:08x}'for v in candidates],
            selection='Previously decoded caller target when ambiguous; unique full-body masked match otherwise',
            primary_sha256=hashlib.sha256(blob).hexdigest(),gdi_sha256=hashlib.sha256(native_blob).hexdigest(),
            instructions=comparisons))
    constants=[]
    for address,size in ((0xaa2454,51000*4),(0x970a70,8),(0xad437c,len('Random Number seed: %d')+1)):
        mapped=correspondences[address]
        a=pe.read_va(primary,primary_sections,address,size);b=pe.read_va(gdi,gdi_sections,mapped,size)
        assert a==b
        constants.append(dict(primary_va=hex(address),gdi_va=hex(mapped),size=size,sha256=hashlib.sha256(a).hexdigest()))
    report=dict(status='pass',command=sys.argv,python=sys.version,capstone=capstone.__version__,
        capstone_module_sha256=pe.digest(Path(capstone.__file__)),script_sha256=pe.digest(Path(__file__)),
        address_convention='preferred-image VA in each binary, image base 0x00400000',
        primary_sha256=pe.SHA256,gdi_sha256=GDI_HASH,functions=records,data_constants=constants,
        address_correspondences={f'0x{k:08x}':f'0x{v:08x}'for k,v in sorted(correspondences.items())},
        limitations=['Instruction/data correspondence for these four routines only',
            'Referenced time/format/ftol callees not mapped by this comparison',
            'No whole-binary equivalence, runtime determinism or live integration established'])
    assert pe.digest(pe.FALLBACK)==pe.SHA256 and pe.digest(supplied)==pe.digest(copy)==GDI_HASH
    (out/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print('PASS RNG primary/GDI mapping: four complete bodies and table/scale/format constants')


if __name__=='__main__':main()
