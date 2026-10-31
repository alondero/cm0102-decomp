/* Tier-2 C recovery for the 3.9.68 primary executable, VA 0x009448b0
   (859 bytes, image base 0x00400000).

   Convention: cdecl with two stack arguments: a = [esp+4], b = [esp+8] on
   entry; the caller cleans up. The original overwrites both argument slots
   with their absolute values and reuses stack slots as scratch. Those stack
   writes are not observable, so they are modelled as C locals.

   mul_fix() is the 16x16 partial-product form of (x * y) >> 16 (mod 2^32)
   that the listing computes with imul/shr/add on the low and high halves. */
#include "cmre.h"

extern s32 CM_CDECL cm_fn_009446e0(s32 a, s32 b);
extern u32 CM_CDECL cm_fn_00944840(s32 a, s32 b);

static u32 mul_fix(u32 x, u32 y)
{
    u32 lx = x & 0xffffu;
    u32 hx = (x >> 16) & 0xffffu;
    u32 ly = y & 0xffffu;
    u32 hy = (y >> 16) & 0xffffu;

    return ((lx * ly) >> 16) + ly * hx + lx * hy + ((hx * hy) << 16);
}

u32 CM_CDECL cm_fn_009448b0(u32 param_1, u32 param_2)
{
    u32 a;
    u32 b;
    u32 v;
    u32 vabs;
    u32 w;
    u32 q;
    u32 rem;
    u32 divisor;
    s32 sg;
    s32 sg2;
    s32 sg4;
    s32 sg6;
    u32 t1;
    u32 t1abs;
    u32 t2;
    u32 t2abs;
    u32 t3;
    u32 t3abs;
    u32 m;
    u32 m2;
    u32 m3;
    u32 x;
    u32 x2;
    u32 xabs;
    u32 xdiv;
    u32 qx;
    u32 rB;
    u32 dv;
    u32 qBs;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r4;
    u32 r5;
    u32 r6;

    /* 009448b0..009448d8: absolute values; zero operands return the other. */
    a = param_1;
    if ((s32)a < 0) {
        a = 0u - a;
    }
    b = param_2;
    if ((s32)b < 0) {
        b = 0u - b;
    }
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }

    /* 009448e3..009448f6: v = (min >> 1) + max, signed compare. */
    if ((s32)a <= (s32)b) {
        v = (u32)((s32)a >> 1) + b;
    } else {
        v = (u32)((s32)b >> 1) + a;
    }

    /* 009448f8..0094492c: |v| and the sign of a * v for the first quotient. */
    vabs = ((s32)v >= 0) ? v : 0u - v;
    sg = (((s32)a < 0) ? -1 : 1) * (((s32)v >= 0) ? 1 : -1);

    /* 00944930..00944960: a / |v| split into quotient and remainder, then a
       16.16 fixed-point term t1 = ((rem << 16) / divisor + (q << 16)) * sg. */
    w = a;
    q = w / vabs;
    rem = w % vabs;
    divisor = vabs;
    w = q << 16;
    while (rem > 0xffffu) {
        rem = rem >> 1;
        divisor = (u32)((s32)divisor >> 1);
    }
    t1 = ((((rem << 16) / divisor) + w) * (u32)sg);

    /* 00944969..0094498b: sign of a * t1 and |t1|. */
    sg2 = (((s32)a < 0) ? -1 : 1) * (((s32)t1 >= 0) ? 1 : -1);
    t1abs = ((s32)t1 >= 0) ? t1 : 0u - t1;

    /* 009449ca: m = mulfix(|a|, |t1|) * sg2. */
    m = mul_fix(a, t1abs) * (u32)sg2;

    /* 009449cd..00944a03: sign of b * v, and the same split for |b| / |v|. */
    sg = (((s32)b < 0) ? -1 : 1) * (((s32)v >= 0) ? 1 : -1);
    rB = b % vabs;
    qBs = (b / vabs) << 16;
    dv = vabs;
    while (rB > 0xffffu) {
        rB = rB >> 1;
        dv = (u32)((s32)dv >> 1);
    }

    /* 00944a34..00944a45: t2 = ((rB << 16) / dv + (qBs)) * sg. */
    t2 = ((((rB << 16) / dv) + qBs) * (u32)sg);

    /* 00944a4a..00944a7a: sign of b * t2 and |t2|. */
    sg4 = (((s32)b < 0) ? -1 : 1) * (((s32)t2 >= 0) ? 1 : -1);
    t2abs = ((s32)t2 >= 0) ? t2 : 0u - t2;

    /* 00944a7c..00944ab7: m2 = mulfix(|b|, |t2|) * sg4. */
    m2 = mul_fix(b, t2abs) * (u32)sg4;

    /* 00944abc..00944ac6: x = (v + (m2 + m) + 1) >> 1, arithmetic shift. */
    x = (u32)((s32)(v + m2 + m + 1) >> 1);

    /* 00944ac2..00944af4: sign of a * x and |x|. */
    sg = (((s32)a < 0) ? -1 : 1) * (((s32)x >= 0) ? 1 : -1);
    xabs = ((s32)x >= 0) ? x : 0u - x;

    /* 00944b00..00944b32: a / |x| split, then t3 = ((rem << 16) / xdiv + (qx << 16)) * sg. */
    qx = a / xabs;
    rem = a % xabs;
    xdiv = xabs;
    while (rem > 0xffffu) {
        rem = rem >> 1;
        xdiv = (u32)((s32)xdiv >> 1);
    }
    t3 = ((((rem << 16) / xdiv) + (qx << 16)) * (u32)sg);

    /* 00944b37..00944b67: sign of a * t3 and |t3|. */
    sg6 = (((s32)a < 0) ? -1 : 1) * (((s32)t3 >= 0) ? 1 : -1);
    t3abs = ((s32)t3 >= 0) ? t3 : 0u - t3;

    /* 00944b72..00944b97: first helper pair on (|b|, x). */
    r1 = cm_fn_00944840((s32)b, (s32)x);

    /* 00944b9c..00944ba2: second helper on (|b|, r1). */
    r2 = (u32)cm_fn_009446e0((s32)b, (s32)r1);

    /* 00944ba7..00944bcb: m3 = mulfix(|a|, |t3|) * sg6. */
    m3 = mul_fix(a, t3abs) * (u32)sg6;

    /* 00944bbe..00944bd4: x2 = (m3 + (x + r2) + 1) >> 1, arithmetic shift. */
    x2 = (u32)((s32)(m3 + x + r2 + 1) >> 1);

    /* 00944bd6..00944bdf: helper pair on (|a|, x2). */
    r3 = cm_fn_00944840((s32)a, (s32)x2);
    r4 = (u32)cm_fn_009446e0((s32)a, (s32)r3);

    /* 00944be4..00944bf3: helper pair on (|b|, x2). */
    r5 = cm_fn_00944840((s32)b, (s32)x2);
    r6 = (u32)cm_fn_009446e0((s32)b, (s32)r5);

    /* 00944bfb..00944c04: (r6 + (x2 + r4) + 1) >> 1, arithmetic shift. */
    return (u32)((s32)(r6 + x2 + r4 + 1) >> 1);
}
