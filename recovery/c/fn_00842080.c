/* Preferred-image VA 0x00842080, CM 01/02 3.9.68; image base 0x00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00946464(u32 key, u32 base, u32 count,
                                  u32 size, u32 comparator);

u32 CM_THISCALL cm_fn_00842080(u32 self, u32 first, u32 second)
{
    u32 base = *(volatile u32 *)self;
    volatile u32 key[2];
    u32 count, found;

    if (base == 0)
        return 0;
    key[0] = *(volatile u32 *)first;
    if (second != 0)
        key[1] = *(volatile u32 *)second;
    else
        key[1] = 0xffffffffu;
    count = *(volatile u32 *)(self + 0x10u);
    found = cm_fn_00946464((u32)key, base, count, 8u, 0x00757ea0u);
    /* SETNE AL preserves the remaining 24 bits of the search result. */
    return (found & 0xffffff00u) | (u32)(found != 0);
}
