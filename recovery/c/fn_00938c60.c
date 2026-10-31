/* Preferred-image VA 0x00938c60, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_00938c60(u32 self)
{
    u32 head = CM_GLOBAL(volatile u32, self + 0x20u) - 4u;
    u32 count = CM_GLOBAL(volatile u32, head);
    u32 cursor = head - count * 4u;
    u32 saved = CM_GLOBAL(volatile u32, cursor);
    if (count) {
        do {
            u32 value = CM_GLOBAL(volatile u32, cursor + 4u);
            CM_GLOBAL(volatile u32, cursor) = value;
            cursor += 4u;
            --count;
        } while (count);
        head -= 4u;
    }
    CM_GLOBAL(volatile u32, head) = saved;
    CM_GLOBAL(volatile u32, self + 0x20u) = head + 4u;
    return cursor;
}
