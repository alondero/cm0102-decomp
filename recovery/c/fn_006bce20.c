/* Preferred-image VA 0x006bce20, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u8 CM_FASTCALL cm_fn_006f4600(u32 self);

u32 CM_THISCALL cm_fn_006bce20(u32 self, u32 object, u32 first, u32 second)
{
    if (!object) return 0;
    if (B(object + 0x102u) == (u8)first && B(object + 0x103u) == (u8)second)
        return 0;
    if ((s8)B(object + 0x2bu) >= 2) return 0;
    if (object == D(self + 0xf582u)) return 0;
    if (object == D(self + 0xf57au)) return 0;
    if (!cm_fn_006f4600(object)) return 0;
    if (B(self + 0x8eb2u)) return 0;
    return 1;
}
