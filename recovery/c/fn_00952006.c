/* Preferred-image VA 0x00952006, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_00952006(u32 p)
{
    u32 low = CM_GLOBAL(volatile u32, p);
    u32 mid = CM_GLOBAL(volatile u32, p + 4u);
    u32 high;
    CM_GLOBAL(volatile u32, p) = low << 1;
    high = CM_GLOBAL(volatile u32, p + 8u);
    CM_GLOBAL(volatile u32, p + 4u) = (mid << 1) | (low >> 31);
    CM_GLOBAL(volatile u32, p + 8u) = (high << 1) | (mid >> 31);
    return p;
}
