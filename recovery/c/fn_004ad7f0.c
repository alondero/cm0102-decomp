#include "cmre.h"
/* Preferred-image VA 004ad7f0, image base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0 / RET0. The sort, statistic and record-copy calls remain original.
   qsort has four stack arguments; its incidental EDX:EAX result is dead here. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define F(a) CM_GLOBAL(volatile float, (a))
extern void CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 width, u32 compare);
extern long double CM_THISCALL cm_fn_004a3250(u32 self, u32 stats, s8 selector);
extern u32 CM_CDECL cm_fn_007e4920(u32 source, u32 destination);

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_004ad7f0(u32 self)
{
    u32 count, base, stats, index;
    long double value;
    long long converted;
    if (D(self + 0x19u) == 0) return 0;
    if (B(self + 0x25u) != 0x11u) {
        if (B(self + 0x18u) != 0xffu) {
            B(self + 0x18u) = 0xffu;
            D(0x00adae10u) = self;
            count = D(self + 0x14u);
            base = D(self + 0xcu);
            cm_fn_00945333(base, count, 0x76u, 0x004ad1a0u);
        }
        B(self + 0x25u) = 0x11u;
        D(0x00adae10u) = self;
        count = D(self + 0x21u);
        base = D(self + 0x19u);
        cm_fn_00945333(base, count, 0x2du, 0x004ad250u);
    }
    stats = D(self + 0x19u);
    value = cm_fn_004a3250(self, stats, 0x11);
    F(self + 0x418u) = (float)value;
    value = cm_fn_004a3250(self, stats, 1);
    /* Original __ftol call at VA 004ad86c: chop ST0 to a full 64-bit result.
       Disable SSE3 so GCC saves/restores the control word around FISTP64. */
    converted = (long long)value;
    B(self + 0x434u) = (u8)converted;
    index = D(stats + 4u);
    base = D(0x00ae23bcu);
    cm_fn_007e4920(base + index * 0x6eu, self + 0x41cu);
    D(self + 0x435u) = D(stats + 8u);
    W(self + 0x439u) = 0;
    return 1;
}
