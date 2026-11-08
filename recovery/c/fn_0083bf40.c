#include "cmre.h"
#pragma GCC target("no-sse3")

/* Preferred-image VA 0x0083bf40, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Ret 4 and incoming ECX establish thiscall. Caller 0048b1ad stores full EAX.
   The five pushes at 0083bf4c..6a are club,1,1,0,0 in callee order. */
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag,
                                  u32 person, u32 force);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile s8 *)(u32)(a))
#define F(a) CM_GLOBAL(volatile double, a)

s32 CM_THISCALL cm_fn_0083bf40(u32 self, u32 club)
{
    u32 base, index, value, slot, p, q, result;
    s32 amount, left, right;
    s16 minimum, current;
    volatile long double t;
    base = D(self + 0xdc);
    index = D(club);
    amount = (s32)D(club + 0x73);
    slot = base + index * 9u;
    W(slot + 5) = (u16)((u32)(amount / 10) + 0x1194u);
    result = cm_fn_00600a50(club, 1, 1, 0, 0);
    if (!result) {
        p = D(club + 0xcf);
        if (p) {
            q = D(p + 0x69);
            right = B(q + 0x1d);
            left = B(p + 0x57);
            amount = (s16)W(slot + 5);
            t = (long double)(left - right);
            t = t * (long double)F(0x0096a740);
            t = t + (long double)F(0x00967890);
            t = t * (long double)amount;
            /* Original __ftol call at VA 0x0083bfbe: truncate to int64, use AX. */
            value = (u32)(long long)t;
            W(slot + 5) = (u16)value;
        }
    }
    p = D(club + 0xbf);
    if (p) {
        q = D(p + 0x69);
        if (q) {
            right = B(q + 0x1d);
            left = B(p + 0x57);
            amount = (s16)W(slot + 5);
            t = (long double)(left - right);
            t = t * (long double)F(0x00968958);
            t = t + (long double)F(0x00967890);
            t = t * (long double)amount;
            /* Original __ftol call at VA 0x0083c004: truncate to int64, use AX. */
            value = (u32)(long long)t;
            W(slot + 5) = (u16)value;
            p = D(club + 0xbf);
            q = D(p + 0x69);
            amount = B(q + 0x20);
            t = (long double)(amount - 10);
            t = t * (long double)F(0x00967878);
            t = t + (long double)F(0x00967890);
            t = t * (long double)(s16)value;
            /* Original __ftol call at VA 0x0083c03e: truncate to int64, use AX. */
            value = (u32)(long long)t;
            W(slot + 5) = (u16)value;
        }
    }
    minimum = (s16)W(club + 0x80);
    current = (s16)W(slot + 5);
    if (minimum > current) {
        W(slot + 5) = (u16)minimum;
        return minimum;
    }
    if (current > 10000) {
        W(slot + 5) = 10000;
        return 10000;
    }
    W(slot + 5) = (u16)current;
    return current;
}
