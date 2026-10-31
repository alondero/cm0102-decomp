/* Preferred-image VA 0x00714ca0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u8 CM_THISCALL cm_fn_00714ca0(u32 self, u32 target)
{
    s8 count = (s8)CM_GLOBAL(volatile u8, self + 0x101u);
    u8 index = 0;
    if (count <= 0)
        return 0;
    do {
        u32 address = self + (u32)(s32)(s8)index * 4u + 0x1aeu;
        if (CM_GLOBAL(volatile u32, address) == target) {
            if (index != 0xffu) {
                u32 last = CM_GLOBAL(volatile u32, self + (u32)(s32)count * 4u + 0x1aau);
                CM_GLOBAL(volatile u32, address) = last;
                index = CM_GLOBAL(volatile u8, self + 0x101u) - 1u;
                CM_GLOBAL(volatile u8, self + 0x101u) = index;
            }
            return index;
        }
        ++index;
    } while ((s8)index < count);
    return index;
}
