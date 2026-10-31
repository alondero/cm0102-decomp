/* Preferred-image VA 0x0094a51d, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_0094a51d(u32 base, u32 descriptor)
{
    u32 result = CM_GLOBAL(volatile u32, descriptor);
    u32 offset = CM_GLOBAL(volatile u32, descriptor + 4u);
    result += base;
    if ((s32)offset >= 0) {
        u32 table = CM_GLOBAL(volatile u32, base + offset);
        u32 displacement = CM_GLOBAL(volatile u32, descriptor + 8u);
        u32 adjustment = CM_GLOBAL(volatile u32, table + displacement);
        result += adjustment + offset;
    }
    return result;
}
