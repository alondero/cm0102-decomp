/* Preferred-image VA 0x008676d0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_THISCALL cm_fn_008676d0(u32 self, u32 mask)
{
    u32 flags = CM_GLOBAL(volatile u32, self + 0x40u);
    u32 result = ~mask;
    CM_GLOBAL(volatile u32, self + 0x40u) = flags & result;
    return result;
}
