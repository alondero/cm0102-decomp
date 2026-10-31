#include "cmre.h"

u16 CM_THISCALL cm_fn_008033c0(u32 self)
{
    u16 sum = 0;
    u32 cursor = self + 0x380au;
    u32 count = 16;

    do {
        sum = (u16)(sum + *(volatile u16 *)cursor);
        cursor += 2u;
        --count;
    } while (count != 0);
    return sum;
}
