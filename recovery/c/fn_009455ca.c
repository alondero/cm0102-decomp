/* Preferred-image VA 0x009455ca, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_009455ca(void)
{
    CM_GLOBAL(volatile u32, 0x00ad8da4u) = 0x0094a76bu;
    CM_GLOBAL(volatile u32, 0x00ad8da0u) = 0x0094aad6u;
    CM_GLOBAL(volatile u32, 0x00ad8da8u) = 0x0094a7d1u;
    CM_GLOBAL(volatile u32, 0x00ad8dacu) = 0x0094a711u;
    CM_GLOBAL(volatile u32, 0x00ad8db0u) = 0x0094a7b9u;
    CM_GLOBAL(volatile u32, 0x00ad8db4u) = 0x0094aad6u;
    return 0x0094aad6u;
}
