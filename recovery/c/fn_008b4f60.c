#include "cmre.h"
/* Pinned 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   VAs 008b4f64-008b4f66 read ECX as receiver; 008b4ff9 returns without stack cleanup.
   The x87 products are converted by __ftol at 008b4f78, 008b4f92,
   008b4fae, 008b4fc7, and 008b4fe0, represented as long long casts.
   The receiver-byte read is sequenced before the coefficient read in a separate
   full expression, matching each listing pair (008b4f66/008b4f72,
   008b4f7d/008b4f8c, 008b4f97/008b4fa8, 008b4fb5/008b4fc1, 008b4fcc/008b4fda). */
int CM_THISCALL cm_fn_008b4f60(u8 *p)
{
    long double t;
    u32 a, b, c, d, e, result;
    s16 low;
    s8 rb;
    rb = *(volatile s8 *)(p + 4);
    t = (long double)rb * CM_GLOBAL(volatile double, 0x96f878);
    a = (u32)(long long)t;
    rb = *(volatile s8 *)(p + 8);
    t = (long double)rb * CM_GLOBAL(volatile double, 0x968968);
    b = (u32)(long long)t;
    rb = *(volatile s8 *)(p + 7);
    t = (long double)rb * CM_GLOBAL(volatile double, 0x969540);
    c = (u32)(long long)t;
    rb = *(volatile s8 *)(p + 6);
    t = (long double)rb * CM_GLOBAL(volatile double, 0x969540);
    d = (u32)(long long)t;
    rb = *(volatile s8 *)(p + 5);
    t = (long double)rb * CM_GLOBAL(volatile double, 0x969540);
    e = (u32)(long long)t;
    result = (u16)b - (u16)a - c - d - e;
    low = (s16)result;
    return low >= 100 ? 100 : low;
}
