#include "cmre.h"
#pragma GCC target("no-sse3")

/* Primary 3.9.68 SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All instruction addresses below are preferred-image VAs (base 00400000).
   Five cdecl stack slots; retain full EAX conservatively. */
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
#define R8(p) CM_GLOBAL(volatile s8, (u32)(p))
#define R16(p) CM_GLOBAL(volatile s16, (u32)(p))
#define R32(p) CM_GLOBAL(volatile u32, (u32)(p))
#define RD(p) CM_GLOBAL(volatile double, (u32)(p))
#define RF(p) CM_GLOBAL(volatile float, (u32)(p))

u32 CM_CDECL cm_fn_006d3de0(u32 object, u32 side, float input,
                           u32 which, u32 optional)
{
    u32 record = R32(object + 0x4792u), selected, other, p;
    s32 b, n, original, converted, cap, result;
    s16 tag;
    long double ratio, lower, t;
    volatile float delta, base_float, adjustment, result_float;
    if ((u8)side) { selected=R32(record+0x20); other=R32(record+0x1c); }
    else { selected=R32(record+0x1c); other=R32(record+0x20); }
    if (R32(record)==0xfffffffeu) b=5;
    else {
        if (R8(record+0x40)) {
            s32 q=R16(other+0x80)/500;
            p=R32(record+0x14);
            b=(s8)((q+R16(p+0x69))/2);
        } else { p=R32(record+0x14); b=R8(p+0x69); }
        if (b>20) b=20;
    }
    if (cm_fn_0053b900((u32 *)selected)) {
        n=(s8)(R16(selected+0x80)/1000+16);
        if(n<21) n=21;
        n-=b;
    } else {
        if (R8(record+0x3a)==8) {
            tag=R16(record+0x32);
            if ((u16)tag>=0x41f && (u16)tag<=0x426) {
                n=(s8)(R16(selected+0x80)/1000+16);
                if(n<21) n=21;
                n=n-b+1;
                goto numeric;
            }
            tag=R16(record+0x30);
            if (((u16)tag>=0x78 && (u16)tag<=0x7c) ||
                ((u16)tag>=0x82 && (u16)tag<=0x87) ||
                ((u16)tag>=0x96 && (u16)tag<=0x9b)) {
                n=(s8)(R16(selected+0x80)/1000+16);
                if(n<21) n=21;
                p=R32(record+0x14); b=R8(p+0x69); n-=b;
                goto numeric;
            }
        }
        n=202-10*b;
    }
numeric:
    p=R32(0xb74350);
    t=RD(p+((u8)which?0x28:0x18));
    ratio=RD(0x9689a0);
    /* FCOMP C3: equality OR unordered selects the fallback. */
    if (!(t<ratio) && !(t>ratio)) ratio=RD(0x968998);
    else { ratio=RD(p+((u8)which?0x20:0x10)); ratio/=RD(p+((u8)which?0x28:0x18)); }
    delta=(float)((long double)input-ratio);
    if (!((long double)delta>=RF(0x968970)))
        delta=(float)(__builtin_fabsl((long double)delta)*(long double)delta);
    original=R16(selected+0x80);
    lower=(long double)original;
    t=(long double)b; t*=RD(0x968968); t+=RD(0x9688a0);
    t*=(long double)delta; t*=RD(0x968ff8); t+=lower;
    /* 006d4075 __ftol consumes only ST0; lower survives. */
    converted=(s32)(u32)(long long)t;
    if ((u8)optional) {
        p=R32(0xb74350); t=RD(p+((u8)which?0x28:0x18)); ratio=RD(0x9689a0);
        if (!(t<ratio) && !(t>ratio)) ratio=RD(0x968998);
        else { ratio=RD(p+((u8)which?0x20:0x10)); ratio/=RD(p+((u8)which?0x28:0x18)); }
        t=(long double)(b*250); t*=(long double)input; t/=ratio;
        /* 006d40f1 __ftol, followed by explicit pop of ratio. */
        result=(s32)(u32)(long long)t;
        if (!((long double)delta>=RF(0x968970))) {
            p=R32(0xb74350); t=RD(p+((u8)which?0x28:0x18)); ratio=RD(0x9689a0);
            if (!(t<ratio) && !(t>ratio)) ratio=RD(0x968998);
            else { ratio=RD(p+((u8)which?0x20:0x10)); ratio/=RD(p+((u8)which?0x28:0x18)); }
            t=(long double)(s16)result; t*=(long double)input; t/=ratio;
            /* 006d4160 __ftol, followed by explicit pop of ratio. */
            result=(s32)(u32)(long long)t;
        }
        if ((s16)result>(s16)converted) converted=result;
    }
    lower*=RD(0x96a5c0); lower=RD(0x968950)-lower;
    result=(s32)(((u32)(s32)(s16)n-1u)*(u32)original+(u32)(s32)(s16)converted);
    result/=(s16)n;
    cap=(s16)(4000+300*b);
    if (cap<=(s16)original) cap=original;
    if ((s16)result>=cap) result=cap;
    base_float=(float)(long double)original;
    adjustment=(float)((long double)(b*5)*lower);
    t=(long double)base_float-(long double)adjustment;
    result_float=(float)(long double)(s16)result;
    if (!(t>(long double)result_float)) {
        t=(long double)adjustment+(long double)base_float;
        if (t>=(long double)result_float) t=(long double)result_float;
    }
    /* 006d4216 __ftol returns full EDX:EAX; subsequent clamps test AX. */
    result=(s32)(u32)(long long)t;
    if ((s16)result>10000) result=10000;
    else if ((s16)result<1) result=1;
    CM_GLOBAL(volatile u16, object+0x1d6u+(u32)(s32)(s8)side*2u)=(u16)result;
    return (u32)result;
}
