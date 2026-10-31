#include "cmre.h"

u32 CM_THISCALL cm_fn_006f5b20(u32 self, u32 arg)
{
    s8 a = *(volatile s8 *)(self + 0x102);
    s8 b;
    if (a < 2 || a > 6)
        return 0;
    b = *(volatile s8 *)(self + 0x103);
    if ((u8)arg == 1)
        return b >= 10 && b <= 11;
    return b >= 0 && b <= 1;
}
