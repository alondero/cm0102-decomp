/* Preferred-image VA 0x00948687, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_00948687(void)
{
    u32 index;
    for (index = 0; index < 64u; ++index)
        CM_GLOBAL(volatile u32, 0x00de4520u + 4u * index) = 0;
    CM_GLOBAL(volatile u8, 0x00de4620u) = 0;
    CM_GLOBAL(volatile u32, 0x00de4408u) = 0;
    CM_GLOBAL(volatile u32, 0x00de441cu) = 0;
    CM_GLOBAL(volatile u32, 0x00de4624u) = 0;
    CM_GLOBAL(volatile u32, 0x00de4410u) = 0;
    CM_GLOBAL(volatile u32, 0x00de4414u) = 0;
    CM_GLOBAL(volatile u32, 0x00de4418u) = 0;
    return 0;
}
