/* Preferred-image VA 0x0094b258, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

u32 CM_CDECL cm_fn_0094b258(u32 start, u32 end)
{
    u32 cursor;
    if (start >= end)
        return 0;
    if (!CM_GLOBAL(volatile u32, 0x00de441c))
        return end - 1;
    cursor = end - 1;
    if (CM_GLOBAL(volatile u8, 0x00de4521 + CM_GLOBAL(volatile u8, cursor)) & 4)
        return cursor - 1;
    --cursor;
    while (start <= cursor &&
           (CM_GLOBAL(volatile u8, 0x00de4521 + CM_GLOBAL(volatile u8, cursor)) & 4))
        --cursor;
    return end - ((end - cursor) & 1) - 1;
}
