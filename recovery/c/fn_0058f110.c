#include "cmre.h"
/* Preferred-image VA 0x0058f110, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
u32 CM_STDCALL cm_fn_0058f110(u32 unused, u32 destination)
{
    (void)unused;
    *(volatile u32 *)destination = 0xffffffffu;
    return destination;
}
