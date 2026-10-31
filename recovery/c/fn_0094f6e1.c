/* Preferred-image VA 0x0094f6e1, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_0094f6e1(u32 words, s32 bit)
{
    s32 index = bit / 32;
    u32 shift = (31u - (u32)(bit % 32)) & 31u;
    u32 mask = ~(0xffffffffu << shift);
    if (CM_GLOBAL(volatile u32, words + (u32)index * 4u) & mask)
        return 0;
    ++index;
    while (index < 3) {
        if (CM_GLOBAL(volatile u32, words + (u32)index * 4u))
            return 0;
        ++index;
    }
    return 1;
}
