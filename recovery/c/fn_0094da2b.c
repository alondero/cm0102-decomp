/* Preferred-image VA 0x0094da2b, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094c322(u32 value, u32 mask);
u32 CM_CDECL cm_fn_0094da2b(u32 value)
{
    u32 classification;
    if ((s32)CM_GLOBAL(volatile u32, 0x00ad91dcu) > 1)
        classification = cm_fn_0094c322(value, 4);
    else {
        u32 table = CM_GLOBAL(volatile u32, 0x00ad8fd0u);
        classification = CM_GLOBAL(volatile u8, table + value * 2u) & 4u;
    }
    if (!classification)
        value = (value & 0xffffffdfu) - 7u;
    return value;
}
