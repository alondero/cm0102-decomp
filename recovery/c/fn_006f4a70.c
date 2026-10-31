#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
u8 CM_THISCALL cm_fn_006f4a70(u32 self)
{
    u8 kind = B(self + 0x27);
    u32 object = D(self + 0x19e);
    return B(object + (kind ? 0xf5bdu : 0xf5f3u));
}

