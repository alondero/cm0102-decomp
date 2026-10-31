/* Preferred-image VA 0x00935080, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (ret 0, caller pops), two dword stack arguments a1 (+4) and a2 (+8).
   Two dwords x = *a1 and y = *a2 are read, and the result depends on which is zero:
   - x == 0: *a2 = (y < 0) ? -0x4000 : 0x4000; *a1 unchanged; EAX = a2.
   - x != 0, y == 0: *a1 = (x < 0) ? -0x4000 : 0x4000; *a2 unchanged; EAX = x.
   - otherwise: with ax = |x|, ay = |y|, d = (ax <= ay) ? (ax >> 1) + ay : ax + (ay >> 1),
     q1 = (x << 14) / d, q2 = (y << 14) / d, s = q1*q1 + q2*q2, then a fixed-point
     loop c = (s / c + c + 1) >> 1 from c = 0x4000 until c stops changing.
     *a1 = (q1 << 14) / c; *a2 = (q2 << 14) / c; EAX = the new *a2.
   Division is 32-bit signed (cdq/idiv), which matches the listing. Object accesses
   are volatile so -O2 keeps the listing's order at a fault. The routine has two
   direct callers; their use of its result is not inferred here. Field meanings
   remain unknown. */
#include "cmre.h"

u32 CM_CDECL cm_fn_00935080(s32 *a1, s32 *a2)
{
    s32 x;
    s32 y;
    s32 ax;
    s32 ay;
    s32 d;
    s32 q1;
    s32 q2;
    s32 s;
    s32 c;
    s32 prev;
    s32 t;
    s32 r1;
    s32 r2;

    x = *(volatile s32 *)a1;
    if (x == 0) {
        y = *(volatile s32 *)a2;
        *(volatile s32 *)a2 = (y < 0) ? -0x4000 : 0x4000;
        return (u32)a2;
    }

    y = *(volatile s32 *)a2;
    if (y == 0) {
        *(volatile s32 *)a1 = (x < 0) ? -0x4000 : 0x4000;
        return (u32)x;
    }

    ax = (x < 0) ? (s32)(0u - (u32)x) : x;
    ay = (y < 0) ? (s32)(0u - (u32)y) : y;
    if (ax <= ay) {
        d = (s32)((u32)(ax >> 1) + (u32)ay);
    } else {
        d = (s32)((u32)ax + (u32)(ay >> 1));
    }

    q1 = (s32)((u32)x << 14) / d;
    q2 = (s32)((u32)y << 14) / d;
    s = (s32)((u32)q1 * (u32)q1 + (u32)q2 * (u32)q2);

    c = 0x4000;
    do {
        prev = c;
        t = s / c;
        c = (s32)((u32)t + (u32)c + 1u) >> 1;
    } while (prev != c);

    r1 = (s32)((u32)q1 << 14) / c;
    *(volatile s32 *)a1 = r1;
    r2 = (s32)((u32)q2 << 14) / c;
    *(volatile s32 *)a2 = r2;
    return (u32)r2;
}
