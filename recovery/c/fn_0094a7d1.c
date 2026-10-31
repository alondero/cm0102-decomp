/* Preferred-image VA 0x0094a7d1, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094fa73(u32 destination, u32 input);
extern u32 CM_CDECL cm_fn_0094faa0(u32 destination, u32 input);
u32 CM_CDECL cm_fn_0094a7d1(u32 flag, u32 destination, u32 input)
{
    volatile u32 scratch[2];
    volatile u32 local = input;
    if (flag) {
        cm_fn_0094fa73((u32)scratch, input);
        CM_GLOBAL(volatile u32, destination) = scratch[0];
        CM_GLOBAL(volatile u32, destination + 4u) = scratch[1];
    } else {
        cm_fn_0094faa0((u32)&local, input);
        CM_GLOBAL(volatile u32, destination) = local;
    }
    return destination;
}
