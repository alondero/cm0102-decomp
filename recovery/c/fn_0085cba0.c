/* Root independent delta/lowering review complete; actual gates and controls pending. */
#include "cmre.h"
/* Independent full source review complete; canonical gate and physical-site controls pending. */

/* Preferred-image VAs, base 00400000; original SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX is retained as the physical receiver channel. Its saved value is
   subsequently forwarded only to 0085d8a0, whose entry overwrites ECX.
   Both root returns pop eight stack bytes and return full EAX.
   All original scratch addresses share one uninitialized allocation.
   L denotes ESP immediately after SUB ESP,34h, before three saved registers.
   The original register-save slots are below L and are not scratch objects. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define Q(a) CM_GLOBAL(volatile double, (u32)(a))
#define PUT64(a,lo,hi) do { D(a) = (lo); D((a)+4u) = (hi); } while (0)

extern u32 CM_THISCALL cm_fn_004a4850(u32 self, u32 target);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 target);
extern u32 CM_STDCALL cm_fn_007e6f20(u32 input);
/* Native entry loads its object from the stack, ignores incoming ECX,
   returns ST0 normally, and executes RET 14h on every return. */
extern long double CM_STDCALL cm_fn_0085d8a0(u32 object, u32 point,
                                            u32 other, double incoming);
/* PUSH ECX is a saved register, not an argument; all returns are plain RET.
   The caller supplies one DWORD and performs the cleanup. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0085cba0(u32 self, u32 object, u32 output)
{
    union { double alignment; u8 bytes[0x34]; } scratch;
    u32 L = (u32)scratch.bytes;
    u32 a, c, d, si, di, p, v, index;
    u8 kind, left, right, flag;
    s32 numerator;
    s16 word;
    long double t, factor, residual, selected;

    D(L + 0x0c) = self;
    if (object == 0)
        return D(0x00a835ac);

    a = D(object + 0x1c);
    numerator = (s32)(s16)W(a + 0x80);
    /* Signed high-product/SAR/sign correction implements division by 500
       over the entire sign-extended WORD input domain. */
    c = (u32)(numerator / 500);
    d = D(object + 0x20);
    numerator = (s32)(s16)W(d + 0x80);
    d = (u32)(numerator / 500);
    B(L + 6) = (u8)c;
    B(L + 7) = (u8)d;
    if ((s8)(u8)c < 1) B(L + 6) = 1;
    if ((s8)(u8)d < 1) B(L + 7) = 1;
    B(L + 5) = 0;

prefix_loop:
    c = D(object + 0x14);
    index = (u32)(s32)(s8)B(L + 5);
    v = L + 0x24u + index * 8u;
    if (c != 0) {
        d = (u32)(s32)(s16)W(c + 0x69);
        D(L + 0x1c) = d;
        t = (long double)(s32)D(L + 0x1c);
        Q(v) = (double)t;
        if (t >= (long double)Q(0x00967890)) goto initial_ready;
    }
    PUT64(v, 0, 0x3ff00000u);
initial_ready:
    kind = B(object + 0x3a);
    if (kind == 8 || kind == 10 || kind == 6) {
        t = (long double)Q(v);
        t += (long double)Q(0x00968950);
        Q(v) = (double)t;
    }
    a = (u32)cm_fn_005c6710((void *)D(0x00ae2c74), c);
    if (a != 0) goto default_adder;
    c = D(0x00adadfc);
    if (c == 0) goto default_adder;
    a = D(object + 0x14);
    d = D(a);
    if (D(c + d * 4u) == 0) goto default_adder;
    c = D(0x00b74434);
    if (c == 0) goto default_adder;
    a = cm_fn_007e6f20(a);
    if (a != 0) goto default_adder;
    a = D(object + 0x14);
    d = D(0x00adadfc);
    c = D(a);
    a = D(d + c * 4u);
    if (B(a + 0x43) == 2) goto default_adder;
    left = B(a + 0x3c);
    right = B(object + 0x34);
    right = (u8)(right - left);
    D(L + 0x1c) = (u32)(s32)(s8)right;
    t = (long double)(s32)D(L + 0x1c);
    if (!(t >= (long double)Q(0x0096fe28))) {
        t = (long double)Q(0x0096fe28);
    } else if (t > (long double)Q(0x009688a0)) {
        t = (long double)Q(0x009688a0);
    }
    goto add_ready;
default_adder:
    t = (long double)Q(0x009689a0);
add_ready:
    t += (long double)Q(v);
    Q(L + 0x14) = (double)t;
    a = D(L + 0x14);
    c = D(L + 0x18);
    D(v) = a;
    D(v + 4) = c;

    if (index == 0 || index == 1) {
        if (index == 0) {
            left = B(L + 6);
            right = B(L + 7);
        } else {
            left = B(L + 7);
            right = B(L + 6);
        }
        if (left != 0 && ((s8)left < 15 || (s8)left < (s8)right)) {
            D(L + 0x1c) = (u32)(s32)(s8)right;
            t = (long double)(s32)D(L + 0x1c);
            D(L + 0x1c) = (u32)(s32)(s8)left;
            t /= (long double)(s32)D(L + 0x1c);
            t *= (long double)Q(L + 0x14);
            Q(v) = (double)t;
        }
        if ((s8)right > 17) {
            t = (long double)Q(v);
            t *= (long double)Q(0x009689c0);
            Q(v) = (double)t;
        }
        if ((s8)right > 12 && (s8)left < 14) {
            t = (long double)Q(v);
            t *= (long double)Q(0x00968928);
            Q(v) = (double)t;
        }
        a = D(object + (index == 0 ? 0x1cu : 0x20u));
        a = D(a + 0x57);
        if (a != 0) {
            word = (s16)W(a + 0x69);
            if (word != 0) {
                D(L + 0x1c) = (u32)(s32)word;
                t = (long double)(s32)D(L + 0x1c);
                t = (long double)Q(v) / t;
                goto divisor_ready;
            }
        }
        kind = B(object + 0x3a);
        t = (long double)Q(v);
        if (kind == 10) t *= (long double)Q(0x00968918);
        else t *= (long double)Q(0x00967888);
divisor_ready:
        Q(v) = (double)t;
    }

    kind = B(object + 0x3a);
    if (kind == 8) {
        word = (s16)W(object + 0x30);
        if (word == 0x96) {
            a = D(object + (index == 0 ? 0x1cu : 0x20u));
            if ((s16)W(a + 0x80) < 0x1676) {
                if (!((long double)Q(v) >= (long double)Q(0x00968e38)))
                    PUT64(v, 0, 0x400c0000u);
            } else {
                if (!((long double)Q(v) >= (long double)Q(0x009690a8)))
                    PUT64(v, 0, 0x40060000u);
            }
        } else {
            if (word == 0x82 &&
                !((long double)Q(v) >= (long double)Q(0x00968e18))) {
                a = D(object + (index == 0 ? 0x1cu : 0x20u));
                if ((s16)W(a + 0x80) < 0x1676) {
                    if (!((long double)Q(v) >= (long double)Q(0x00968930)))
                        PUT64(v, 0, 0x40080000u);
                } else {
                    if (!((long double)Q(v) >= (long double)Q(0x00968ec8)))
                        PUT64(v, 0, 0x40020000u);
                }
            } else {
                a = D(object + (index == 0 ? 0x1cu : 0x20u));
                word = (s16)W(a + 0x80);
                if (word < 0x109a) {
                    if (!((long double)Q(v) >= (long double)Q(0x00968f98)))
                        PUT64(v, 0, 0x3ffc0000u);
                } else if (word < 0x1676) {
                    if (!((long double)Q(v) >= (long double)Q(0x00968928)))
                        PUT64(v, 0, 0x3ff40000u);
                } else {
                    if (!((long double)Q(v) >= (long double)Q(0x009689b8)))
                        PUT64(v, 0xcccccccdu, 0x3ff0ccccu);
                }
            }
        }
    }

    if (kind == 6) {
        word = (s16)W(object + 0x30);
        if (word == 0x96) goto special_150;
        if (word == 0x82 &&
            !((long double)Q(v) >= (long double)Q(0x00968e18))) {
            PUT64(v, 0, 0x40040000u);
        } else {
            if (!((long double)Q(v) >= (long double)Q(0x00968928)))
                PUT64(v, 0, 0x3ff40000u);
        }
        goto prefix_done;
    }
    if (W(object + 0x30) == 0x96 && kind == 5) {
special_150:
        a = D(object + (index == 0 ? 0x1cu : 0x20u));
        if ((s16)W(a + 0x80) < 0x109a) {
            if (!((long double)Q(v) >= (long double)Q(0x00968e38)))
                PUT64(v, 0, 0x400c0000u);
        } else {
            if (!((long double)Q(v) >= (long double)Q(0x009690a8)))
                PUT64(v, 0, 0x40060000u);
        }
        goto prefix_done;
    }
    if (W(object + 0x32) == 0xa0 && kind == 5) {
        if (index == 0) {
            a = D(object + 0x1c);
            if ((s16)W(a + 0x80) < 0x109a &&
                !((long double)Q(L + 0x24) >= (long double)Q(0x0096fe20))) {
                PUT64(L + 0x24, 0, 0x400a0000u);
            } else {
                if (!((long double)Q(L + 0x24) >= (long double)Q(0x00968e18)))
                    PUT64(L + 0x24, 0, 0x40040000u);
            }
        } else {
            if (!((long double)Q(v) >= (long double)Q(0x00968e18)))
                PUT64(v, 0, 0x40040000u);
        }
        goto prefix_done;
    }
    if (kind == 10 &&
        !((long double)Q(v) >= (long double)Q(0x00968928))) {
        PUT64(v, 0, 0x3ff40000u);
        goto prefix_done;
    }
    if (kind == 9 &&
        !((long double)Q(v) >= (long double)Q(0x009689b0))) {
        PUT64(v, 0x9999999au, 0x3fe99999u);
        goto prefix_done;
    }
    if (!((long double)Q(v) >= (long double)Q(0x00967880)))
        PUT64(v, 0x9999999au, 0x3fb99999u);
prefix_done:
    B(L + 5) = (u8)(index + 1u);
    if ((s8)B(L + 5) < 2) goto prefix_loop;

    a = D(object + 0x18);
    if (a == 0) goto simple_scores;
    if (B(object + 0x3c) != 0) goto simple_scores;
    a = D(a + 0x38);
    if (a == 0) {
        a = cm_fn_0090cfc0(D(0x00ae2354));
        d = D(0x00ae23ac);
        di = d + a * 56u;
    } else di = a;
    a = D(object + 0x1c);
    D(L + 0x10) = di;
    a = D(a + 0x69);
    if (a != 0) {
        a = D(a + 0x38);
        if (a != 0) { si = a; goto first_point_ready; }
    }
    a = cm_fn_0090cfc0(D(0x00ae2354));
    d = a * 7u;
    a = D(0x00ae23ac);
    si = a + d * 8u;
first_point_ready:
    c = D(object + 0x20);
    D(L + 0x1c) = si;
    a = D(c + 0x69);
    if (a != 0) {
        a = D(a + 0x38);
        if (a != 0) goto second_point_ready;
    }
    a = cm_fn_0090cfc0(D(0x00ae2354));
    d = D(0x00ae23ac);
    a = d + a * 56u;
second_point_ready:
    /* Original pushes: incoming.high, incoming.low, other, point, object.
       ECX receives the saved root channel but is dead in the callee.
       The slot for the second other is written after these pushes. */
    c = D(L + 0x28);
    d = D(L + 0x24);
    (void)D(L + 0x0c);
    D(L + 0x14) = a;
    /* Reconstitute the pushed double from its exact two DWORDs. */
    {
        union { struct { u32 lo, hi; } parts; double value; } incoming;
        incoming.parts.lo = d;
        incoming.parts.hi = c;
        factor = cm_fn_0085d8a0(object, di, si, incoming.value);
    }
    if (!(factor >= (long double)Q(0x00968f20)))
        factor = (long double)Q(0x00968f20);
    a = D(object + 0x1c);
    t = (long double)(s32)D(a + 0x73);
    t *= (long double)Q(L + 0x24);
    t *= factor;
    /* Original __ftol site 0085d2f8; factor remains owned below ST0. */
    si = (u32)(long long)t;
    c = D(L + 0x30);
    d = D(L + 0x2c);
    a = D(L + 0x14);
    p = D(L + 0x10);
    D(L + 8) = si;
    /* Original FSTP discards the retained first factor before this call. */
    {
        union { struct { u32 lo, hi; } parts; double value; } incoming;
        incoming.parts.lo = d;
        incoming.parts.hi = c;
        factor = cm_fn_0085d8a0(object, p, a, incoming.value);
    }
    if (!(factor >= (long double)Q(0x00968f20)))
        factor = (long double)Q(0x00968f20);
    a = D(object + 0x20);
    d = D(a + 0x77);
    di = D(a + 0x73);
    d += di;
    D(L + 0x0c) = d;
    t = (long double)(s32)D(L + 0x0c);
    t *= (long double)Q(L + 0x2c);
    t *= factor;
    t *= (long double)Q(0x00968968);
    /* Original __ftol site 0085d355; retained factor is then discarded. */
    D(L + 0x0c) = (u32)(long long)t;
    a = D(object + 0x14);
    if (a == 0) goto compare_points;
    d = D(object + 0x1c);
    if (d == 0) goto compare_points;
    c = D(d + 0x57);
    if (a != c) goto compare_points;
    a = D(c);
    c = D(0x00adadfc);
    a = D(c + a * 4u);
    if (a == 0) goto compare_points;
    c = D(a + 8);
    if (c == 0) goto compare_points;
    a = cm_fn_004a4850(c, d);
    if (a == 0) goto compare_points;
    d = D(a + 0x19);
    if ((s32)d > 1) {
        if ((s32)d < 4) goto add_50;
        c = D(object + 0x14);
        word = (s16)W(c + 0x69);
        if (word < 12) goto add_50;
        c = (u32)(s32)word;
        if ((s32)d < 7) si += c * 100u;
        else si += c * 150u;
        goto store_adjusted;
add_50:
        d = D(object + 0x14);
        c = (u32)(s32)(s16)W(d + 0x69);
        si += c * 50u;
        goto store_adjusted;
    }
    c = D(a + 0x41);
    if ((s32)c > 2) {
        if ((s32)c >= 4) {
            d = D(object + 0x14);
            word = (s16)W(d + 0x69);
            if (word >= 12) {
                si += (u32)(s32)word * 50u;
                goto store_adjusted;
            }
        }
        d = D(object + 0x14);
        c = (u32)(s32)(s16)W(d + 0x69);
        si += c * 25u;
        goto store_adjusted;
    }
    if ((s32)D(a + 0x2d) > 2) {
        c = D(object + 0x14);
        d = (u32)(s32)(s16)W(c + 0x69);
        c = 0u - d;
        c = (c << 2) - d;
        si += c * 5u;
        goto store_adjusted;
    }
    if ((s32)D(a + 0x55) > 3) {
        c = D(object + 0x14);
        c = (u32)(s32)(s16)W(c + 0x69);
        d = 0u - c;
        d = (d << 2) - c;
        si += d * 2u;
store_adjusted:
        D(L + 8) = si;
    }
    flag = B(a + 0x74);
    right = B(a + 0x75);
    B(L + 7) = flag;
    if (flag == right) goto compare_points;
    if (B(a + 4) <= 4) goto compare_points;
    right = B(a + 0x73);
    left = B(a + 0x72);
    /* XOR AX and MOVZX DI leave upper halves intact. All subsequent
       consumers of this subtraction use AX, so its low WORD is sufficient. */
    word = (s16)(u16)((u32)left - (u32)right);
    if (left < 4 && word <= 0) {
        if (right == 1) word = 4;
        else if (right < 4) word = 2;
        else {
            D(L + 8) = (u32)(s32)word;
            t = (long double)(s32)D(L + 8);
            t *= (long double)Q(0x009689e0);
            /* Original __ftol site 0085d4d5. */
            a = (u32)(long long)t;
            word = (s16)(u16)a;
            flag = B(L + 7);
        }
    }
    c = D(object + 0x14);
    c = (u32)(s32)(s16)W(c + 0x69);
    c *= (u32)(s32)word;
    if (left < 4 && flag == 1) c *= 5u;
    else c *= 3u;
    si += c * 5u;
    D(L + 8) = si;

compare_points:
    d = D(L + 0x10);
    a = D(L + 0x1c);
    if (d == a) {
        a = D(object + 0x1c);
        c = D(a + 0x7b);
        if ((s32)si > (s32)c) {
            si = c;
            D(L + 8) = si;
        } else {
            a = D(a + 0x77);
            if ((s32)si < (s32)a) {
                si = a;
                D(L + 8) = si;
            }
        }
    }
    if (d == D(L + 0x14)) {
        a = D(object + 0x20);
        d = D(L + 0x0c);
        c = D(a + 0x7b);
        if ((s32)d > (s32)c) D(L + 0x0c) = c;
        else {
            a = D(a + 0x77);
            if ((s32)d < (s32)a) D(L + 0x0c) = a;
        }
    }
    goto scores_ready;
simple_scores:
    a = D(object + 0x1c);
    t = (long double)(s32)D(a + 0x73);
    t *= (long double)Q(L + 0x24);
    /* Original __ftol site 0085d571. */
    a = (u32)(long long)t;
    c = D(object + 0x20);
    si = a;
    D(L + 8) = si;
    t = (long double)(s32)D(c + 0x73);
    t *= (long double)Q(L + 0x2c);
    /* Original __ftol site 0085d586. */
    D(L + 0x0c) = (u32)(long long)t;
scores_ready:
    di = D(object + 0x1c);
    p = D(object + 0x20);
    if (D(di + 0xb3) == p) goto direct_relation;
    if (D(di + 0xb7) == p) goto direct_relation;
    if (D(di + 0xbb) == p) goto direct_relation;
    c = D(di + 0x69);
    if (c == 0) goto first_relation_done;
    a = D(p + 0x69);
    if (a == 0) goto first_relation_done;
    c = D(c + 0x38);
    if (c != D(a + 0x38)) goto first_relation_done;
    t = (long double)(s32)D(L + 8);
    t *= (long double)Q(0x00969588);
    goto first_relation_cast;
direct_relation:
    t = (long double)(s32)D(L + 8);
    t *= (long double)Q(0x00968940);
first_relation_cast:
    /* Original __ftol site 0085d5dc. */
    si = (u32)(long long)t;
    D(L + 8) = si;
first_relation_done:
    if (D(p + 0xb3) == di) goto second_relation;
    if (D(p + 0xb7) == di) goto second_relation;
    if (D(p + 0xbb) != di) goto second_relation_done;
second_relation:
    t = (long double)(s32)D(L + 0x0c);
    t *= (long double)Q(0x00969588);
    /* Original __ftol site 0085d60c. */
    D(L + 0x0c) = (u32)(long long)t;
second_relation_done:
    if (B(object + 0x3c) == 0 && B(object + 0x3b) != 0xff) {
        t = (long double)(s32)D(L + 8);
        t *= (long double)Q(0x009689b8);
        /* Original __ftol site 0085d62f. The scratch score is NOT updated. */
        a = (u32)(long long)t;
        t = (long double)(s32)D(L + 0x0c);
        si = a;
        t *= (long double)Q(0x00969520);
        /* Original __ftol site 0085d640. */
        D(L + 0x0c) = (u32)(long long)t;
    }
    c = D(object + 0x14);
    if (c == 0 || di == 0) goto minimum_done;
    a = D(di + 0x57);
    if (c != a) goto minimum_done;
    if (B(di + 0x6d) == 0) goto minimum_done;
    word = (s16)W(a + 0x69);
    if (word > 12) {
        a = (u32)(s32)(s16)(u16)((u16)word - 12u);
        a = (a + 3u) * 2000u;
        if ((s32)si < (s32)a) si = a;
    } else if (word > 8) {
        a = (u32)(s32)(s16)(u16)((u16)word - 8u);
        a = (a + 4u) * 750u;
        if ((s32)si < (s32)a) si = a;
    } else if (word > 4) {
        a = (u32)(s32)(s16)(u16)((u16)word - 4u);
        a = (a + 2u) * 500u;
        if ((s32)si < (s32)a) si = a;
    } else {
        a = (u32)(s32)word;
        c = a * 250u;
        if ((s32)si < (s32)c) si = c;
        else {
            di = a * 1500u;
            if ((s32)si > (s32)di) {
                si -= di;
                D(L + 0x1c) = si;
                t = (long double)(s32)D(L + 0x1c);
                t *= (long double)Q(0x00968f30);
                /* Original __ftol site 0085d724. */
                a = (u32)(long long)t;
                di -= a;
                si = di;
            }
        }
    }
minimum_done:
    c = D(object + 0x18);
    if (c != 0) {
        a = D(c + 0x3c);
        if ((s32)a < 1500) {
            a = (u32)(s32)(s8)B(L + 6);
            a *= 1000u;
            D(c + 0x3c) = a;
            a = D(object + 0x18);
            if ((s32)D(a + 0x3c) < 1500) D(a + 0x3c) = 1500;
        }
    }
    c = D(L + 0x0c);
    a = c + si;
    if (a == 0) {
        si = cm_fn_0090cfc0(50);
        ++si;
        a = cm_fn_0090cfc0(15);
        ++a;
        D(L + 0x0c) = a;
        goto final_random;
    }
    a = D(object + 0x18);
    if (a == 0) {
        if ((s32)si > 40000) si = 40000;
        if ((s32)c > 25000) D(L + 0x0c) = 25000;
        goto final_random;
    }
    flag = B(object + 0x3c);
    di = D(a + 0x3c);
    if (flag == 0) {
        c = D(object + 0x20);
        D(L + 0x1c) = di;
        Q(L + 0x14) = (double)(long double)(s32)D(L + 0x1c);
        residual = (long double)(s32)D(c + 0x73);
        residual *= (long double)Q(L + 0x24);
        residual = (long double)Q(L + 0x14) - residual;
        t = (long double)Q(L + 0x14);
        t *= (long double)Q(0x00968f20);
        Q(L + 0x1c) = (double)t;
        if (!(residual >= (long double)Q(L + 0x1c))) selected = residual;
        else selected = (long double)Q(L + 0x1c);
        t = (long double)Q(L + 0x14);
        t *= (long double)Q(0x00967888);
        Q(L + 0x14) = (double)t;
        /* FST, not FSTP: compare the unrounded retained value. */
        if (t > selected) {
            t = (long double)Q(L + 0x14);
            /* Original __ftol site 0085d7e0. */
            a = (u32)(long long)t;
        } else {
            if (!(residual >= (long double)Q(L + 0x1c))) t = residual;
            else t = (long double)Q(L + 0x1c);
            /* Original __ftol site 0085d7f8. */
            a = (u32)(long long)t;
        }
    } else {
        /* CDQ; SUB EAX,EDX; SAR EAX,1, with wrapping subtraction. */
        a = di - ((s32)di < 0 ? 0xffffffffu : 0u);
        a = (u32)((s32)a >> 1);
    }
    c = di - a;
    if ((s32)si > (s32)c) si = c;
    if ((s32)D(L + 0x0c) > (s32)a) D(L + 0x0c) = a;
final_random:
    a = cm_fn_0090cfc0(50);
    di = D(L + 0x0c);
    c = di + si;
    c -= a;
    if ((s32)c < 0) {
        a = cm_fn_0090cfc0(150);
        c = a + 45u;
    }
    if (output != 0) {
        D(output) = c;
        D(output + 4) = si;
        D(output + 8) = di;
    }
    return c;
}
