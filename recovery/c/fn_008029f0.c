/* Preferred-image VA 0x008029f0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_0084cc80(u32 self, u32 argument);

u32 CM_THISCALL cm_fn_008029f0(u32 self, u32 argument)
{
    u32 index, receiver;
    if ((u8)argument == 0xffu) return 0;
    index = (u32)(s32)(s16)W(self + 0x382au);
    if (!W(self + index * 768u + 0x7d4u)) {
        receiver = D(0x00dd7eecu);
        if (!receiver) return 0;
        return cm_fn_0084cc80(receiver, argument);
    }
    return D(self + (index * 10u + (u32)(s32)(s8)argument) * 4u + 0x26250au);
}
