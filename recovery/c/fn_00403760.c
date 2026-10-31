#include "cmre.h"
/* Preferred-image VA00403760/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/fullEAX, including incidental pointer/self-field returns.
   Scratch200 diagnosticsL0,L100. Signed WORD indices, wrapping DWORD
   arithmetic and signed comparisons. Output stores may alias self/tables:
   retain every original reload and the late output-formal reads. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00403760(u32 self,volatile u32 first_out,
                              volatile u32 second_out)
{
    u8 scratch[0x200];
    u32 l=(u32)scratch,index,base,record,child,other_index,flags;
    u32 left,right,value,sum,eax,pointer,other;
    index=W(self+0x20e);
    if(index==0xffff){
        cm_fn_00944cff(0x009873c0,0,0,l+0x100,l);
        eax=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,eax,l+0x100,l,0x2ce);
        eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return eax;
    }
    index=(u32)(s32)(s16)(u16)index;
    base=D(self+0x208); record=base+index*0x18cu;
    other_index=W(record+0x7a);
    if(other_index!=0xffff){
        other_index=(u32)(s32)(s16)(u16)other_index;
        base=D(self+0x204); child=base+other_index*0xbf1u;
        if(W(child+0x20e)!=0xffff){
            if((D(child+0x18)&0x100u)==0) goto linked;
        }
    }
    flags=D(self+0x18);
    if((flags&0x100u)!=0) goto flag100;
    left=D(self+0xc); right=D(self+4); value=D(record+0x14);
    sum=left-right+value;
    if(((s32)sum<600 || (flags&0x800000u)!=0) && (flags&0x400000u)==0){
        pointer=second_out; D(pointer)=value;
    }else{
        eax=D(record+0x1c)-left+right;
        if((s32)eax<=0) eax=0;
        pointer=second_out; D(pointer)=eax;
    }
    index=(u32)(s32)(s16)W(self+0x20e);
    base=D(self+0x208); record=base+index*0x18cu;
    eax=D(record+0x10);
    if((s32)eax<=400 || (D(self+0x18)&0x200000u)!=0){
        if((D(self+0x18)&0x100000u)==0){
            eax=D(record+0x18);
            pointer=first_out;
            eax+=3; D(pointer)=eax;
            return eax;
        }
    }
    right=D(self+8);
    eax-=right;
    other=D(self);
    pointer=first_out;
    eax=eax+other-3u;
    D(pointer)=eax;
    return eax;
flag100:
    value=D(record+0x1c); right=D(self+4); left=D(self+0xc);
    sum=value-right+left+3u;
    if((s32)sum<600){
        pointer=second_out;
        value+=3; D(pointer)=value;
    }else{
        eax=D(record+0x14)-left;
        right=eax+right-3u;
        pointer=second_out; D(pointer)=right;
    }
    index=(u32)(s32)(s16)W(self+0x20e);
    base=D(self+0x208); record=base+index*0x18cu;
    right=D(record+0x10);
    if((s32)right<=400){
        pointer=first_out; D(pointer)=right;
        return record;
    }
    right=D(record+0x18); left=D(self+8); eax=D(self);
    right=right-left+eax;
    pointer=first_out; D(pointer)=right;
    return pointer;
linked:
    left=D(self+0xc); right=D(self+4); value=D(record+0x14);
    sum=left-right+value;
    if((s32)sum<600){
        pointer=second_out; D(pointer)=value;
    }else{
        eax=D(record+0x1c)-left+right;
        if((s32)eax<=0) eax=0;
        pointer=second_out; D(pointer)=eax;
    }
    index=(u32)(s32)(s16)W(self+0x20e);
    base=D(self+0x208); record=base+index*0x18cu;
    other_index=(u32)(s32)(s16)W(record+0x7a);
    child=D(self+0x204)+other_index*0xbf1u;
    other_index=(u32)(s32)(s16)W(child+0x20e);
    right=D(base+other_index*0x18cu+0x18);
    eax=D(child);
    if((s32)right<(s32)eax){
        eax=D(record+0x18); pointer=first_out;
        eax+=3; D(pointer)=eax;
        return eax;
    }
    right=D(record+0x10); value=D(self+8);
    right-=value;
    eax=D(self);
    other=right+eax-3u;
    pointer=first_out; D(pointer)=other;
    return eax;
}
