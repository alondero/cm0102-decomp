#include "cmre.h"
/* Preferred-image VA005ee150/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS21/RET84/full EAX=0 on normal return. Keep actual argument-slot
   writes/copy boundaries, cached versus late values, WORD colors, repeated
   measurement call, signed division (including zero faults), and scan/copy
   access widths. Original helpers remain boundaries. */
#define D(a) (*(volatile u32 *)(a))
typedef u16 word_5ee150 __attribute__((may_alias));
#define W(a) (*(volatile word_5ee150 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_0090ba90(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e5ad0(u32,u32);
extern u32 CM_CDECL cm_fn_005e5cd0(u32);

u32 CM_THISCALL cm_fn_005ee150(u32 self,
    volatile u32 p1,volatile u32 p2,volatile u32 p3,volatile u32 p4,
    volatile u32 p5,volatile u32 p6,volatile u32 p7,volatile u32 p8,
    volatile u32 p9,volatile u32 p10,volatile u32 p11,volatile u32 p12,
    volatile u32 p13,volatile u32 p14,volatile u32 p15,volatile u32 p16,
    volatile u32 p17,volatile u32 p18,volatile u32 p19,volatile u32 p20,
    volatile u32 p21)
{
    u32 eax=p1,initial=p5,other=p2,flags=p11,upper,first,height;
    u32 offset,base,row,pointer,text,font,measured,old_value,table,last;
    u32 ordinal,word2,word,source,destination,remaining,bytes,scan;
    u32 arg4,arg5,arg8,arg9,arg11;
    volatile s32 divisor;
    u8 byte;
    D(self)=eax;upper=p7;D(self+4)=other;D(self+0x184)=0;
    if((s32)initial>(s32)upper && ((flags&0x400u)==0 || (flags&0x4000u)!=0))
        cm_fn_0090ba90((u32)&p5,(u32)&p7,4);
    eax=p6;other=p8;
    if((s32)eax>(s32)other && ((flags&0x400u)==0 || (flags&0x4000u)!=0)){
        cm_fn_0090ba90((u32)&p6,(u32)&p8,4);eax=p6;
    }
    first=p5;other=p7;p5=(s32)first>0?first:0;
    height=(s32)eax>0?eax:0;p6=height;
    if((s32)other>=799) p7=799;
    other=p8;if((s32)other>=599) p8=599;
    word=W((u32)&p21);
    if(word!=0xffffu){
        offset=(u32)(s32)(s16)(u16)word*3057u;base=D(self);
        if(W(base+offset+0x20e)!=0xffffu){
            text=p17;if(text==0) text=0x00ad9c64;
            pointer=self+0x80;cm_fn_00944c9f(pointer,0x009cdc58,text);
            font=p15;measured=cm_fn_005e5ad0(font,pointer)+15u;
            if((s32)measured>125) measured=cm_fn_005e5ad0(font,pointer)+15u;
            else measured=125;
            base=D(self);row=base+offset;old_value=D(row+8);
            if((s32)measured>(s32)old_value) D(row+8)=measured;
            base=D(self);byte=B(base+offset+0xbab);row=base+offset;
            if(byte==1) p9=0;
            else{
                table=D(self+4);last=0;W(row+0xb78)=0;
                for(;;){
                    word=W(row+0xb78);
                    if((s16)(u16)word>=(s16)W(row+0xb74)) break;
                    first=(u32)(s32)(s16)(u16)word;
                    other=W(row+first*2u+0x212);W(row+0xb78)=(u16)(word+1u);
                    if(other==0xffffu) break;
                    first=(u32)(s32)(s16)(u16)other;
                    last=D(table+first*396u+0x20);
                }
                eax=last+1u;divisor=(s32)B(row+0xbab);
                p9=(u32)((s32)eax%divisor);
            }
            base=D(self);font=p15;ordinal=B(base+offset+0xbac);
            old_value=D(base+offset+0xc);eax=cm_fn_005e5cd0(font);
            base=D(self);D(base+offset+0xc)=eax+old_value+6u;
            if((ordinal&1u)!=0) D(self+0x184)=1;
            base=D(self);B(base+offset+0xbac)=(u8)(ordinal+1u);
            height=p6;goto fields;
        }
    }
    text=p17;if(text==0) text=0x00ad9c64;
    scan=text;remaining=0xffffffffu;
    while(remaining!=0){byte=B(scan);++scan;--remaining;if(byte==0) break;}
    bytes=~remaining;source=scan-bytes;destination=self+0x80;ordinal=p10;
    remaining=bytes>>2;
    while(remaining!=0){first=D(source);D(destination)=first;source+=4;destination+=4;--remaining;}
    remaining=bytes&3u;
    while(remaining!=0){byte=B(source);B(destination)=byte;++source;++destination;--remaining;}
fields:
    word=W((u32)&p3);arg4=p4;W(self+0x70)=(u16)word;
    arg5=p5;D(self+0x14)=height;D(self+0x30)=height;
    word=W((u32)&p12);D(self+0xc)=arg4;arg8=p8;
    D(self+0x10)=arg5;D(self+0x28)=arg5;arg11=p11;
    W(self+0x72)=(u16)word;word=W((u32)&p15);
    upper=p7;arg9=p9;D(self+0x1c)=arg8;D(self+0x34)=arg8;
    word2=W((u32)&p13);D(self+0x38)=arg11;other=p14;
    W(self+0x76)=(u16)word;first=p19;W(self+0x74)=(u16)word2;
    word=W((u32)&p16);D(self+0x3c)=other;other=p18;
    D(self+8)=first;first=p20;W(self+0x78)=(u16)word;
    D(self+0x40)=other;D(self+0x48)=first;word=W((u32)&p21);
    D(self+0x18)=upper;D(self+0x20)=arg9;D(self+0x24)=ordinal;D(self+0x2c)=upper;
    text=p17;D(self+0x44)=0xffffffffu;D(self+0x180)=0;D(self+0x4c)=0;
    W(self+0x7a)=(u16)word;W(self+0x7c)=0xffffu;D(self+0x188)=0;
    other=D(0x00b67a08);D(self+0x50)=other;byte=B((u32)&p4);
    W(self+0x7e)=0xffffu;D(self+0x5c)=0;
    D(self+0x54)=(text!=0 && (byte&8u)!=0)?text:0;
    if((byte&8u)!=0 && (byte&0x10u)!=0){
        scan=self+0x80;remaining=0xffffffffu;
        while(remaining!=0){byte=B(scan);++scan;--remaining;if(byte==0) break;}
        D(self+0x44)=~remaining-1u;
    }
    return 0;
}
