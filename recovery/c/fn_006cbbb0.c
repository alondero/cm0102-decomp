/* Preferred-image VA 0x006cbbb0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u8 CM_THISCALL cm_fn_006cbbb0(u32 self, u32 argument)
{
    u32 index = (u32)(s32)(s8)argument;
    u32 cursor = self + index * 8920u + 0x47afu;
    u32 i;
    u8 count = 0;
    for (i = 0; i < 20u; ++i) {
        if ((s8)CM_GLOBAL(volatile u8, cursor) >= 0)
            ++count;
        cursor += 0x1beu;
    }
    CM_GLOBAL(volatile u8, self + index + 4u) = count;
    return count;
}
