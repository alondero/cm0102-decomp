/* Preferred-image VA 0x007bd830, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_THISCALL cm_fn_007bd830(u32 self, u32 argument)
{
    s32 position;
    s8 index = (s8)argument;
    s8 count = (s8)CM_GLOBAL(volatile u8, self + 0x1a1u);
    if (index >= count)
        return 0;
    position = (s8)CM_GLOBAL(volatile u8, self + 0x1a0u);
    position += index;
    if (position >= 10)
        position -= 10;
    return self + (u32)position * 41u + 6u;
}
