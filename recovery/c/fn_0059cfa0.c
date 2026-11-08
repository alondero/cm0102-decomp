/* Original instruction recovery; native callees remain explicit boundaries.
   Preferred-image VA 0059CFA0, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Unsigned temporaries preserve wrapping address/arithmetic operations. Original
   calls remain native boundaries; their physical EAX projection is retained. */
#include "cmre.h"
typedef unsigned long long u64;
typedef signed long long s64;
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_0059ffb0(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_0059fae0(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_0059fe10(u32,u32);
extern u32 CM_THISCALL cm_fn_0059b630(u32,u32);
extern u32 CM_THISCALL cm_fn_004a4850(u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u32,u32,u32);
extern u64 __attribute__((regparm(2))) CM_CDECL cm_fn_0090cfc0(u32,u32,u32);

/* All callers here provide a nonzero small bound. Native 0090CFC0 does not
   consume entry EAX or EDX on that path. EDX:EAX is its physical projection;
   each continuation overwrites ECX before reading it. */
static u32 random_bound(u32 bound)
{
    return (u32)cm_fn_0090cfc0(0,0,bound);
}
static u32 signed_high(u32 a,u32 b)
{
    return (u32)((s64)(s32)a*(s64)(s32)b>>32);
}

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0059cfa0(u32 receiver,u32 club)
{
    u8 scratch[0x21c];
    u32 l=(u32)scratch;
    u32 eax,ecx=receiver,edx=0,ebx=0,ebp=receiver,esi,edi=club;
    u32 old,carry,temp,amount;
    long double fp,comparison;
    D(l+4)=ebp; D(l+0x10)=ebx; B(l+3)=(u8)ebx;
    eax=cm_fn_0053b900(edi);
    if(eax)goto done;
    esi=edi;
    if(edi)goto v59d036;
    eax=cm_fn_00944cff(0x009c8784,0,0,(void *)(l+0x11c),(void *)(l+0x1c));
    eax=cm_fn_0090d130();
    eax=cm_fn_00944c9f(0x00ae24d0,0x009870e8,eax,l+0x11c,l+0x1c,0x20b5);
    eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,ebx);
    D(0x00b67a34)=ebx;
    goto v59d066;
v59d036:
    eax=cm_fn_00540a50(edi,l+0x18,1);
    if(eax && D(l+0x18)==ebx)esi=eax;
    eax=D(esi); esi=eax*5u; esi*=9u; esi<<=3; esi-=eax;
    eax=D(ebp); esi+=eax;
    if(esi)goto v59d0cf;
v59d066:
    eax=cm_fn_00944cff(0x009c8784,0,0,(void *)(l+0x1c),(void *)(l+0x11c));
    eax=cm_fn_0090d130();
    eax=cm_fn_00944c9f(0x00ae24d0,0x009870e8,eax,l+0x1c,l+0x11c,0x168a);
    eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,ebx);
    D(0x00b67a34)=ebx;
    goto done;
v59d0cf:
    ecx=D(edi+0x69); eax=D(esi); D(l+8)=eax;
    if(ecx==ebx)goto v59daf2;
    edx=(edx&0xffffff00u)|B(edi+0x6d);
    if((u8)edx)goto v59d2aa;
    if(D(edi+0x57)==ebx)goto v59d2aa;
    if((s32)eax<=35000000)goto v59daf2;
    edx=D(0x00ae235c); ebp=D(0x00ae23b4); eax=0;
    if((s32)edx<=0)goto v59d12c;
v59d111:
    if(ebp==edi)goto v59d121;
    if(D(ebp+0x69)!=ecx)goto v59d121;
    ebx=(ebx&0xffffff00u)|B(ebp+0x6d);
    if((u8)ebx)goto v59d17a;
v59d121:
    eax++; ebp+=0x245;
    if((s32)eax<(s32)edx)goto v59d111;
v59d12c:
    B(edi+0x6d)=1; ebx=D(esi+0x8c); edx=D(esi+0x12c); eax=D(esi);
    ecx=20000000; ebx+=ecx; edx+=ecx;
    old=eax; eax+=0xfeced300u; carry=eax<old;
    D(esi+0x8c)=ebx; D(esi)=eax; ebp=D(esi+4); ebp+=0xffffffffu+carry;
    D(esi+0x12c)=edx; D(esi+4)=ebp;
    eax=D(edi+0x69); ecx=D(eax);
    eax=cm_fn_0059ffb0(edi,ecx,4); goto v59daf2;
v59d17a:
    if(!ebp)goto v59daf2;
    ebx=ebp; eax=cm_fn_00540a50(ebp,l+0x10,1);
    if(eax) {ecx=D(l+0x10); if(!ecx)ebx=eax;}
    ecx=D(ebx); ebx=D(l+4); eax=ecx*5u; eax*=9u; eax<<=3; eax-=ecx;
    ecx=D(ebx); eax+=ecx;
    if(eax)goto v59d206;
    B(edi+0x6d)=1; ebp=D(esi+0x8c); edx=D(esi+0x12c); ecx=20000000;
    ebp+=ecx; edx+=ecx; ecx=D(esi); D(esi+0x8c)=ebp;
    old=ecx; ecx+=0xfeced300u; carry=ecx<old;
    D(esi+0x12c)=edx; D(esi)=ecx; eax=D(esi+4); eax+=0xffffffffu+carry;
    D(esi+4)=eax; eax=D(edi+0x69); ecx=D(eax);
    eax=cm_fn_0059ffb0(edi,ecx,4); goto v59daf2;
v59d206:
    ecx=(ecx&0xffffff00u)|B(eax+0x165);
    if((u8)ecx==1)goto v59d21e;
    if((u8)ecx!=2)goto v59daf2;
    ebx=D(l+4);
v59d21e:
    B(edi+0x6d)=1; B(ebp+0x6d)=0; edx=D(esi+0x8c); ecx=20000000;
    edx+=ecx; D(esi+0x8c)=edx; edx=D(esi+0x12c); edx+=ecx; D(esi+0x12c)=edx;
    edx=D(esi); old=edx; edx+=0xfeced300u; carry=edx<old;
    D(esi)=edx; edx=D(esi+4); edx+=0xffffffffu+carry; D(esi+4)=edx;
    edx=D(eax+0xb4); edx+=ecx; D(eax+0xb4)=edx;
    edx=D(eax+0x154); edx+=ecx; D(eax+0x154)=edx;
    edx=D(eax); old=edx; edx+=ecx; carry=edx<old; D(eax)=edx;
    ecx=D(eax+4); ecx+=carry; D(eax+4)=ecx;
    edx=D(edi+0x69); eax=D(edx);
    eax=cm_fn_0059ffb0(edi,eax,4);
    ecx=D(edi+0x69); edx=D(ecx);
    eax=cm_fn_0059ffb0(ebp,edx,5); goto v59daf2;
v59d2aa:
    if(ecx==ebx)goto v59daf2;
    eax=(eax&0xffffff00u)|B(edi+0x6d);
    if(!(u8)eax)goto v59daf2;
    eax=D(edi+0x57); if(!eax)goto v59daf2;
    ebp=D(l+8);
    if((s32)ebp<=750000)goto v59d32a;
    eax=(eax&0xffffff00u)|B(esi+0x166);
    if((s8)eax<=10)goto v59d32a;
    eax=(u32)(s32)(s8)eax; edx=D(esi+0x12c); ebx=D(esi+0x8c);
    eax*=5u; eax*=5u; eax*=5u; eax*=5u; eax<<=4; ecx=eax;
    eax=10u; eax-=ecx; ecx=D(esi); edx+=eax; ebp-=eax; ebx+=eax;
    D(esi+0x12c)=edx; edx=(u32)((s32)eax>>31);
    old=ecx; ecx-=eax; carry=old<eax;
    D(l+8)=ebp; D(esi)=ecx; eax=D(esi+4); eax-=edx+carry;
    D(esi+0x8c)=ebx; D(esi+4)=eax;
v59d32a:
    eax=D(edi+0xcf); if(!eax)goto v59d549;
    if(W(edi+0x80)==0)goto v59d549;
    if((s8)B(edi+0x7f)>=20)goto v59d549;
    ecx=D(eax+0x69); if(!ecx)goto v59d549;
    eax=cm_fn_00600a50(edi,0,1,0,0); if(eax)goto v59d549;
    ecx=(u32)(s32)(s16)W(edi+0x80); eax=0x10624dd3;
    edx=signed_high(eax,ecx); edx=(u32)((s32)edx>>5);
    eax=edx; eax>>=31; edx+=eax;
    ecx=(u32)(s32)(s8)edx; edx=(u32)(s32)(s8)B(edi+0x7f); ecx+=8;
    if((s32)edx<=(s32)ecx)goto v59d549;
    if((s32)ebp<=2250000)goto v59d549;
    eax=random_bound(40); ecx=D(edi+0xcf); edx=D(ecx+0x69);
    ecx=(u32)(s32)(s8)B(edx+0x22);
    if((s32)ecx<=(s32)eax)goto v59d549;
    ecx=(ecx&0xffffff00u)|B(edi+0x7f); eax=(u32)(s32)(s8)ecx;
    eax*=5; eax*=5; eax*=5; eax*=5; eax*=5; eax<<=4; eax+=75000;
    if((s8)ecx<20) {ecx=(ecx&0xffffff00u)|(u8)(ecx+1);B(edi+0x7f)=(u8)ecx;}
    edx=D(edi); ebx=D(l+4); ecx=edx*5; ecx*=9; ecx<<=3; ecx-=edx;
    edx=D(ebx); ecx+=edx; edx=D(ecx+0x8c); edx+=eax; D(ecx+0x8c)=edx;
    edx=D(ecx+0x12c); edx+=eax; D(ecx+0x12c)=edx; edx=(u32)((s32)eax>>31);
    old=D(ecx); temp=old-eax; carry=old<eax; D(ecx)=temp;
    eax=D(ecx+4); eax-=edx+carry; D(ecx+4)=eax;
    ecx=(ecx&0xffffff00u)|B(edi+0x7f);
    if((s8)ecx>=20)goto v59d4d0;
    if((s16)W(edi+0x80)<=8250)goto v59d474;
    eax=(u32)(s32)(s8)ecx; eax*=5; eax*=5; eax*=5; eax*=5; edx=eax*5;
    eax=4500000; edx<<=6; eax-=edx; if((s32)eax<1000000)eax=1000000;
    B(edi+0x7f)=20; goto v59d50f;
v59d474:
    if((s8)ecx>=20)goto v59d4d0;
    eax=D(edi); edx=eax*5; edx*=9; edx<<=3; edx-=eax; eax=edx; eax+=D(ebx);
    temp=D(eax+4);
    if((s32)temp<0)goto v59d4d0;
    if((s32)temp>0)goto v59d49a;
    if(D(eax)<=15000000u)goto v59d4d0;
v59d49a:
    eax=(u32)(s32)(s8)ecx; eax*=5; eax*=5; eax*=5; eax*=5; eax*=5; eax<<=6;
    ecx=eax; eax=4500000; eax-=ecx; if((s32)eax<1000000)eax=1000000;
    B(edi+0x7f)=20; ecx=D(ebx); edx+=ecx; ecx=edx; goto v59d520;
v59d4d0:
    if((s8)ecx<16 && (s16)W(edi+0x80)>5250)goto v59d4f0;
    if((s8)ecx>=20)goto v59d54d;
    if((s16)W(edi+0x80)<=7250)goto v59d54d;
v59d4f0:
    eax=(u32)(s32)(s8)ecx; eax*=5; eax*=5; eax*=5; eax*=5; eax*=5; eax<<=4; eax+=75000;
    ecx=(ecx&0xffffff00u)|(u8)(ecx+1); B(edi+0x7f)=(u8)ecx;
v59d50f:
    edx=D(edi); ecx=edx*5; ecx*=9; ecx<<=3; ecx-=edx; edx=D(ebx); ecx+=edx;
v59d520:
    edx=D(ecx+0x8c); edx+=eax; D(ecx+0x8c)=edx;
    edx=D(ecx+0x12c); edx+=eax; D(ecx+0x12c)=edx;
    edx=(u32)((s32)eax>>31); old=D(ecx); temp=old-eax; carry=old<eax; D(ecx)=temp;
    eax=D(ecx+4); eax-=edx+carry; D(ecx+4)=eax; goto v59d54d;
v59d549:
    ebx=D(l+4);
v59d54d:
    eax=D(edi); ecx=D(0x00b67a44); edx=eax*9; eax+=edx*8; edx=D(ecx);
    eax=(eax&0xffffff00u)|B(eax+edx+2);
    temp=(u8)((u8)eax<<1); temp=(u32)(s32)((s8)temp>>7); B(l+0xf)=(u8)temp;
    if(!(u8)temp)goto v59d59d;
    eax=cm_fn_00600a50(edi,0,1,0,0); if(!eax)goto v59d59d;
    eax=D(edi); ecx=D(0x00b67a44); B(l+3)=1; ecx=D(ecx); edx=eax*9;
    eax+=edx*8; eax+=ecx+2; B(eax)&=0xbf;
v59d59d:
    eax=D(edi+0x69); if(!eax)goto v59d681;
    if((s32)ebp<=17500000)goto v59d617;
    if((s16)W(edi+0x80)<=7500)goto v59d617;
    if((s32)D(eax+0x3c)<=25000)goto v59d617;
    ecx=(ecx&0xffffff00u)|B(eax+0x4d); if((u8)ecx)goto v59d617;
    B(eax+0x4d)=1; edx=D(esi+0x8c); ecx=D(esi+0x12c); eax=1500000;
    edx+=eax; ecx+=eax; eax=D(esi); D(esi+0x8c)=edx;
    old=eax; eax+=0xffe91ca0u; carry=eax<old;
    D(esi+0x12c)=ecx; D(esi)=eax; ebp=D(esi+4); ebp+=0xffffffffu+carry; D(esi+4)=ebp;
    edx=D(edi+0x69); eax=D(edx); eax=cm_fn_0059ffb0(edi,eax,6); goto v59daf2;
v59d617:
    if(!eax)goto v59d681;
    if((s32)ebp<=75000000)goto v59d681;
    if((s16)W(edi+0x80)<=9250)goto v59d681;
    if((s32)D(eax+0x3c)<=57500)goto v59d681;
    if(B(edi+0x7f)!=20)goto v59d681;
    ecx=(ecx&0xffffff00u)|B(eax+0x4c); if((u8)ecx)goto v59d681;
    B(eax+0x4c)=1; ebx=D(esi+0x8c); edx=D(esi+0x12c); ecx=D(esi);
    eax=7500000; ebx+=eax; edx+=eax; old=ecx; ecx+=0xff8d8f20u; carry=ecx<old;
    D(esi+0x8c)=ebx; D(esi)=ecx; eax=D(esi+4); eax+=0xffffffffu+carry;
    D(esi+0x12c)=edx; D(esi+4)=eax; goto v59daf2;
v59d681:
    if((s32)ebp>5250000)goto v59d6cd;
    eax=cm_fn_00600a50(edi,0,1,0,0); if(!eax)goto v59d6ba;
    eax=(u32)(s32)(s16)W(edi+0x80); eax*=5; eax*=5; ecx=eax*5; edx=ebp+ecx*8;
    if((s32)edx>5250000)goto v59d6cd;
v59d6ba:
    eax=(eax&0xffffff00u)|B(l+3); if(!(u8)eax)goto v59daf2;
    amount=0xffffffffu; goto v59dae6;
v59d6cd:
    eax=D(edi+0x57); if(!eax)goto v59d70b;
    eax=D(eax); ecx=D(0x00adadfc); eax=D(ecx+eax*4); if(!eax)goto v59d70b;
    ecx=D(eax+8); if(!ecx)goto v59d70b;
    eax=cm_fn_004a4850(ecx,edi); if(!eax)goto v59d70b;
    ecx=(ecx&0xffffff00u)|B(eax+5); if(!(u8)ecx)goto v59d70b;
    eax=D(eax+6); ecx&=255; edx=(u32)((s32)eax>>31);
    eax=(u32)((s32)eax/(s32)ecx); D(l+0x10)=eax;
v59d70b:
    ebx=D(edi+0x69); ebp=D(ebx+0x3c); D(l+0x14)=ebp;
    if((s32)ebp>125000)goto v59daf2;
    eax=(eax&0xffffff00u)|B(l+3); if((u8)eax)goto v59d88e;
    edx=(u32)(s32)(s16)W(l+0x10); D(l+0x10)=edx;
    comparison=(long double)(s32)D(l+0x10);
    fp=(long double)(s32)D(l+0x14); fp*=CM_GLOBAL(volatile double,0x0096a720);
    if(fp<comparison)goto v59d88e;
    eax=(eax&0xffff0000u)|W(edi+0x80); ecx=eax+eax; edx=(u32)(s32)(s16)ecx;
    if((s32)edx<=(s32)ebp)goto v59d76c;
    if((s16)eax>4250)goto v59d88e;
v59d76c:
    eax=random_bound(20); ecx=D(edi+0x57); eax+=5; edx=(u32)(s32)(s16)W(ecx+0x69);
    if((s32)eax>=(s32)edx)goto v59daf2;
    ebx=D(edi+0x69); ebp=D(ebx+0x3c); ecx=D(ebx+0x40); ebp-=ecx;
    if((s32)ebp<=0)goto v59daf2;
    fp=(long double)(s32)D(l+8); fp*=CM_GLOBAL(volatile double,0x00968918);
    eax=(u32)(long long)fp; /* Native __ftol CALL 0059D7A5. */
    ecx=eax; eax=0x1b4e81b5; edx=signed_high(eax,ecx); edx=(u32)((s32)edx>>3);
    eax=edx; eax>>=31; edx+=eax; eax=0x10624dd3; ecx=(u32)(s32)(s16)edx;
    edx=signed_high(eax,ecx); edx=(u32)((s32)edx>>5); ecx=edx; ecx>>=31; edx+=ecx;
    eax=(u32)(s32)(s16)edx; eax*=5; eax*=5; eax*=5; eax<<=2;
    if((s32)eax>=(s32)ebp)eax=ebp;
    if((s16)eax<500)goto v59d875;
    ecx=(u32)(s32)(s16)eax; eax=D(ebx+0x3c); amount=ecx;
    edx=ecx+eax; D(ebx+0x3c)=edx; eax=D(edi+0x69); edx=D(eax+0x40); edx+=ecx; D(eax+0x40)=edx;
    eax=0x10624dd3; edx=signed_high(eax,ecx); edx=(u32)((s32)edx>>6);
    eax=edx; ebp=D(esi+0x12c); eax>>=31; ebx=D(esi); eax=edx+eax+1;
    edx=ecx; edx<<=4; eax*=5; edx-=ecx; ecx=D(l+4);
    eax*=5; edx*=5; eax*=5; eax*=5; eax*=5; eax<<=4; eax+=edx;
    edx=D(esi+0x8c); edx+=eax; ebp+=eax; D(esi+0x8c)=edx; D(esi+0x12c)=ebp;
    edx=(u32)((s32)eax>>31); old=ebx; ebx-=eax; carry=old<eax; D(esi)=ebx;
    ebp=D(esi+4); ebp-=edx+carry; D(esi+4)=ebp;
    eax=cm_fn_0059ffb0(edi,amount,3); goto v59daf2;
v59d875:
    if((s32)D(ebx+0x3c)>=30000)goto v59daf2;
    D(ebx+0x44)=D(ebx+0x44)+1500; goto v59dae4;
v59d88e:
    eax=(eax&0xffffff00u)|B(edi+0x64); temp=(u8)eax==1; eax=D(l+8);
    if(temp)goto v59d9af;
    if((s32)eax>20000000) {edx=21000;goto v59d8cd;}
    fp=(long double)(s32)D(l+8); fp*=CM_GLOBAL(volatile double,0x00968918);
    eax=(u32)(long long)fp; /* Native __ftol CALL 0059D8B5. */
    ecx=eax; eax=0x51eb851f; edx=signed_high(eax,ecx); edx=(u32)((s32)edx>>4);
    eax=edx; eax>>=31; edx+=eax;
v59d8cd:
    ecx=(u32)(s32)(s16)edx; eax=0x10624dd3; edx=signed_high(eax,ecx); edx=(u32)((s32)edx>>5);
    ecx=edx; ecx>>=31; edx+=ecx; eax=edx*5; ecx=eax*5; eax=D(ebx+0x44); ecx*=5; ecx<<=2;
    edx=(u32)(s32)(s16)ecx; edx+=ebp;
    if((s32)edx>(s32)eax) {eax-=ebp;ecx=(eax>>31)-1u;ecx&=eax;}
    eax=0x55555556; edx=signed_high(eax,D(ebx+0x44)); eax=edx; eax>>=31; edx+=eax;
    if((s32)edx<5000)edx=5000;
    eax=(u32)(s32)(s16)ecx; if((s32)eax>=(s32)edx)eax=edx;
    if((s16)eax<1000)goto v59d99e;
    ecx=(u32)(s32)(s16)eax; eax=0x10624dd3; amount=ecx; edx=ecx+ebp; D(ebx+0x3c)=edx;
    edx=signed_high(eax,ecx); edx=(u32)((s32)edx>>6); eax=edx; ebp=D(esi+0x12c); eax>>=31; ebx=D(esi);
    eax=edx+eax+1; eax*=5; eax*=5; edx=eax*5; eax=ecx+edx*8;
    edx=D(esi+0x8c); ecx=D(l+4); eax*=5; eax*=5; eax<<=1; edx+=eax; ebp+=eax;
    D(esi+0x8c)=edx; D(esi+0x12c)=ebp; edx=(u32)((s32)eax>>31);
    old=ebx; ebx-=eax; carry=old<eax; D(esi)=ebx; ebp=D(esi+4); ebp-=edx+carry; D(esi+4)=ebp;
    eax=cm_fn_0059ffb0(edi,amount,2); goto v59daf2;
v59d99e:
    if((s32)ebp>=30000)goto v59dad2;
    goto v59dac7;
v59d9af:
    if((s32)eax>20000000) {edx=16000;goto v59d9df;}
    fp=(long double)(s32)D(l+8); fp*=CM_GLOBAL(volatile double,0x00967880);
    eax=(u32)(long long)fp; /* Native __ftol CALL 0059D9C7. */
    ecx=eax; eax=0x10624dd3; edx=signed_high(eax,ecx); edx=(u32)((s32)edx>>3);
    eax=edx; eax>>=31; edx+=eax;
v59d9df:
    ecx=(u32)(s32)(s16)edx; eax=0x10624dd3; edx=signed_high(eax,ecx); edx=(u32)((s32)edx>>5);
    ecx=edx; ecx>>=31; edx+=ecx; eax=edx*5; ecx=eax*5; eax=D(ebx+0x44); ecx*=5; ecx<<=2;
    edx=(u32)(s32)(s16)ecx; edx+=ebp;
    if((s32)edx>(s32)eax) {eax-=ebp;ecx=(eax>>31)-1u;ecx&=eax;}
    eax=0x55555556; edx=signed_high(eax,D(ebx+0x44)); eax=edx; eax>>=31; edx+=eax;
    if((s32)edx<5000)edx=5000;
    eax=(u32)(s32)(s16)ecx; if((s32)eax>=(s32)edx)eax=edx;
    if((s16)eax<1000)goto v59dabf;
    ecx=(u32)(s32)(s16)eax; amount=ecx; edx=ecx+ebp; D(ebx+0x3c)=edx;
    eax=D(edi+0x69); edx=D(eax+0x40); edx+=ecx; D(eax+0x40)=edx;
    eax=0x10624dd3; edx=signed_high(eax,ecx); edx=(u32)((s32)edx>>6);
    eax=edx; ebp=D(esi+0x12c); eax>>=31; ebx=D(esi); edx=edx+eax+1;
    eax=edx; eax<<=4; eax-=edx; edx=D(esi+0x8c); eax*=5; eax*=5; eax<<=4; eax+=ecx;
    ecx=D(l+4); eax*=5; eax*=5; eax*=5; edx+=eax; ebp+=eax;
    D(esi+0x8c)=edx; D(esi+0x12c)=ebp; edx=(u32)((s32)eax>>31);
    old=ebx; ebx-=eax; carry=old<eax; D(esi)=ebx; ebp=D(esi+4); ebp-=edx+carry; D(esi+4)=ebp;
    eax=cm_fn_0059ffb0(edi,amount,1); goto v59daf2;
v59dabf:
    if((s32)ebp>=30000)goto v59dadc;
v59dac7:
    eax=D(ebx+0x44); eax+=1500; D(ebx+0x44)=eax;
v59dad2:
    eax=(eax&0xffffff00u)|B(l+3); if(!(u8)eax)goto v59daf2;
    goto v59dae4;
v59dadc:
    eax=(eax&0xffffff00u)|B(l+3); if(!(u8)eax)goto v59daf2;
v59dae4:
    amount=2;
v59dae6:
    ecx=D(l+4); eax=cm_fn_0059fae0(edi,2,amount);
v59daf2:
    eax=D(edi); ecx=eax*9; edx=eax+ecx*8; eax=D(0x00b67a44); ecx=D(eax);
    edx=(edx&0xffffff00u)|B(edx+ecx+2); temp=(u32)(s32)((s8)edx>>7); B(l+0xf)=(u8)temp;
    if(!(u8)temp)goto v59db6f;
    ecx=(u32)(s32)(s16)W(edi+0x80); eax=500; eax-=ecx; eax*=5; eax*=5; edx=eax*5; eax=D(l+8); edx<<=2;
    if((s32)eax<=(s32)edx)goto v59db4a;
    ebx=D(l+4); eax=cm_fn_0059b630(ebx,edi); eax=cm_fn_0059ffb0(edi,0xffffffffu,8); goto v59db58;
v59db4a:
    ecx=D(l+4); eax=cm_fn_0059fae0(edi,8,0xffffffffu);
v59db58:
    eax=D(edi); ecx=D(0x00b67a44); ecx=D(ecx); edx=eax*9; eax+=edx*8; eax+=ecx+2; B(eax)&=0x7f;
v59db6f:
    ecx=(ecx&0xffff0000u)|W(edi+0x80); ebp=(u32)(s32)(s16)ecx;
    eax=ebp*5; eax*=5; eax*=5; eax<<=6; if((s32)eax<500000)eax=500000;
    ebx=D(esi+4); edx=(u32)((s32)eax>>31);
    if((s32)ebx<(s32)edx)goto v59dc05;
    if((s32)ebx>(s32)edx)goto v59dba0;
    if(D(esi)<=eax)goto v59dc05;
v59dba0:
    eax=(eax&0xffffff00u)|B(edi+0x82); if(!(u8)eax)goto v59dc05;
    if((s16)ecx>=4000)goto v59dbe6;
    eax=random_bound(501); edx=(u32)(s32)(s16)W(edi+0x80); ecx=eax; ecx+=1500; ecx*=edx;
    if((s32)ecx<50000)ecx=50000;
    goto v59dcf2;
v59dbe6:
    eax=random_bound(501); ecx=eax; eax=(u32)(s32)(s16)W(edi+0x80); ecx+=2000; ecx*=eax; goto v59dcf2;
v59dc05:
    eax=D(edi+0xbf); if(!eax)goto v59dc46;
    edx=D(eax+0x69); D(l+0x14)=edx; if(!edx)goto v59dc46;
    edx=(edx&0xffff0000u)|(u16)(s16)(s8)B(eax+0x59); eax=7; eax-=edx;
    edx=D(l+0x14); edx=(edx&0xffff0000u)|(u16)(s16)(s8)B(edx+0x20);
    eax*=3; eax-=edx; ecx+=eax*4; eax+=ecx;
    if((s16)eax>=500)goto v59dc48;
    eax=500; goto v59dc48;
v59dc46:
    eax=ecx;
v59dc48:
    ecx=(u32)(s32)(s16)eax; eax=ecx; eax<<=4; eax-=ecx; eax*=5; eax*=5; eax<<=4;
    edx=(u32)((s32)eax>>31);
    if((s32)ebx<(s32)edx)goto v59dcae;
    if((s32)ebx>(s32)edx)goto v59dc66;
    if(D(esi)<=eax)goto v59dcae;
v59dc66:
    if((s16)W(edi+0x80)>=4000)goto v59dc92;
    eax=random_bound(501); edx=(u32)(s32)(s16)W(edi+0x80); ecx=eax; ecx+=1000; ecx*=edx; goto v59dcf2;
v59dc92:
    eax=random_bound(501); ecx=eax; eax=(u32)(s32)(s16)W(edi+0x80); ecx+=1500; ecx*=eax; goto v59dcf2;
v59dcae:
    eax=ecx; eax<<=4; eax-=ecx; eax*=5; eax*=5; eax<<=3; edx=(u32)((s32)eax>>31);
    if((s32)ebx<(s32)edx)goto done;
    if((s32)ebx>(s32)edx)goto v59dcc9;
    if(D(esi)<=eax)goto done;
v59dcc9:
    if((s16)W(edi+0x80)>=4000)goto v59dce3;
    eax=ebp*5; eax*=5; ecx=eax*5; ecx<<=2; goto v59dcf2;
v59dce3:
    eax=ebp; eax<<=4; eax-=ebp; eax*=5; ecx=eax*5; ecx<<=1;
v59dcf2:
    edx=D(esi+0x94); amount=ecx; edx+=ecx; D(esi+0x94)=edx;
    eax=D(esi+0x134); eax+=ecx; D(esi+0x134)=eax;
    ebp=D(esi); eax=ecx; ecx=D(l+4); edx=(u32)((s32)eax>>31);
    old=ebp; ebp-=eax; carry=old<eax; D(esi)=ebp;
    ebx=D(esi+4); ebx-=edx+carry; D(esi+4)=ebx;
    eax=cm_fn_0059fe10(edi,amount);
done:
    return eax;
}
