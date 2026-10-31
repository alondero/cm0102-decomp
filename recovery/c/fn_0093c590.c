/* Preferred-image VA 0x0093c590, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_00944250(u32 self, u32 size, u32 index);
u32 CM_CDECL cm_fn_0093c590(u32 self, u32 count)
{
    u32 allocation = cm_fn_00944250(CM_GLOBAL(volatile u32, self), count * 13u + 52u, 1);
    u32 cursor;
    CM_GLOBAL(volatile u32, self + 0x4cu) = allocation;
    cursor = allocation + count * 4u + 0x10u;
    CM_GLOBAL(volatile u32, self + 0x50u) = cursor;
    cursor += count * 4u + 0x10u;
    CM_GLOBAL(volatile u32, self + 6u) = count;
    CM_GLOBAL(volatile u32, self + 0x40u) = cursor;
    cursor += count * 2u + 8u;
    CM_GLOBAL(volatile u32, self + 0x44u) = cursor;
    cursor += count * 2u + 8u;
    CM_GLOBAL(volatile u32, self + 0x48u) = cursor;
    return cursor;
}
