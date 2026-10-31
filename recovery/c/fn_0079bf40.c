/* Preferred-image VA 0x0079bf40, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_THISCALL cm_fn_0079bf40(u32 self)
{
    u32 index = 0;
    s32 count = (s32)CM_GLOBAL(volatile u32, self + 4u);
    if (count > 0) {
        do {
            u32 base = CM_GLOBAL(volatile u32, self + 8u);
            ++index;
            CM_GLOBAL(volatile u8, base + index - 1u) = 0;
            count = (s32)CM_GLOBAL(volatile u32, self + 4u);
        } while ((s32)index < count);
    }
    return index;
}
