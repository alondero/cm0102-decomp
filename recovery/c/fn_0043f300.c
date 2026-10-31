/* Preferred-image VA 0x0043f300, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_0043f300(u32 address, u32 count)
{
    u32 result = count - 1u;
    while (count) {
        u32 value = CM_GLOBAL(volatile u32, address);
        u32 swapped = (((value & 0x00ff0000u) | (value >> 16)) >> 8) |
                      (((value << 16) | (value & 0x0000ff00u)) << 8);
        result = value & 0x0000ff00u;
        CM_GLOBAL(volatile u32, address) = swapped;
        address += 4u;
        --count;
    }
    return result;
}
