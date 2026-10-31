/* Preferred-image VA 0x009368e0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_00936930(u32 self, int flag, volatile u8 *out);
u32 CM_CDECL cm_fn_009368e0(u32 self, u32 first, u32 second, volatile u8 *out)
{
    if (!self || CM_GLOBAL(volatile u32, self) != 0xa5a0f5a5u ||
        CM_GLOBAL(volatile u32, self + 0x44cu) != 0x0fa55af0u)
        return 0xffffffffu;
    CM_GLOBAL(volatile u32, self + 0x29eu) = first;
    CM_GLOBAL(volatile u32, self + 0x2a2u) = second;
    return cm_fn_00936930(self, 1, out);
}
