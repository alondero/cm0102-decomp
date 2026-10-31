/* Preferred-image VA 0x00946c06, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094c797(volatile u8 *stream, unsigned long long amount, u32 mode);
u32 CM_CDECL cm_fn_00946c06(volatile u8 *stream, u32 offset)
{
    u32 high = CM_GLOBAL(volatile u32, offset + 4u);
    u32 low = CM_GLOBAL(volatile u32, offset);
    unsigned long long amount = ((unsigned long long)high << 32) | low;
    return cm_fn_0094c797(stream, amount, 0);
}
