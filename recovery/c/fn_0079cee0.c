#include "cmre.h"
/* Preferred-image VA0079cee0, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical ECX receiver/one stack word/RET4, incidental incoming EAX.
   stdcall+regparm3 carries EAX/unusedEDX/ECX then the original stack word.
   Scratch208 countL0/startL4/diagnosticsL8,L108. Stale count reads before
   selection are never used: a match or fallback supplies the count.
   Preserve full EAX partial-AL returns and the reset loop's initial +17. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_004b0700(u32);
extern u32 CM_CDECL cm_fn_00539c60(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0079cee0(u32 eax_in,u32 edx_in,u32 self,u32 competition)
{
    u8 scratch[0x208];
    u32 l=(u32)scratch,value=eax_in,id,base,group,remaining,count,start,offset;
    u32 pointer,first,second,word,reset,selected;
    s32 difference;
    u8 type,found,byte;
    (void)edx_in;
    if (competition==0) return value&0xffffff00u;
    id=D(competition);
    base=D(0x00adadfc);
    value=D(base+id*4);
    if (value==0) return value;
    type=B(value+0x42);
    value=(value&0xffffff00u)|type;
    if (type==4 || type==2) return value&0xffffff00u;
    value=cm_fn_004b0700(id);
    if (value==0) return value;
    value=D(competition+0x5d);
    if (value!=0) value=D(value);
    else {
        first=D(0x00ae2350);
        value=cm_fn_0090cfc0(first);
    }
    selected=value;
    base=D(self+0xc);
    group=base+selected*258;
    remaining=(u32)(s32)(s16)W(group+0x10);
    found=0;
    value=base;
    if ((s32)remaining>0) {
        (void)D(l);
        value=group+0x12;
        do {
            if (found==0) {
                id=D(value+0xc);
                if (id==D(competition)) {
                    first=D(value);
                    found=1;
                    D(l+4)=first;
                    second=D(value+4);
                    D(l)=second;
                } else (void)D(l);
            }
            value+=0x10;
            --remaining;
        } while (remaining!=0);
    }
    if (found==0) {
        start=D(group+8);
        count=D(group+4);
        D(l+4)=start;
    } else {
        start=D(l+4);
        count=D(l);
    }
    if ((s32)count<=0) return (value&0xffffff00u)|1;
    offset=start*43;
    D(l)=count;
    do {
        base=D(0x00ae23e8);
        pointer=base+offset;
        if (pointer==0) {
            cm_fn_00944cff(0x00a6a22c,0,0,l+0x108,l+8);
            value=cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,l+0x108,l+8,0xa61);
            cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
            D(0x00b67a34)=0;
        } else {
            first=(u32)(s32)(s16)W(pointer+0xc);
            second=(u32)(s32)(s16)W(0x00ae2c90);
            difference=(s32)first-(s32)second;
            byte=B(0x00ae2c92);
            type=B(pointer+0xe);
            byte=(u8)(byte-type);
            if (difference>0) byte=(u8)(byte-1);
            if ((s8)byte>45) value=D(pointer+0x16);
            else {
                if ((s8)byte<=40) goto next;
                value=cm_fn_0090cfc0(3);
                if (value!=0) goto next;
                base=D(0x00ae23e8);
                value=D(base+offset+0x16);
            }
            value=cm_fn_00539c60(value);
            base=D(0x00ae23e8);
            D(base+offset+4)=value;
            base=D(0x00ae23e8);
            value=D(base+offset+0x16);
            value=cm_fn_00539c60(value);
            base=D(0x00ae23e8);
            D(base+offset+8)=value;
            value=cm_fn_0090cfc0(10);
            if (value!=0) {
                value=cm_fn_0090cfc0(16);
                base=D(0x00ae23e8);
                B(base+offset+0x25)=(u8)(value+5);
            } else {
                first=cm_fn_0090cfc0(11);
                second=cm_fn_0090cfc0(11);
                base=D(0x00ae23e8);
                B(base+offset+0x25)=(u8)(first+second);
            }
            value=cm_fn_0090cfc0(5);
            word=W(0x00ae2c92);
            base=D(0x00ae23e8);
            W(base+offset+0xe)=(u16)(value+(u16)(word-27));
            value=cm_fn_0090cfc0(0x154);
            base=D(0x00ae23e8);
            W(base+offset+0xc)=(u16)value;
        }
next:
        value=D(l);
        offset+=43;
        --value;
        D(l)=value;
    } while (value!=0);
    start=D(l+4);
    if ((s32)count>0) {
        value=start*17;
        reset=count;
        do {
            base=D(self);
            value+=17;
            --reset;
            B(base+value-2)=0;
            base=D(self); W(base+value-9)=0;
            base=D(self); W(base+value-7)=0;
            base=D(self); W(base+value-5)=0;
            base=D(self); D(base+value-0xd)=0;
        } while (reset!=0);
    }
    return (value&0xffffff00u)|1;
}
