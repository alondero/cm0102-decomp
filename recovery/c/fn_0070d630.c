/* Preferred-image VA 0x0070d630, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define B(a) CM_GLOBAL(volatile u8, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u8 CM_THISCALL cm_fn_006cd5f0(u32 self, u32 object);

u8 CM_THISCALL cm_fn_0070d630(u32 self)
{
    u8 side = B(self + 0x27u);
    s32 delta = (s8)B(self + 0x103u) - (side ? 11 : 0);
    s32 first = (s8)(u8)(delta < 0 ? -delta : delta);
    s32 other = (s8)B(self + 0x102u) - 4;
    s32 second = (s8)(u8)(other < 0 ? -other : other);
    u32 index = (u32)(first * 9 + second);
    long double value = CM_GLOBAL(volatile float, 0x00a15e44u + index * 4u);
    double threshold = CM_GLOBAL(volatile double, 0x00967890u);
    if (value > threshold) {
        u32 owner = D(self + 0x19eu);
        if (!cm_fn_006cd5f0(owner, self))
            return 0;
    }
    return 1;
}
