/* Preferred-image VA 0x00549c90, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
void CM_CDECL cm_fn_00549c90(u32 input)
{
    u32 offset;
    if (!input)
        return;
    if ((s32)CM_GLOBAL(volatile u32, input) >=
        (s32)CM_GLOBAL(volatile u32, 0x00ae2364u))
        return;
    for (offset = 0; offset < 0x28u; offset += 0x14u) {
        u32 index = CM_GLOBAL(volatile u32, input);
        u32 base = CM_GLOBAL(volatile u32, 0x00ae2d0cu);
        CM_GLOBAL(volatile u32, base + index * 79u + offset + 0x27u) = 0xffffffffu;
        index = CM_GLOBAL(volatile u32, input);
        base = CM_GLOBAL(volatile u32, 0x00ae2d0cu);
        CM_GLOBAL(volatile u32, base + index * 79u + offset + 0x37u) = 0xffffffffu;
    }
}
