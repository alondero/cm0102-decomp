#include "cmre.h"
#pragma GCC target("no-sse3")

/* Preferred-image VAs, base 0x00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   No local address escapes: native scratch and argument slots are scalar
   spills/reassignments, not buffers or aliased inputs. */
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
#define R8(p,o) (*(volatile s8 *)((u32)(p)+(u32)(o)))
#define R16(p,o) (*(volatile s16 *)((u32)(p)+(u32)(o)))
#define R32(p,o) (*(volatile u32 *)((u32)(p)+(u32)(o)))
#define RD(p,o) (*(volatile double *)((u32)(p)+(u32)(o)))
#define GD(a) CM_GLOBAL(volatile double,a)
#define GF(a) CM_GLOBAL(volatile float,a)

static long double ratio(u8 flag)
{
    u32 p = CM_GLOBAL(volatile u32,0x00b74350u);
    u32 offset = flag ? 0x28u : 0x18u;
    long double d = RD(p,offset);
    long double z = GD(0x009689a0u);
    /* FCOMP's C3 is set on equality and unordered. */
    if (!(d < z || d > z)) return GD(0x00968998u);
    d = RD(p,offset-8);
    return d / (long double)RD(p,offset);
}

static s16 native_denominator(s16 value, s8 coefficient, s8 floor)
{
    s8 byte = (s8)((u32)(value / 1000) + 16u);
    if (byte < floor) byte = floor;
    return (s16)((s32)byte - coefficient);
}

void CM_CDECL cm_fn_006d3610(u32 output,u32 input,u32 other,u32 record,
                            float amount,u8 flag,u8 adjust)
{
    u32 object = R32(output,0x1b), stats, p, result;
    s8 coefficient;
    s16 den_a,den_b,den_c,div_b=2,div_c=2;
    volatile s16 raw_a,raw_b,raw_c;
    volatile s32 a,b,c;
    s32 cap,limit,total;
    long double delta,contribution,t;
    volatile long long converted;
    volatile double base_a,base_b,base_c;
    volatile float range_a,range_b,range_c,rounded_raw,rounded_target;
    if (!object) return;
    stats = R32(object,0x69);
    if (!stats) return;
    if (R32(record,0) == 0xfffffffeu) coefficient=5;
    else {
        if (R8(record,0x40)) {
            s32 q = R16(other,0x80) / 500;
            p = R32(record,0x14);
            q += R16(p,0x69);
            coefficient = (s8)(q/2);
        } else {
            p=R32(record,0x14);
            coefficient=R8(p,0x69);
        }
        if (coefficient>20) coefficient=20;
    }
    den_b=native_denominator(R16(stats,0xa),coefficient,22);
    result=cm_fn_0053b900((u32 *)input);
    if (result) goto related;
    if (R32(record,0)==0xfffffffeu) {
        p=R32(input,0x53);
        if (p && p==R32(object,0x1a)) goto related;
    }
    p=R32(record,0x14);
    if (p) {
        u32 team=R32(p,0x5d);
        if (team==R32(object,0x1a)) goto related;
    }
    p=R32(input,0x53);
    if (p==R32(object,0x1a)) goto related;
    stats=R32(object,0x69);
    den_a=native_denominator(R16(stats,8),coefficient,22);
    den_a=(s16)(((s32)(s16)(202-10*coefficient)+(s32)den_a)/2);
    if (den_a<4) den_a=4;
    goto second;
related:
    stats=R32(object,0x69);
    den_a=native_denominator(R16(stats,8),coefficient,22);
    div_b=1;
second:
    result=cm_fn_0053b900((u32 *)input);
    if (result) {
        stats=R32(object,0x69);
        den_c=native_denominator(R16(stats,0xc),coefficient,21);
        div_c=1;
    } else if (R8(record,0x3a)==8) {
        s16 code=R16(record,0x32);
        div_c=1;
        if (code>=0x41f && code<=0x426) {
            stats=R32(object,0x69);
            den_c=(s16)(native_denominator(R16(stats,0xc),coefficient,21)+1);
        } else {
            code=R16(record,0x30);
            if ((code>=0x78 && code<=0x7c) ||
                (code>=0x82 && code<=0x87) || (code>=0x96 && code<=0x9b)) {
                stats=R32(object,0x69);
                den_c=native_denominator(R16(stats,0xc),0,21);
                p=R32(record,0x14);
                coefficient=R8(p,0x69);
                den_c=(s16)(den_c-coefficient);
            } else den_c=(s16)(202-10*coefficient);
        }
    } else den_c=(s16)(202-10*coefficient);

    delta=ratio(flag);
    delta=(long double)amount-delta;
    delta=delta*(long double)GF(0x0096d490u);
    stats=R32(object,0x69);
    t=(long double)coefficient;
    raw_a=R16(stats,8);
    t=t*(long double)GD(0x00968968u);
    t=t+(long double)GD(0x009688a0u);
    contribution=t*delta;
    contribution=contribution*(long double)GD(0x00968ff8u);
    base_a=(long double)raw_a;
    t=(long double)raw_a+contribution;
    a=(s32)(u32)(converted=(long long)t); /* __ftol at VA 0x006d399c */
    raw_b=R16(stats,0xa);
    base_b=(long double)raw_b;
    t=(long double)raw_b+contribution;
    b=(s32)(u32)(converted=(long long)t); /* __ftol at VA 0x006d39bc */
    raw_c=R16(stats,0xc);
    base_c=(long double)raw_c;
    t=(long double)raw_c+contribution;
    c=(s32)(u32)(converted=(long long)t); /* __ftol at VA 0x006d39de */
    if (adjust) {
        long double r=ratio(flag);
        t=(long double)(250*(s32)coefficient)*(long double)amount;
        t=t/r;
        cap=(s32)(u32)(converted=(long long)t); /* __ftol at VA 0x006d3a54 */
        /* FCOMP tests C0, including unordered. */
        if (!(delta >= (long double)GF(0x00968970u))) {
            r=ratio(flag);
            t=(long double)(s16)cap*(long double)amount;
            t=t/r;
            cap=(s32)(u32)(converted=(long long)t); /* __ftol at VA 0x006d3abf */
        }
        if ((s16)cap>(s16)b) b=cap;
        if ((s16)cap>(s16)a) a=cap;
        if ((s16)cap>(s16)c) c=cap;
    }
    t=(long double)base_b*(long double)GD(0x0096a5c0u);
    range_b=(long double)GD(0x00968950u)-t;
    t=(long double)base_a*(long double)GD(0x0096a5c0u);
    range_a=(long double)GD(0x00968950u)-t;
    t=(long double)base_c*(long double)GD(0x0096a5c0u);
    b=(s32)((u32)((s32)den_b-1)*(u32)(s32)raw_b+(u32)(s32)(s16)b)/(s32)den_b;
    range_c=(long double)GD(0x00968950u)-t;
    a=(s32)((u32)((s32)den_a-1)*(u32)(s32)raw_a+(u32)(s32)(s16)a)/(s32)den_a;
    c=(s32)((u32)((s32)den_c-1)*(u32)(s32)raw_c+(u32)(s32)(s16)c)/(s32)den_c;
    limit=4000+300*(s32)coefficient;
    cap=(s16)limit>raw_b ? limit : raw_b;
    if ((s16)b >= (s16)cap) b=(s16)limit>raw_b ? limit : raw_b;
    rounded_raw=(long double)raw_b;
    t=(long double)range_b*(long double)GF(0x0096d210u);
    t=(long double)rounded_raw-t;
    rounded_target=(long double)(s16)b;
    if (!(t>(long double)rounded_target)) {
        t=(long double)range_b*(long double)GF(0x0096d20cu);
        t=t+(long double)rounded_raw;
        if (t >= (long double)rounded_target) t=rounded_target;
    }
    b=(s32)(u32)(converted=(long long)t); /* __ftol at VA 0x006d3beb */
    cap=(s16)limit>raw_a ? limit : raw_a;
    if ((s16)a >= (s16)cap) a=(s16)limit>raw_a ? limit : raw_a;
    rounded_raw=(long double)raw_a;
    t=(long double)range_a*(long double)GF(0x0096d210u);
    t=(long double)rounded_raw-t;
    rounded_target=(long double)(s16)a;
    if (!(t>(long double)rounded_target)) {
        t=(long double)range_a*(long double)GF(0x0096d208u);
        t=t+(long double)rounded_raw;
        if (t >= (long double)rounded_target) t=rounded_target;
    }
    a=(s32)(u32)(converted=(long long)t); /* __ftol at VA 0x006d3c60 */
    cap=(s16)limit>raw_c ? limit : raw_c;
    if ((s16)c >= (s16)cap) c=(s16)limit>raw_c ? limit : raw_c;
    rounded_raw=(long double)raw_c;
    t=(long double)range_c*(long double)GF(0x0096d1a8u);
    t=(long double)rounded_raw-t;
    rounded_target=(long double)(s16)c;
    if (!(t>(long double)rounded_target)) {
        t=(long double)(100*(s32)coefficient)*(long double)range_c;
        t=t+(long double)rounded_raw;
        if (t >= (long double)rounded_target) t=rounded_target;
    }
    c=(s32)(u32)(converted=(long long)t); /* __ftol at VA 0x006d3cea */
    total=400*(s32)coefficient;
    cap=total/(s32)div_b;
    if (cap<5000) cap=5000; else if (cap>10000) cap=10000;
    if ((s16)cap<=raw_a) cap=raw_a;
    if ((s16)a>(s16)cap) a=cap;
    if ((s16)a<1) a=1;
    cap=total;
    if (cap<5000) cap=5000; else if (cap>10000) cap=10000;
    if ((s16)cap<=raw_b) cap=raw_b;
    if ((s16)b>(s16)cap) b=cap;
    if ((s16)b<1) b=1;
    cap=total/(s32)div_c;
    if (cap<3000) cap=3000; else if (cap>10000) cap=10000;
    if ((s16)cap<=raw_c) cap=raw_c;
    if ((s16)c>(s16)cap) c=cap;
    if ((s16)c<1) c=1;
    *(volatile s16 *)output=(s16)b;
    *(volatile s16 *)(output+2u)=(s16)a;
    *(volatile s16 *)(output+4u)=(s16)c;
}
