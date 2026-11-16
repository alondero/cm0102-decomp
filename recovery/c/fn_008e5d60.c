/* Preferred-image VA008E5D60, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, six DWORD homes, RET24/full EAX. Original dependencies are
   explicit boundaries. L/BL/WL retain overlapping local widths and native
   post-prologue ESP offsets. Uninitialized locals stay uninitialized.

   __ftol contract (the held evidence question). The original body executes
   exactly 28 `call 0x00945640` sites; each is lowered here as one
   token-selected conversion macro, in the listing's x87 operation order on the
   listing's own constants (CM_GLOBAL double/float), with the full 64-bit
   truncation kept in a local `s64` and split into the physical EDX:EAX result.
   No site uses a 32-bit saturating `(int)` conversion and no site shares or
   reuses another site's expression, so a +1 / zero control injected at any one
   token changes exactly that conversion and nothing else:

     24 months, 008e682e..008e684a -> 008e684b 008e685f 008e6874 (SCALE)
     18 months, 008e6887..008e68a3 -> 008e68a4 008e68b8 008e68cd (SCALE)
     12 months, 008e68e0..008e68fc -> 008e68fd 008e6911 008e6926 (SCALE)
      6 months, 008e6936..008e6950 -> 008e6951 008e6965 008e697a (SCALE)
     008e6ae1 bonus -> 008e6aff   008e6b0d bonus -> 008e6b2b
     008e6b71 bonus -> 008e6bbb   008e6c70 bound -> 008e6c79
     008e6ca2 bonus -> 008e6cec   008e6d9e bonus -> 008e6ddd
     008e6e8d offer   -> 008e6eb9 008e6ebe bonus -> 008e6f04
     008e6fcd bonus -> 008e700c   008e71f5 offer  -> 008e71fe
     008e71c2 bonus -> 008e7249   008e727a bonus -> 008e72b9
     008e7377 ratio -> 008e7387   008e73e5 share -> 008e7422
     008e742c top-up -> 008e7441  008e7457 floor -> 008e7474

   INCOMPLETE (recorded, not claimed): the +1/zero control on 008e684b is not
   rejected by the 24 provenance captures on file, i.e. none of them reach the
   scaling path, so the source-level lowering above is not yet evidenced by an
   observed original conversion value. That gap needs original inputs that
   reach the scaling/bonus conversions; it is an input-provenance gap, not a
   remaining ambiguity in this source.

   Boundaries reviewed at this revision: the cm_fn_0068d0d0 FASTCALL prefix
   keeps ECX (the copied record) and EDX (the caller's *player prefix, high 24
   bits from the original `mov edx,[ecx]` with only DL overwritten at
   008e629d) as explicit parameters; cm_fn_00944cf1 keeps EAX/EDX/ECX untouched
   with the size on the stack, as push 0xe/add esp,4 shows; the 008ced40 call
   forwards arg2 (the caller home at [esp+0x274]) as arg1 while re-reading the
   argument homes after the intervening pushes. */
#include "cmre.h"
typedef unsigned long long u64;
typedef long long s64;
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define F(a) CM_GLOBAL(volatile float,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
#define P(off) (l+(off)-0x10u)
#define L(off) D(P(off))
#define BL(off) B(P(off))
#define WL(off) W(P(off))
#define RANDOM(n) do {u32 bound=(n);pair=cm_fn_0090cfc0(eax,edx,bound);eax=(u32)pair;edx=(u32)(pair>>32);}while(0)
#define CONVERT(site,expr) do {volatile s64 converted=(s64)(expr);eax=(u32)converted;edx=(u32)((u64)converted>>32);}while(0)
#define ADD_AL(n) eax=(eax&0xffffff00u)|(u8)((u8)eax+(n))
#define MUL_AL(n) eax=(eax&0xffff0000u)|(u16)((s16)(s8)eax*(s16)(s8)(n))
#define BONUS(site,off) do { \
    edx=(u32)(s32)(s8)B(self+0x39u);L(off)=eax;fp=(long double)(s32)L(off); \
    L(off)=edx;fp*=(s32)D(self+0x1bu);other=(long double)(s32)L(off); \
    other*=Q(0x0096a670u);other=(long double)Q(0x009688a8u)-other;fp/=other; \
    CONVERT(site,fp);D(self+0x35u)=eax; \
}while(0)
#define SCALE(s1,s2,s3,mult) do { \
    fp=(long double)(s32)L(0x1c);B(self+0x40u)=months;fp*=Q(mult);CONVERT(s1,fp); \
    fp=(long double)(s32)D(self+0x1bu);ebx=eax;L(0x1c)=ebx;fp*=Q(mult);CONVERT(s2,fp); \
    fp=(long double)(s32)value_home;D(self+0x1bu)=eax;fp*=Q(mult);CONVERT(s3,fp); \
    edi=eax;value_home=edi; \
}while(0)
extern u64 CM_CDECL __attribute__((regparm(2))) cm_fn_0090cfc0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32,u32);
extern u64 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008cdd80(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u64 CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,u32);
extern u64 CM_CDECL __attribute__((regparm(3))) cm_fn_00944cf1(u32,u32,u32,u32);
extern u64 CM_CDECL cm_fn_00944c94(u32);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32,u32,u32,u32);
extern u64 CM_THISCALL cm_fn_008c5e40(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008c61e0(u32,u32);
extern u32 CM_THISCALL cm_fn_005a0590(u32,u32);
extern s64 CM_THISCALL cm_fn_00598800(u32,u32,u32,u32,u32,u32);
extern u32 CM_STDCALL cm_fn_0084b6f0(u32,u32,u32,u32);
extern u32 CM_STDCALL cm_fn_008d0a70(u32);
extern u32 CM_THISCALL cm_fn_0085e410(u32);
extern u32 CM_THISCALL cm_fn_004cda70(u32,u32,u32,u32,s32,double,u32);
extern u32 CM_THISCALL cm_fn_004d42d0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_005444a0(u32,u32);
extern u32 CM_THISCALL cm_fn_004d1600(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00867b50(u32);
extern u32 CM_THISCALL cm_fn_00840d80(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004d0c60(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005986b0(u32);
extern u32 CM_CDECL cm_fn_005409b0(u32);
extern u32 CM_THISCALL cm_fn_008cc930(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008e7a70(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008e8330(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053b620(u32);
extern u32 CM_THISCALL cm_fn_00868210(u32,u32,u32,u32);
extern u32 CM_STDCALL cm_fn_008d0780(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008ced40(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL __attribute__((target("no-sse3")))
cm_fn_008e5d60(u32 self,volatile u32 player_home,volatile u32 club_home,
              volatile u32 offer_home,volatile u32 value_home,
              volatile u32 mode_home,volatile u32 output_home)
{
    volatile struct {u32 previous,handler,state;} frame;
    u8 scratch[0x24c];u32 l=(u32)scratch;
    u32 eax,edx=0,ecx,ebx,ebp,edi=0,a,b,target;
    u8 months,al,bl,cl;
    u64 pair;
    long double fp,other,third;
    frame.state=0xffffffffu;frame.handler=0x00964982u;
    frame.previous=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
    ebp=player_home;L(0x10)=0;L(0x14)=0xffffffffu;L(0x2c)=0;
    if(!ebp || !club_home)goto invalid;
    eax=D(ebp+0x61u);ebx=offer_home;
    if(!eax || !ebx || (B(ebx+0x24u)&0x1eu)>=8)goto not_interested;
    RANDOM(3000);ecx=D(ebp+0x61u);eax+=4000u;edx=(u32)(s32)(s16)W(ecx+0xbu);
    if((s32)edx<=(s32)eax)goto not_interested;
    eax=cm_fn_004cf5a0(D(0x00ae1050u),ebp);D(0x00ae1080u)=eax;
    if(!eax)goto not_interested;
    eax=D(eax+4u);edx=D(0x00ae23b4u);eax=edx+eax*581u;
    if(!eax)goto not_interested;
    eax=cm_fn_004cf5a0(D(0x00ae1050u),ebp);D(0x00ae1080u)=eax;
    if(eax){eax=D(eax+4u);edx=D(0x00ae23b4u);eax=edx+eax*581u;}
    ecx=club_home;eax=D(eax+0x57u);
    if(eax!=D(ecx+0x57u))goto not_interested;
    RANDOM(20);if(eax)goto not_interested;
    B(self+0x2du)=1;goto initialize;
not_interested:
    B(self+0x2du)=0;
initialize:
    edx=club_home;D(self)=0;D(self+4u)=0;eax=D(edx);D(self+8u)=eax;
    ecx=D(ebp);D(self+0xcu)=ecx;
    eax=cm_fn_004cf5a0(D(0x00ae1050u),ebp);edx=(edx&0xffffff00u)|(u8)mode_home;
    L(0x34)=eax;B(self+0x2fu)=(u8)edx;
    eax=ebx?(u32)(s32)(s8)B(ebx):11u;B(self+0x11u)=(u8)eax;
    al=B(self+0x30u);D(self+0x28u)=0xffffffffu;D(self+0x24u)=0xffffffffu;
    eax=D(self+0x31u);al&=0xf0u;eax&=0xfffffff0u;
    B(self+0x1fu)=0;D(self+0x31u)=eax;
    eax=output_home;D(self+0x35u)=0;B(self+0x39u)=10;D(self+0x3au)=0;
    B(self+0x3eu)=10;B(self+0x3fu)=0;B(self+0x40u)=0;B(self+0x2cu)=0;
    B(self+0x10u)=0;B(self+0x2eu)=0;B(self+0x30u)=al;D(self+0x1bu)=0;L(0x20)=0;
    if(eax)D(eax)=0;
    if(!cm_fn_00600a50(club_home,0,1,0,0))goto ordinary;
    edi=cm_fn_008cdd80(D(0x00de1f54u),ebp,club_home,1);
    if(!edi || B(edi+0x1fu) || B(self+0x2fu))goto new_managed;
    eax=D(edi);D(self)=eax;eax=D(edi+4u);D(self+4u)=eax;
    eax=D(edi+8u);D(self+8u)=eax;eax=D(edi+0xcu);D(self+0xcu)=eax;
    al=B(edi+0x10u);B(self+0x10u)=al;al=B(edi+0x11u);B(self+0x11u)=al;
    pair=cm_fn_00417c10(self+0x12u,P(0x3c),edi+0x12u);edx=(u32)(pair>>32);
    al=B(edi+0x1au);B(self+0x1au)=al;eax=D(edi+0x1bu);D(self+0x1bu)=eax;
    al=B(edi+0x1fu);B(self+0x1fu)=al;eax=D(edi+0x20u);D(self+0x20u)=eax;
    eax=D(edi+0x24u);D(self+0x24u)=eax;eax=D(edi+0x28u);D(self+0x28u)=eax;
    al=B(edi+0x2cu);B(self+0x2cu)=al;al=B(edi+0x2du);B(self+0x2du)=al;
    al=B(edi+0x2eu);B(self+0x2eu)=al;cl=B(edi+0x2fu);al=B(self+0x30u);B(self+0x2fu)=cl;
    b=B(edi+0x30u);b=((b^al)&15u)^al;eax=D(self+0x31u);B(self+0x30u)=(u8)b;
    ecx=D(edi+0x31u);D(self+0x31u)=((ecx^eax)&15u)^eax;
    eax=D(edi+0x35u);D(self+0x35u)=eax;al=B(edi+0x39u);B(self+0x39u)=al;
    eax=D(edi+0x3au);D(self+0x3au)=eax;al=B(edi+0x3eu);B(self+0x3eu)=al;
    al=B(edi+0x3fu);B(self+0x3fu)=al;al=B(edi+0x40u);B(self+0x40u)=al;
    if(!D(ebp+0x61u))goto copied_no_interest;
    eax=offer_home;if(!eax || (B(eax+0x24u)&0x1eu)>=8)goto copied_no_interest;
    RANDOM(3000);edx=D(ebp+0x61u);eax+=4000u;ecx=(u32)(s32)(s16)W(edx+0xbu);
    if((s32)ecx<=(s32)eax)goto copied_no_interest;
    eax=cm_fn_004cf5a0(D(0x00ae1050u),ebp);D(0x00ae1080u)=eax;
    if(!eax)goto copied_no_interest;
    eax=D(eax+4u);ecx=D(0x00ae23b4u);eax=ecx+eax*581u;if(!eax)goto copied_no_interest;
    eax=cm_fn_004cf5a0(D(0x00ae1050u),ebp);D(0x00ae1080u)=eax;
    if(eax){eax=D(eax+4u);ecx=D(0x00ae23b4u);eax=ecx+eax*581u;}
    edx=D(eax+0x57u);eax=club_home;if(edx!=D(eax+0x57u))goto copied_no_interest;
    RANDOM(3);if(eax)goto copied_no_interest;
    B(self+0x2du)=1;goto copied_reset;
copied_no_interest:
    B(self+0x2du)=0;
copied_reset:
    edx=D(self+0x31u);cl=B(self+0x30u);D(self+0x28u)=0xffffffffu;D(self+0x24u)=0xffffffffu;
    eax=D(self+4u);cl&=0xf0u;B(self+0x10u)=0;B(self+0x2cu)=0;B(self+0x2eu)=0;
    edx&=0xfffffff0u;D(self+0x31u)=edx;B(self+0x30u)=cl;B(self+0x2fu)=0;D(self)=0;
    if(!eax)goto validate_value;
    ecx=P(0x3c);eax=cm_fn_0068d020(ecx);frame.state=0;
    pair=cm_fn_00944cf1(eax,edx,ecx,14);eax=(u32)pair;edx=(u32)(pair>>32);
    L(0x10)=eax;*(volatile u8 *)&frame.state=1;
    if(eax)eax=cm_fn_0068d020(eax);
    D(self+4u)=eax;eax=D(edi+4u);*(volatile u8 *)&frame.state=0;
    ecx=D(eax+2u);D(eax+0xau)=ecx;
    ebp=cm_fn_0068d1b0(D(edi+4u));
    while(ebp) {
        eax=D(ebp);ecx=D(0x00ae23bcu);ebx=ecx+eax*110u;
        eax=cm_fn_004cf5a0(D(0x00ae1050u),ebx);D(0x00ae1080u)=eax;
        if(eax){eax=D(eax+4u);ecx=D(0x00ae23b4u);eax=ecx+eax*581u;}
        if(eax!=club_home)goto next_copy;
        if(cm_fn_004cf6d0(D(0x00ae1050u),ebx))goto next_copy;
        eax=cm_fn_004cf5a0(D(0x00ae1050u),ebx);D(0x00ae1080u)=eax;
        if(eax && (B(eax+0x35u)&63u)==5)goto next_copy;
        if(B(ebx+0x3du)!=11)goto next_copy;
        eax=cm_fn_008c5cf0(D(0x00de1f54u),ebx,0,0);
        if(!eax) {
            pair=cm_fn_008c5e40(D(0x00de1f54u),ebx,P(0x3c));
            eax=(u32)pair;edx=(u32)(pair>>32);
        }
        if(eax) {
            if(WL(0x3c)!=1)goto next_copy;
            ecx=L(0x3e);L(0x46)=ecx;eax=cm_fn_0068d1b0(P(0x3c));
            edx=D(eax);eax=cm_fn_008c61e0(D(0x00de1f54u),edx);
            ecx=player_home;eax=D(eax);edx=D(ecx);ecx=D(eax+0xcu);
            if(edx!=ecx)goto next_copy;
        }
        eax=D(ebp);L(0x14)=eax;WL(0x18)=W(ebp+4u);
        edx=(edx&0xffffff00u)|B(ebp+6u);ecx=D(self+4u);
        BL(0x1a)=(u8)edx;BL(0x18)=0;
        cm_fn_0068d0d0(ecx,edx,P(0x14),7);
next_copy:
        ebp=cm_fn_0068d1b0(D(edi+4u));
    }
    edi=D(self+4u);
    if(!W(edi)) {
        if(edi){cm_fn_0068d040(edi);cm_fn_00944c94(edi);}
        D(self+4u)=0;
    }
    frame.state=0xffffffffu;cm_fn_0068d040(P(0x3c));ebp=player_home;goto validate_value;
new_managed:
    ecx=D(ebp+0x52u);edx=club_home;D(self+0x1bu)=ecx;
    eax=cm_fn_005a0590(D(0x00ae2a38u),edx);pair=(u64)cm_fn_00598800(eax,ebp,0,0,1,0);eax=(u32)pair;
    if((s32)D(self+0x1bu)>(s32)eax)D(self+0x1bu)=eax;
    goto validate_value;
ordinary:
    eax=D(0x00de1f54u);ebx=L(0x34);ecx=D(eax+0x856u);eax=club_home;edx=offer_home;
    edi=cm_fn_0084b6f0(edx,ebx,eax,P(0x54));
    if(edi && !cm_fn_008d0a70(ebp))eax=0;else eax=1;
    ecx=offer_home;
    if(ecx && eax) {
        if(ebx && B(ecx)==11) {
            al=B(ebx+0x35u)&63u;eax=(eax&0xffffff00u)|al;
            if(al!=1 && al!=3 && al!=5 && !edi)goto evaluate;
        }
        edi=output_home;if(!edi)goto failure;
        pair=cm_fn_00944cf1(eax,edx,ecx,80);eax=(u32)pair;edx=(u32)(pair>>32);
        L(0x10)=eax;frame.state=2;
        if(eax)eax=cm_fn_0085e410(eax);
        D(edi)=eax;ecx=D(0x00ae1050u);a=club_home;
        frame.state=0xffffffffu;
        if(!cm_fn_004cda70(ecx,ebp,a,eax,-1,1.0,2)) {
            eax=D(edi);if(eax)cm_fn_00944c94(eax);goto failure;
        }
        B(self+0x2fu)=1;goto validate_value;
    }
evaluate:
    eax=cm_fn_004cf5a0(D(0x00ae1050u),ebp);D(0x00ae1080u)=eax;
    if(eax){eax=D(eax+4u);edx=D(0x00ae23b4u);ebx=edx+eax*581u;}else ebx=0;
    L(0x38)=ebx;eax=offer_home;
    if(!eax || B(eax)!=11)goto no_value;
    ecx=D(ebp+0x61u);if(!ecx)goto failure;
    /* Native CMP AL,AL at008E64A4 is always equal. */
    if(ebx && cm_fn_00600a50(ebx,0,1,0,0)) {
        edi=cm_fn_004d42d0(D(0x00ae1050u),ebx,club_home,ebp);L(0x30)=edi;
        if(cm_fn_005444a0(ebp,0)) {
            a=(u32)((s32)edi/3);b=edi-1500000u;
            L(0x14)=(s32)a>(s32)b?a:b;
        }
    }else {
        edi=cm_fn_004d1600(D(0x00ae1050u),ebx,club_home,ebp,0,1,1);L(0x30)=edi;
        eax=cm_fn_004d1600(D(0x00ae1050u),ebx,club_home,ebp,0,1,0);L(0x2c)=edi-eax;
    }
    goto value_ready;
no_value:
    edi=0;L(0x30)=0;
value_ready:
    ebx=L(0x34);
    if(ebx && cm_fn_00867b50(ebx)) {
        eax=D(ebx+0x21u);
        if((s32)edi>(s32)eax) {
            L(0x30)=eax;eax=D(self+0x31u);D(self+0x31u)=(eax&0xfffffff1u)|1u;
        }
    }
    eax=offer_home;
    if(eax && B(eax)==11) {
        ecx=D(0x00de1f54u);ecx=D(ecx+0x856u);edi=eax;
        ebx=cm_fn_00840d80(ecx,club_home,ebp,edi,P(0x58),0xffffffffu,1);L(0x1c)=ebx;
    }else {edi=offer_home;ebx=0;L(0x1c)=0;}
    if(edi && B(edi)==11) {
        edi=L(0x38);if(!edi)goto cap_ready;
        eax=cm_fn_005a0590(D(0x00ae2a38u),edi);if(!eax)goto cap_ready;
        eax=cm_fn_005a0590(D(0x00ae2a38u),edi);if(B(eax+0x165u)!=1)goto cap_ready;
        if(cm_fn_00600a50(edi,0,1,0,0)) {
            eax=cm_fn_004d0c60(D(0x00ae1050u),ebp,edi,0,0xffffffffu,0);
            if((s32)ebx<(s32)eax)eax=ebx;
            else eax=cm_fn_004d0c60(D(0x00ae1050u),ebp,edi,0,0xffffffffu,0);
            ecx=L(0x30);
            if((s32)ecx<(s32)eax){ebx=ecx;L(0x10)=1;L(0x1c)=ebx;goto cap_ready;}
            eax=cm_fn_004d0c60(D(0x00ae1050u),ebp,edi,0,0xffffffffu,0);
            if((s32)ebx>=(s32)eax) {
                ebx=cm_fn_004d0c60(D(0x00ae1050u),ebp,edi,0,0xffffffffu,0);L(0x1c)=ebx;
            }
            L(0x10)=1;
        }else {
            eax=D(ebp+0x52u);ecx=ebx;if((s32)ebx>=(s32)eax)ecx=eax;edx=L(0x30);
            if((s32)edx<(s32)ecx){ebx=edx;L(0x10)=1;L(0x1c)=ebx;goto cap_ready;}
            if((s32)ebx>=(s32)eax){ebx=eax;L(0x1c)=ebx;}L(0x10)=1;
        }
    }else edi=L(0x38);
cap_ready:
    ecx=L(0x30);D(self+0x1bu)=ecx;
    if(!edi)goto unscaled;
    RANDOM(1000000);ecx=D(self+0x1bu);
    if((s32)ecx<=(s32)eax)goto unscaled;
    edx=club_home;ecx=D(0x009cf2e4u);L(0x24)=0;L(0x28)=0x40000000u;
    eax=D(edx+0x53u);eax=D(eax);
    if(eax==ecx || eax==D(0x009cf45cu)){L(0x24)=0;L(0x28)=0x3fe80000u;}
    eax=cm_fn_005a0590(D(0x00ae2a38u),club_home);eax=cm_fn_005986b0(eax);
    eax=(u32)(s32)(s8)eax+2u;
    if(eax<=4) {
        target=D(0x008e77dcu+eax*4u);(void)target;fp=Q(P(0x24));
        switch(eax) {case 0:fp*=Q(0x00968e18u);break;case 1:fp*=Q(0x00968940u);break;
        case 2:fp*=Q(0x009689b0u);break;case 3:fp*=Q(0x00968e00u);break;default:fp*=Q(0x00967880u);break;}
        Q(P(0x24))=(double)fp;
    }
    eax=cm_fn_005a0590(D(0x00ae2a38u),edi);eax=cm_fn_005986b0(eax);
    eax=(u32)(s32)(s8)eax+2u;fp=Q(P(0x24));
    if(eax<=4) {
        target=D(0x008e77f0u+eax*4u);(void)target;
        switch(eax) {case 0:fp*=Q(0x00967880u);break;case 1:fp*=Q(0x00968e00u);break;
        case 2:fp*=Q(0x009689b0u);break;case 3:fp*=Q(0x00968940u);break;default:fp*=Q(0x00968e18u);break;}
    }
    if(fp>(long double)Q(0x00968930u)){months=24;SCALE(008e684b,008e685f,008e6874,0x0096f9a8u);goto scaled;}
    if(fp>(long double)Q(0x00970448u)){months=18;SCALE(008e68a4,008e68b8,008e68cd,0x00970440u);goto scaled;}
    if(fp>(long double)Q(0x00969018u)){months=12;SCALE(008e68fd,008e6911,008e6926,0x00968928u);goto scaled;}
    if(fp>(long double)Q(0x00967890u)){months=6;SCALE(008e6951,008e6965,008e697a,0x0096f998u);goto scaled;}
unscaled:
    edi=value_home;
scaled:
    if((s32)edi<=-1 || (s32)edi>=(s32)D(self+0x1bu))goto price_ready;
    ecx=L(0x14);
    if((s32)ecx>-1) {
        eax=D(self+0x1bu);if((s32)ecx<(s32)eax)eax=ecx;
        D(self+0x1bu)=eax;B(self+0x40u)=0;
        if((s32)eax>(s32)edi)D(self+0x1bu)=edi;
        goto price_ready;
    }
    fp=(long double)(s32)value_home;other=(long double)(s32)L(0x30);
    D(self+0x1bu)=edi;Q(P(0x24))=(double)other;
    third=fp;third*=Q(0x00968dc0u);
    if(!(third>=(long double)Q(P(0x24))))goto expensive;
    fp*=Q(0x00968978u);
    if(!(fp>=(long double)Q(P(0x24))) && L(0x2c))goto expensive;
    if((s32)edi<100000)goto price_ready;
    if(fp>=(long double)Q(P(0x24)))goto price_ready;
    eax=offer_home;if(!eax || B(eax)!=11)goto price_ready;
    eax=D(ebp+0x1au);if(!eax)goto price_ready;
    a=W(eax+0x8eu);
    if((s16)a>5000 && B(ebp+0x22u)<5) {
        edx=D(ebp+0x61u);ecx=(u32)(s32)(s16)W(edx+9u);L(0x14)=ecx;
        fp=(long double)(s32)L(0x14);edx=(u32)(s32)(s16)a;fp*=Q(0x00970438u);
        L(0x14)=edx;other=(long double)(s32)L(0x14);
        if(!(other>=fp))goto first_bonus;
        eax=D(ebp+0x24u);if(eax && cm_fn_005409b0(eax))goto first_bonus;
        goto first_bonus_done;
first_bonus:
        RANDOM(4);ecx=(u32)(s32)(s8)B(ebp+0x18u);eax+=25u;
        if((s32)ecx>=(s32)eax)goto first_bonus_done;
        L(0x20)=1;
        if(!B(ebp+0x22u)) {
            RANDOM(5);
            if(!eax) {
                RANDOM(6);eax+=5u;L(0x14)=eax;fp=(long double)(s32)L(0x14);
                fp*=(s32)D(self+0x1bu);fp*=Q(0x00967898u);CONVERT(008e6aff,fp);
                D(self+0x3au)=eax;B(self+0x3eu)=1;goto first_bonus_done;
            }
        }
        RANDOM(11);eax+=5u;L(0x14)=eax;fp=(long double)(s32)L(0x14);
        fp*=(s32)D(self+0x1bu);fp*=Q(0x00967898u);CONVERT(008e6b2b,fp);
        D(self+0x3au)=eax;B(self+0x3eu)=10;
    }
first_bonus_done:
    eax=D(self+0x1bu);edx=(s32)eax<0?7u:0u;eax=(u32)((s32)(eax+edx)>>3);
    RANDOM(eax);ecx=D(self+0x3au);
    if((s32)ecx<(s32)eax) {
        edx=offer_home;al=B(edx+0x24u)&0x1eu;
        if(al==10) {
            RANDOM(2);
            if(eax) {
                L(0x20)=1;RANDOM(5);ADD_AL(1);MUL_AL(10);B(self+0x39u)=(u8)eax;
                RANDOM(11);eax+=5u;BONUS(008e6bbb,0x14);
            }
        }
    }
    eax=D(self+0x1bu);edx=(s32)eax<0?7u:0u;eax=(u32)((s32)(eax+edx)>>3);
    RANDOM(eax);ecx=D(self+0x35u);edx=D(self+0x3au);ecx+=edx;
    if((s32)ecx<(s32)eax) {
        edx=offer_home;al=B(edx+0x24u)&0x1eu;
        if(al!=10) {
            RANDOM(20);edx=(u32)(s32)(s8)B(ebp+0x18u);ecx=28u-eax;
            if((s32)edx>=(s32)ecx)goto extra_bonus_done;
        }
        L(0x20)=1;RANDOM(5);ADD_AL(1);MUL_AL(5);bl=(u8)eax;
        RANDOM(5);ADD_AL(1);MUL_AL(5);
        if((s8)bl<(s8)eax)eax=(eax&0xffffff00u)|bl;
        edi=value_home;ebx=L(0x1c);B(self+0x3fu)=(u8)eax;
    }
extra_bonus_done:
    if(D(self+0x3au) || D(self+0x35u) || B(self+0x3fu))goto price_ready;
    ebx=offer_home;fp=(long double)(s32)D(ebx+3u);fp*=Q(0x00968f80u);CONVERT(008e6c79,fp);
    RANDOM(eax);edx=eax;eax=D(ebx+0xbu);L(0x14)=edx;RANDOM(eax);ecx=L(0x14);
    if((s32)eax>(s32)ecx) {
        L(0x20)=1;RANDOM(5);ADD_AL(1);MUL_AL(10);B(self+0x39u)=(u8)eax;
        RANDOM(11);eax+=5u;BONUS(008e6cec,0x14);
    }
    ebx=L(0x1c);goto price_ready;
expensive:
    eax=offer_home;if(!eax || B(eax)!=11)goto failure;
    al=B(eax+0x24u)&0x1eu;
    if(al!=10 && !(al==8 && (s8)B(ebp+0x18u)<28) && !L(0x2c))goto failure;
    ecx=D(ebp+0x1au);L(0x20)=1;a=W(ecx+0x8eu);
    if((s16)a<=5000 || B(ebp+0x22u))goto expensive_general;
    edx=D(ebp+0x61u);ecx=(u32)(s32)(s16)W(edx+9u);L(0x14)=ecx;
    fp=(long double)(s32)L(0x14);edx=(u32)(s32)(s16)a;fp*=Q(0x00970438u);
    L(0x14)=edx;other=(long double)(s32)L(0x14);
    if(!(other>=fp))goto expensive_upfront;
    eax=D(ebp+0x24u);if(eax && cm_fn_005409b0(eax))goto expensive_upfront;
    RANDOM(5);ADD_AL(1);MUL_AL(10);B(self+0x39u)=(u8)eax;
    RANDOM(11);eax+=10u;BONUS(008e6ddd,0x14);
    RANDOM(6);MUL_AL(5);bl=(u8)eax;RANDOM(6);MUL_AL(5);cl=(u8)eax;
    fp=(long double)(s32)L(0x2c);other=Q(P(0x24));other*=Q(0x00969040u);
    if(!(other>=fp))goto expensive_cap;
    goto expensive_max_percent;
expensive_upfront:
    fp=(long double)(s32)D(self+0x1bu);fp*=Q(0x00969040u);
    other=(long double)(s32)L(0x2c);Q(P(0x3c))=(double)other;
    other*=Q(0x00969520u);Q(P(0x14))=(double)other;
    if(!(fp>(long double)Q(P(0x14))))fp=Q(P(0x14));
    CONVERT(008e6eb9,fp);D(self+0x3au)=eax;B(self+0x3eu)=10;
    RANDOM(3);ADD_AL(3);MUL_AL(10);B(self+0x39u)=(u8)eax;
    RANDOM(11);eax+=5u;BONUS(008e6f04,0x14);
    RANDOM(6);MUL_AL(5);bl=(u8)eax;RANDOM(6);MUL_AL(5);cl=(u8)eax;
    fp=Q(P(0x24));fp*=Q(0x00969040u);
    if(!(fp>=(long double)Q(P(0x3c))))goto expensive_cap;
    if((s8)bl<(s8)cl)cl=bl;
    ebp=player_home;B(self+0x3fu)=cl;goto restore_price_registers;
expensive_general:
    RANDOM(3);ADD_AL(3);MUL_AL(10);B(self+0x39u)=(u8)eax;
    RANDOM(11);eax+=5u;BONUS(008e700c,0x14);
    RANDOM(5);ADD_AL(1);MUL_AL(5);bl=(u8)eax;
    RANDOM(5);ADD_AL(1);MUL_AL(5);cl=(u8)eax;
    fp=(long double)(s32)L(0x2c);other=Q(P(0x24));other*=Q(0x00969040u);
    if(!(other>=fp)) {
        if((s8)bl>(s8)cl)cl=bl;
        B(self+0x3fu)=cl;goto restore_price_registers;
    }
expensive_max_percent:
    if((s8)bl<(s8)cl)cl=bl;
    B(self+0x3fu)=cl;goto restore_price_registers;
expensive_cap:
    if((s8)bl>(s8)cl)cl=bl;
    ebx=L(0x30);B(self+0x3fu)=cl;
    b=(s32)ebx>10000?ebx:10000u;
    eax=(u32)(s32)(s8)B(self+0x39u);ecx=D(self+0x35u);ecx*=L(0x2c);
    edi=eax*30000u+L(0x30);
    a=(u32)((s32)ecx/(s32)b);
    if((s32)edi<(s32)a)eax=edi;
    else {
        edi=L(0x30);if((s32)edi<=10000)edi=10000;
        eax=(u32)((s32)ecx/(s32)edi);
    }
    ebp=player_home;D(self+0x35u)=eax;
restore_price_registers:
    edi=value_home;ebx=L(0x1c);
price_ready:
    eax=cm_fn_008cc930(D(0x00de1f54u),ebp,club_home);
    if(eax && B(eax+0x2cu)==2 && !L(0x10))L(0x2c)=cm_fn_008e7a70(eax,0,0,0,1);
    else L(0x2c)=0xffffffffu;
    ecx=D(self+0x1bu);L(0x14)=ecx;
    if((s32)ebx<=(s32)ecx) {
        fp=(long double)(s32)L(0x1c);other=(long double)(s32)L(0x14);other*=Q(0x009689b0u);
        if(!(other>fp)) {
            fp=(long double)(s32)L(0x14);other=(long double)(s32)L(0x2c);other*=Q(0x00968978u);
            if(other>fp)goto clear_output;
            goto final_money;
        }
    }
    fp=(long double)(s32)L(0x1c);Q(P(0x3c))=(double)fp;
    fp=(long double)(s32)L(0x14);other=fp;other*=Q(0x00969040u);
    if(!(other>=(long double)Q(P(0x3c))))goto price_fallback;
    if(L(0x20))goto price_fallback;
    edx=offer_home;if(!edx || B(edx)!=11)goto price_fallback;
    fp*=Q(0x00968e00u);
    if(fp>(long double)Q(P(0x3c)))goto clear_output;
    al=B(edx+0x24u)&0x1eu;
    if(al!=10 && !(al==8 && (s8)B(ebp+0x18u)<28))goto clear_output;
    ecx=D(ebp+0x1au);a=W(ecx+0x8eu);
    if((s16)a<=5000 || B(ebp+0x22u))goto counter_general;
    edx=D(ebp+0x61u);ecx=(u32)(s32)(s16)W(edx+9u);L(0x10)=ecx;
    fp=(long double)(s32)L(0x10);edx=(u32)(s32)(s16)a;fp*=Q(0x00970438u);
    L(0x10)=edx;other=(long double)(s32)L(0x10);
    if(!(other>=fp))goto counter_upfront;
    eax=D(ebp+0x24u);if(eax && cm_fn_005409b0(eax))goto counter_upfront;
    RANDOM(5);ADD_AL(1);MUL_AL(10);B(self+0x39u)=(u8)eax;
    RANDOM(11);eax+=10u;goto counter_bonus;
counter_upfront:
    fp=(long double)(s32)D(self+0x1bu);fp*=Q(0x00969040u);CONVERT(008e71fe,fp);
    D(self+0x3au)=eax;B(self+0x3eu)=10;
    RANDOM(5);ADD_AL(3);MUL_AL(10);B(self+0x39u)=(u8)eax;
    RANDOM(11);eax+=5u;
counter_bonus:
    BONUS(008e7249,0x10);
    RANDOM(6);MUL_AL(5);bl=(u8)eax;RANDOM(6);MUL_AL(5);
    if((s8)bl<(s8)eax)eax=(eax&0xffffff00u)|bl;
    B(self+0x3fu)=(u8)eax;goto failure;
counter_general:
    RANDOM(3);ADD_AL(3);MUL_AL(10);B(self+0x39u)=(u8)eax;
    RANDOM(11);eax+=5u;BONUS(008e72b9,0x10);
    RANDOM(5);ADD_AL(1);MUL_AL(5);bl=(u8)eax;
    RANDOM(5);ADD_AL(1);MUL_AL(5);
    if((s8)bl<(s8)eax)eax=(eax&0xffffff00u)|bl;
    B(self+0x3fu)=(u8)eax;goto failure;
price_fallback:
    if((s32)ebx>=(s32)ecx)goto callback_floor;
    if((s32)ecx>=(s32)edi)goto store_cap;
    eax=offer_home;if(!eax)goto store_cap;
    eax=D(eax+3u);L(0x10)=eax;if(!eax || L(0x20))goto store_cap;
    ecx=offer_home;if(B(ecx)!=11)goto store_cap;
    edx=ecx;fp=(long double)(s32)D(edx+0xbu);fp/=(s32)L(0x10);
    F(P(0x20))=(float)fp;
    if(!(fp>=(long double)Q(0x00967888u)))L(0x20)=0x3d4ccccdu;
    fp=F(P(0x20));if(fp>(long double)Q(0x00969040u))L(0x20)=0x3f400000u;
    RANDOM(2);
    if(eax) {
        fp=F(0x00968a78u);fp/=F(P(0x20));fp*=F(0x0096afc4u);CONVERT(008e7387,fp);
        eax=(u32)(s32)(s8)eax*10u;
        if((s32)eax<20)eax=20;else if((s32)eax>100)eax=100;
        ecx=(u32)(s32)(s8)eax;B(self+0x39u)=(u8)eax;
        eax=(u32)((s32)ecx/10)*10u;
        if((s32)eax<10)eax=10;else if((s32)eax>50)eax=50;
        ecx=D(self+0x1bu);B(self+0x39u)=(u8)eax;eax=(u32)(s32)(s8)eax;
        edi-=ecx;L(0x10)=edi;fp=(long double)(s32)L(0x10);fp*=F(P(0x20));
        edx=eax*30000u;L(0x10)=edx;other=(long double)(s32)L(0x10);
        F(P(0x10))=(float)other;
        if(fp>=(long double)F(P(0x10)))fp=F(P(0x10));
        CONVERT(008e7422,fp);D(self+0x35u)=eax;
    }else {
        edi-=D(self+0x1bu);L(0x10)=edi;fp=(long double)(s32)L(0x10);
        fp*=F(P(0x20));fp*=F(0x00968f18u);CONVERT(008e7441,fp);ebx+=eax;
    }
    eax=L(0x30);if((s32)ebx>(s32)eax)ebx=eax;
store_cap:
    D(self+0x1bu)=ebx;goto final_money;
callback_floor:
    other=(long double)(s32)L(0x2c);other*=Q(0x00968978u);Q(P(0x3c))=(double)other;
    if(!(fp>=(long double)Q(P(0x3c)))) {
        fp=Q(P(0x3c));CONVERT(008e7474,fp);D(self+0x1bu)=eax;
        if((s32)eax>(s32)ebx)goto clear_output;
    }
final_money:
    eax=cm_fn_005a0590(D(0x00ae2a38u),club_home);
    pair=(u64)cm_fn_00598800(eax,ebp,0,0,1,0);
    eax=D(self+0x1bu);
    if((s64)pair<(s64)(s32)eax)goto failure;
    edi=L(0x34);
    if(edi && cm_fn_00867b50(edi)) {
        fp=(long double)(s32)D(self+0x1bu);fp*=Q(0x009689b8u);
        other=(long double)(s32)D(edi+0x21u);
        if(!(other>=fp)) {
            D(self+0x35u)=0;D(self+0x3au)=0;B(self+0x3fu)=0;
            D(self+0x1bu)=D(edi+0x21u);
        }
        goto validate_value;
    }
    eax=offer_home;
    if(!eax || B(eax)==11)cm_fn_008e8330(self,ebp,L(0x38),club_home);
validate_value:
    eax=value_home;
    if(eax!=0xffffffffu) {
        if((s32)eax<5000) {
            ecx=D(ebp+0x52u);if((s32)ecx>0)eax=5000;
        }
        if((s32)eax>-1 && (s32)D(self+0x1bu)>(s32)eax)goto clear_output;
    }
    edx=D(0x00ae2c90u);eax=D(0x00ae2c94u);L(0x4c)=edx;L(0x50)=eax;
    ebx=output_home;frame.state=3;
    if(ebx && D(ebx) && cm_fn_0053b620(B(ebp+0x3du))) {
        edi=L(0x34);
        if(edi) {
            al=B(self+0x30u);B(self+0x30u)=(al&0xf3u)|3u;
            cm_fn_00417c10(P(0x4c),P(0x24),edi+0x2du);
            edx=club_home;eax=cm_fn_00868210(edi,edx,0,0);
            if(!eax) {
                eax=cm_fn_008d0780(ebp,club_home,eax);D(self+0x1bu)=eax;
                cl=B(self+0x30u);B(self+0x30u)=(cl&0xf5u)|5u;
            }
        }
    }
    eax=cm_fn_004cf5a0(D(0x00ae1050u),ebp);D(0x00ae1080u)=eax;
    if(eax && (B(eax+0x35u)&63u)!=5) {
        eax=D(eax+4u);edx=D(0x00ae23b4u);eax=edx+eax*581u;
    }else eax=0;
    edx=P(0x4c);
    edi=cm_fn_008ced40(D(0x00de1f54u),club_home,edx,ebp,eax,1,P(0x4a));
    if(!edi && WL(0x4a)!=9) {
        if(ebx) {eax=D(ebx);if(eax)cm_fn_00944c94(eax);D(ebx)=0;}
        goto failure;
    }
    if((!ebx || !D(ebx)) && cm_fn_0053b620(B(ebp+0x3du)) && edi) {
        al=B(self+0x30u);B(self+0x30u)=al&0xf0u;
    }else if(cm_fn_0053b620(B(ebp+0x3du))) {
        al=B(self+0x30u);cl=al&15u;
        if(!cl)B(self+0x30u)=(al&0xf1u)|1u;
        else if(cl==3)B(self+0x30u)=(al&0xf4u)|4u;
    }
    eax=cm_fn_004cf5a0(D(0x00ae1050u),ebp);D(0x00ae1080u)=eax;
    if(eax && (B(eax+0x35u)&63u)!=5) {
        eax=D(eax+4u);ecx=D(0x00ae23b4u);eax=ecx+eax*581u;
        if(eax)goto final_status;
    }
    B(self+0x2fu)=1;
final_status:
    al=B(self+0x2fu);
    if(al && (B(self+0x30u)&15u)!=5)D(self+0x1bu)=cm_fn_008d0780(ebp,club_home,1);
    eax=1;goto done;
clear_output:
    eax=output_home;
    if(eax) {
        edi=eax;eax=D(edi);if(eax)cm_fn_00944c94(eax);D(edi)=0;
    }
    goto failure;
invalid:
    cm_fn_00944cff(0x00a9e318u,0,0,P(0x15c),P(0x5c));
    eax=cm_fn_0090d130();cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,eax,P(0x15c),P(0x5c),0x154u);
    cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);D(0x00b67a34u)=0;
failure:
    eax=0;
done:
    CM_FS(u32,0)=frame.previous;
    return eax;
}
