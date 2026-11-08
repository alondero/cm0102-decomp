#include "cmre.h"
/* Preferred-image VA 007ae9c0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All callees remain original-code boundaries. Scratch offsets are relative to
   the original post-SUB ESP, before the four saved-register pushes. */
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_006181d0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00616f20(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
/* EDX:EAX also transports the actual caller-clobbered high half of DX used
   immediately after this call; it is not an inferred semantic 64-bit value. */
extern unsigned long long CM_THISCALL cm_fn_0048d500(u32,u32);
extern u32 CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32,u32,u32);
extern void CM_CDECL cm_fn_009452ca(u32);
extern void CM_THISCALL cm_fn_007b0050(u32);
extern void CM_THISCALL cm_fn_00806450(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32,a)

u32 CM_THISCALL cm_fn_007ae9c0(u32 self,u32 nation,u32 region,
                              u32 competition,u32 youth,u32 opposition)
{
    u8 scratch[0x2dc];
    u32 local=(u32)scratch, text=local+0x14u;
    u32 index,person,first,second,third,date,result,table,offset;
    u32 eax,edx,ecx,esi,edi=0,ebx,source,count,length,word;
    u32 minus=0xffffffffu,output,assert_first,assert_second,line;
    unsigned long long pair;
    index=D(self+0x180u);
    if ((s32)index < 0) return 0;
    third=D(self+0x17cu);
    date=self+0x184u;
    second=D(self+0x178u);
    first=D(self+0x174u);
    table=G(0x00ae23bc);
    person=table+index*110u;
    result=cm_fn_0053e990(person,first,second,third,date);
    if (result==0) return 0;
    index=D(self+0x180u);
    table=G(0x00ae23bc);
    person=table+index*110u;
    if (person==0) return 0;
    if (B(self+0xau)!=4) {
        cm_fn_00944cff(0x00a701b0,0,0,local+0x1dcu,local+0xdcu);
        assert_first=local+0x1dcu;assert_second=local+0xdcu;line=0x18e;
        goto assertion;
    }
    ebx=nation;
    if ((s32)ebx>=0 && (s32)ebx<(s32)G(0x00ae2350)) {
        table=G(0x00ae23a8);offset=ebx*290u;
        cm_fn_006181d0(table+offset,text,200,1);
        table=G(0x00ae23a8);
        eax=(u32)(s32)(s8)B(table+offset+0x37u);
        cm_fn_0066f4e0(self+0xbu,0x00a70240,eax,eax,text);
        D(self+0x18cu)=ebx;
        B(self+0x194u)=0xff;
        D(self+0x195u)=minus;
        eax=cm_fn_0090cfc0(3);
        eax=cm_fn_0054c5b0(0x00ae2c90,local,eax+1u);
        output=local+8u;
        goto assign_date;
    }
    ebx=region;
    if ((s8)ebx>=1 && (s8)ebx<=14) {
        cm_fn_00616f20(ebx,text,200,1);
        cm_fn_0066f4e0(self+0xbu,0x00a70220,100,100,text);
        D(self+0x18cu)=minus;
        B(self+0x194u)=(u8)ebx;
        D(self+0x195u)=minus;
        eax=cm_fn_0090cfc0(3);
        eax=cm_fn_0054c5b0(0x00ae2c90,local+8u,eax+1u);
        output=local;
        goto assign_date;
    }
    ebx=competition;
    if ((s32)ebx>=0 && (s32)ebx<(s32)G(0x00ae2378)) {
        table=G(0x00ae23d0);
        edx=table+ebx*107u;
        D(local)=text;
        source=edx+4u;
        count=0xffffffffu;edi=source;
        do { --count;word=B(edi);++edi; } while(word!=0 && count!=0);
        length=~count;source=edi-length;esi=source;edi=D(local);
        count=length>>2;
        while(count!=0) {word=D(esi);D(edi)=word;esi+=4;edi+=4;--count;}
        count=length&3;
        while(count!=0) {word=B(esi);B(edi)=(u8)word;++esi;++edi;--count;}
        eax=(u32)(s32)(s8)B(edx+0x37u);
        cm_fn_0066f4e0(self+0xbu,0x00a70200,eax,eax,text);
        D(self+0x18cu)=minus;
        B(self+0x194u)=0xff;
        D(self+0x195u)=ebx;
        ecx=G(0x00adadfc);
        ecx=D(ecx+ebx*4u);
        pair=cm_fn_0048d500(ecx,local+0x10u);
        esi=(u32)pair;
        /* Preserve the partial DX/AX inputs to the two date constructors. */
        edx=(u32)(pair>>32)&0xffff0000u;
        edx|=W(esi+0xeu);
        eax=(esi&0xffff0000u)|W(esi+0xcu);
        eax=cm_fn_00549ef0(local+8u,eax,edx);
        edi=self+0x1b6u;
        cm_fn_00417c10(edi,local,eax);
        eax=D(local+0x10u);
        edx=eax*5u;
        /* 417c10 restores its incoming ECX (EDI) before returning. */
        ecx=(edi&0xffff0000u)|W(esi+edx*4u-6u);
        eax=esi+edx*4u;
        edx=(edx&0xffff0000u)|W(eax-8u);
        eax=cm_fn_00549ef0(local+8u,edx,ecx);
        cm_fn_00417c10(self+0x1beu,local,eax);
        cm_fn_009452ca(esi);
        eax=cm_fn_0090cfc0(3);
        eax=cm_fn_0054c5b0(0x00ae2c90,local+8u,eax+1u);
        esi=self+0x1adu;
        cm_fn_00417c10(esi,local,eax);
        eax=cm_fn_0054c8e0(edi,local+8u,2);
        ecx=D(eax);edx=D(eax+4u);
        word=W(esi+2u);
        D(local)=ecx;D(local+4u)=edx;
        edx=W(local+2u);
        if ((u16)edx>(u16)word || ((u16)edx==(u16)word && (s16)ecx>(s16)W(esi))) {
            eax=cm_fn_0054c8e0(edi,local+8u,2);
            cm_fn_00417c10(esi,local,eax);
        }
        B(self+0x1b5u)=0;
        goto update;
    }
    ebx=youth;
    if (ebx!=edi) {
        cm_fn_0066f4e0(self+0xbu,0x00a701f0);
        D(self+0x18cu)=minus;
        B(self+0x194u)=0xff;
        D(self+0x195u)=minus;
        D(self+0x199u)=ebx;
        eax=cm_fn_0090cfc0(3);
        eax=cm_fn_0054c5b0(0x00ae2c90,local+8u,eax+1u);
        output=local;
        goto assign_date;
    }
    ebx=opposition;
    if (ebx!=edi) {
        cm_fn_0066f4e0(self+0xbu,0x00a701e0);
        D(self+0x18cu)=minus;
        B(self+0x194u)=0xff;
        D(self+0x195u)=minus;
        D(self+0x19du)=ebx;
        eax=cm_fn_0054c5b0(0x00ae2c90,local+8u,1);
        cm_fn_00417c10(self+0x1adu,local,eax);
        B(self+0x1b5u)=1;
        goto update;
    }
    cm_fn_00944cff(0x00a701b0,0,0,local+0xdcu,local+0x1dcu);
    assert_first=local+0xdcu;assert_second=local+0x1dcu;line=0x21d;
assertion:
    eax=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,eax,assert_first,assert_second,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,edi);
    G(0x00b67a34)=edi;
    return 0;
assign_date:
    cm_fn_00417c10(self+0x1adu,output,eax);
    B(self+0x1b5u)=0;
update:
    cm_fn_007b0050(self);
    cm_fn_00806450(self+0x1fau);
    return 1;
}
