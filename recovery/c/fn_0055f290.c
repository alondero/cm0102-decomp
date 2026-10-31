/* Preferred-image VA 0x0055f290, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
void CM_STDCALL cm_fn_0055f290(u32 index)
{
    u32 base = CM_GLOBAL(volatile u32, 0x00ae2918u);
    if (base)
        CM_GLOBAL(volatile u16, base + index * 237601u * 2u + 0x74040u) = 0;
}
