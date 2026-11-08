/* Preferred-image VA 006f4aa0, base 00400000; original PE SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The discovered game call contract is void (return-proof-6f4aa0/VERDICT.md).
   Physical ECX from 006c38c0 affects the discarded full EAX return, including
   006f54ea and 006f55a4. Its high bits do not affect the stored AX. This source
   does not represent arbitrary computed/external callers observing that return.
   Callees below remain original-code boundaries, not composed replacements. */
#include "cmre.h"
/* Native 006c38c0: ECX receiver, [ESP+4] object, RET 4. Only AL/AX is used
   here. Incoming EAX/EDX are not input values of those result bits: valid
   index loads AX from the row; invalid index XORs AX. Accepted helper C has
   a different, wider explicit-register input declaration; it is not linked. */
extern u32 CM_THISCALL cm_fn_006c38c0(u32 group, u32 object);
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);
#define M(t, p) (*(volatile t *)(u32)(p))
#define B(o) M(u8, self + (o))
#define S(o) M(s8, self + (o))
#define W(o) M(s16, self + (o))
#define P(o) M(u32, self + (o))
#define F(o) M(float, self + (o))
#define K(t, p) CM_GLOBAL(volatile t, p)

/* Avoid SSE3 FISTTP: the registered MSVC cast saves/restores the x87 CW. */
__attribute__((target("no-sse3")))
void CM_THISCALL cm_fn_006f4aa0(u32 self)
{
    /* Native L is ESP after SUB ESP,16, before saved EBX/ESI/EDI.
       [ESP+0c/10/14/18] after those saves are L+0/4/8/12. */
    union { u32 align; u8 bytes[16]; } scratch;
    u8 b, b2, bl, cl;
    s8 sb;
    s16 rating, threshold;
    u32 group, row, flags, n, m, q;
    s32 v, floor_value, saved;
    long double x, y;
    volatile float *ratio = (volatile float *)(scratch.bytes + 0);
    volatile float *rounded = (volatile float *)(scratch.bytes + 4);
    volatile s32 *first = (volatile s32 *)(scratch.bytes + 8);
    volatile s32 *second = (volatile s32 *)(scratch.bytes + 12);

    sb = S(0x2b);
    if (sb > 0) B(0x2b) = (u8)(sb - 1);
    else if (sb < 0) B(0x2b) = 0;
    x = (long double)F(0x1a2);
    x *= (long double)K(float, 0x0096d364);
    group = P(0x69);
    F(0x1a2) = (float)x; /* FST does not round the live ST(0). */
    sb = M(s8, group + 0x5d);
    floor_value = (25 - (s32)sb) * 2;
    *rounded = (float)floor_value;
    if (!(x > (long double)*rounded)) x = (long double)*rounded;
    F(0x1a2) = (float)x;
    if (x > (long double)K(double, 0x009688a8)) P(0x1a2) = 0x42c80000;

    n = B(0x10); m = B(0xc);
    if (n + m < 2 && B(6) < 7 && B(3) < 4 && B(0x11) < 6) {
        saved = W(0x35);
        q = cm_fn_0090cfc0(1500);
        if (saved > (s32)(q + 0x203aU)) {
            v = (s32)((u32)saved - 0x203aU);
            x = (long double)v;
            x *= (long double)K(double, 0x0096d5c0);
            x += (long double)K(double, 0x0096d760);
            /* Original __ftol call 006f4b6f: EDX:EAX truncation, then AX. */
            W(0x35) = (s16)(u32)(long long)x;
        }
    }
    b = B(1);
    if (b > 50) {
        n = B(3);
        rating = W(0x35);
        v = ((s32)b + (s32)n * 20 + 275) * 20;
        if (v <= rating) v = rating;
        b2 = B(2);
        W(0x35) = (s16)v;
        if (b2 != 0) {
            x = (long double)b / (long double)b2;
            if (x > (long double)K(double, 0x00969040)) {
                y = (long double)(s16)v;
                x *= (long double)K(float, 0x0096d2dc);
                x += (long double)K(float, 0x0096d75c);
                if (x > y) W(0x35) = (s16)(v + 5);
            }
        }
    }
    b = B(0x10);
    if (b != 0) {
        v = (s32)b * 425 + 6000;
        rating = W(0x35);
        if (v <= rating) v = rating;
        W(0x35) = (s16)v;
    }
    b2 = B(0xc);
    if (b2 != 0 && (n = (u32)b2 + b) > 1) {
        rating = W(0x35);
        v = (s32)n * 375 + 6500;
        if (v <= rating) v = rating;
        W(0x35) = (s16)v;
    }
    group = P(0x19e);
    flags = group ? cm_fn_006c38c0(group, self) : 0;
    if (!(flags & 1)) goto flag2;
    n = B(0xf);
    q = cm_fn_0090cfc0(n * n);
    if (q == 0) {
        rating = W(0x35);
        if (rating < 7350) W(0x35) = (s16)(rating + 1);
    }
    sb = S(0x1f);
    if (sb > 30) {
        if (B(0xf) != 0) return;
        rating = W(0x35); v = 7000; goto increment_below;
    }
    b = B(0x27); group = P(0x19e);
    row = group + (b == 0 ? 54U : 0U) + 0xf5bfU;
    rating = W(0x35);
    n = M(u8, row);
    v = ((s32)n + 14) * 500;
    if (v >= rating) v = rating;
    b = B(0xf);
    W(0x35) = (s16)v;
    if (!b) {
        n = M(u8, row);
        q = (n + 20) * 300;
        if ((s32)q <= (s16)v) q = (u32)(s32)(s16)v;
        W(0x35) = (s16)q;
        b2 = M(u8, row);
        v = ((b2 < 2 ? b2 : 2) + 28) * 250;
        rating = (s16)q; goto increment_below;
    }
    if ((s16)v <= 4250) return;
    if (b > 1 && cm_fn_0090cfc0(b) != 0) {
        b = B(0xf);
        if (b > 2 && W(0x35) > 7750) { W(0x35) = 7750; return; }
        rating = W(0x35);
        if (rating > 8750) { W(0x35) = (s16)(rating - 25); return; }
        if (rating > 7250) { W(0x35) = (s16)(rating - b); return; }
        W(0x35) = (s16)(rating - 1); return;
    }
    b = B(0x27); group = P(0x19e);
    n = M(u8, group + (b == 0 ? 54U : 0U) + 0xf5bfU);
    rating = W(0x35);
    if (rating <= ((s32)n + 25) * 250) return;
    v = M(s16, group + 0x8ed0);
    if (cm_fn_0090cfc0((u32)(90 - v / 11)) != 0) return;
    goto decrement;

flag2:
    group = P(0x19e); flags = group ? cm_fn_006c38c0(group, self) : 0;
    if (!(flags & 2)) goto flag4;
    if (cm_fn_0090cfc0(75) == 0) W(0x35) = (s16)(W(0x35) - 1);
    if (!B(6) && !B(9) && !B(3) && !B(0x10)) {
        rating = W(0x35);
        if (rating > 7500) W(0x35) = (s16)(rating - 10);
    }
    b = B(0x27); group = P(0x19e);
    if (M(u8, group + (b ? 0xf5bdU : 0xf5f3U))) return;
    goto increment_5750;

flag4:
    group = P(0x19e); flags = group ? cm_fn_006c38c0(group, self) : 0;
    if (!(flags & 4)) goto flag8;
    if (cm_fn_0090cfc0(75) == 0) W(0x35) = (s16)(W(0x35) - 1);
    cl = B(4); n = B(7); *second = cl;
    m = (u32)cl + n;
    if (m > 9) {
        n = B(5); q = B(8); *first = (s32)n;
        x = (long double)(q + n) / (long double)m;
        n = B(9); q = B(6);
        *ratio = (float)x;
        if (n + q) {
            x = (long double)(n + q) * (long double)K(double, 0x00967878);
            x += (long double)*ratio; *ratio = (float)x;
        }
        if (cl > 4) {
            x = (long double)*first / (long double)*second;
            *rounded = (float)x;
            if (!(x >= (long double)K(double, 0x0096a5d0))) {
                group = P(0x19e); flags = group ? cm_fn_006c38c0(group, self) : 0;
                x = (long double)*rounded - (long double)K(double, 0x0096a5d0);
                x *= (long double)(flags & 0x880 ? K(double, 0x00968918) : K(double, 0x009689e0));
                *ratio = (float)((long double)*ratio - x);
            }
        }
        b = B(7);
        if (b > 4) {
            n = B(8); *second = (s32)n;
            x = (long double)*second / (long double)b;
            *rounded = (float)x;
            if (!(x >= (long double)K(double, 0x0096a5d0))) {
                group = P(0x19e); flags = group ? cm_fn_006c38c0(group, self) : 0;
                x = (long double)*rounded - (long double)K(double, 0x0096a5d0);
                x *= (long double)(flags & 0x880 ? K(double, 0x00967880) : K(double, 0x009689e0));
                *ratio = (float)((long double)*ratio - x);
            }
        }
        b = B(0x27); group = P(0x19e);
        sb = M(s8, group + (b ? 0xf5bdU : 0xf5f3U));
        b = B(0xc);
        n = (u32)(29 - (s32)sb) * 250U;
        if (b) n += (u32)b * 500U;
        b = B(0x10);
        if (b) n += (u32)b * 250U;
        threshold = (s16)n;
        x = (long double)*ratio;
        if (!(x >= (long double)K(double, 0x00968fe0))) {
            if (W(0x35) > threshold) W(0x35) = threshold;
        } else {
            x = (long double)*ratio;
            if (x > (long double)K(double, 0x0096afb8)) {
                if (W(0x35) < threshold) W(0x35) = threshold;
            }
        }
    }
    bl = B(6);
    if (!bl && !B(9) && !B(3) && !B(0x10)) {
        rating = W(0x35);
        if (rating > 7500) W(0x35) = (s16)(rating - 10);
    }
    b = B(0x27); group = P(0x19e);
    b2 = M(u8, group + (b ? 0xf5bdU : 0xf5f3U));
    if (!b2) {
        if (!bl && !B(9) && !B(3) && !B(0x10) && !B(0xc)) {
            b = B(2);
            if (b <= 20) goto increment_5750;
            n = B(1);
            x = (long double)n / (long double)b;
            y = (long double)K(double, 0x00969040);
            rating = W(0x35);
            if (!(x > y)) { v = 5750; goto increment_below; }
            goto plus5;
        }
        rating = W(0x35); goto plus5;
    }
    b2 = M(u8, group + (b ? 0xf5bdU : 0xf5f3U));
    if (b2 == 1) {
        if (bl || B(9) || B(3) || B(0x10) || B(0xc)) goto increment_6250;
        rating = W(0x35);
        if (rating <= 7250) return;
        W(0x35) = (s16)(rating - 1); return;
    }
    sb = M(s8, group + (b ? 0xf5bdU : 0xf5f3U));
    threshold = (s16)((u32)(31 - (s32)sb) * 250U);
    if (threshold < 6750) threshold = 6750;
    if (W(0x35) <= threshold) return;
    flags = group ? cm_fn_006c38c0(group, self) : 0;
    b = B(6);
    if (flags & 0x880) {
        if (b || B(9) || B(3) || B(0x10) || B(0xc)) goto decrement;
        goto subtract_group;
    }
    if (b || B(9) || B(3) || B(0x10) || B(0xc)) goto subtract_group;
    b = B(0x27); group = P(0x19e);
    sb = M(s8, group + (b ? 0xf5bdU : 0xf5f3U));
    W(0x35) = (s16)((u16)W(0x35) - (u16)((s32)sb * 5)); return;
subtract_group:
    b = B(0x27); group = P(0x19e);
    sb = M(s8, group + (b ? 0xf5bdU : 0xf5f3U));
    W(0x35) = (s16)(W(0x35) - sb); return;

flag8:
    group = P(0x19e); flags = group ? cm_fn_006c38c0(group, self) : 0;
    if (!(flags & 8)) goto flag16;
    if (cm_fn_0090cfc0(500) == 0) W(0x35) = (s16)(W(0x35) - 1);
    cl = B(6);
    if (!cl && !B(9) && !B(3)) {
        rating = W(0x35);
        if (rating > 7500) W(0x35) = (s16)(rating - 10);
    }
    b = B(0x27); group = P(0x19e);
    if (M(u8, group + (b ? 0xf5bdU : 0xf5f3U))) return;
    if (!cl && !B(9) && !B(3) && !B(0x10)) goto increment_6250;
    rating = W(0x35);
plus5:
    if (rating < 5750) { W(0x35) = (s16)(rating + 5); return; }
    v = 6750; goto increment_below;
flag16:
    group = P(0x19e); flags = group ? cm_fn_006c38c0(group, self) : 0;
    if (!(flags & 16)) goto flag32;
    if (cm_fn_0090cfc0(1000) == 0) W(0x35) = (s16)(W(0x35) - 1);
    if (B(0xc) || B(0x10)) return;
    if (B(3) || B(0xb)) goto minus5;
    goto minus10;
flag32:
    group = P(0x19e); flags = group ? cm_fn_006c38c0(group, self) : 0;
    if (!(flags & 32)) goto flag64;
    if (cm_fn_0090cfc0(500) == 0) W(0x35) = (s16)(W(0x35) - 1);
    b = B(0xc);
    if (b) goto plus_field;
    if (B(0x10)) return;
    if (B(3) || B(0xb)) goto minus5;
    goto minus10;
flag64:
    group = P(0x19e); flags = group ? cm_fn_006c38c0(group, self) : 0;
    if (!(flags & 64)) return;
    if (cm_fn_0090cfc0(150) == 0) W(0x35) = (s16)(W(0x35) - 1);
    b = B(0xc);
    if (b) goto plus_field;
    if (B(0x10) || B(0xb)) return;
    if (B(3) || B(9)) {
        rating = W(0x35);
        if (rating > 7750) W(0x35) = (s16)(rating - 5);
        return;
    }
minus10:
    rating = W(0x35);
    if (rating > 7250) W(0x35) = (s16)(rating - 10);
    return;
minus5:
    rating = W(0x35);
    if (rating > 7250) W(0x35) = (s16)(rating - 5);
    return;
plus_field:
    rating = W(0x35);
    if (rating < (s32)b * 500 + 6750) W(0x35) = (s16)(rating + (s32)b * 5);
    return;
increment_6250:
    rating = W(0x35); v = 6250; goto increment_below;
increment_5750:
    rating = W(0x35); v = 5750;
increment_below:
    if ((s32)rating < v) W(0x35) = (s16)(rating + 1);
    return;
decrement:
    W(0x35) = (s16)(W(0x35) - 1);
}
