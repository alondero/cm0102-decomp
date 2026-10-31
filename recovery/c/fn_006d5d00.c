/* Preferred-image VA 0x006d5d00, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_006d6f70(u32 self, u32 a, u32 b, u32 c,
                                      u32 d, u32 e, u32 f, u32 g);

u32 CM_THISCALL cm_fn_006d5d00(u32 self, u32 a, u32 b, u32 c,
                               u32 d, u32 e, u32 f, u32 g)
{
    u16 pos = W(self + 8);
    u16 count;
    if (!pos) pos = 11;
    else pos = (u16)(pos - 1u);
    W(self + 8) = pos;
    count = W(self + 6);
    if ((s16)count > 0) {
        u32 left = self + (u32)((s32)(s16)count * 14) + 0x712u;
        u32 right = self + (u32)((s32)(s16)W(self + 8) * 14) + 0x30u;
        u16 value = W(left);
        if (value == W(right)) W(self + 6) = (u16)(count - 1u);
    }
    return cm_fn_006d6f70(self, a, b, c, d, e, f, g);
}
