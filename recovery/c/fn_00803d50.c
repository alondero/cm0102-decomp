#include "cmre.h"

u16 CM_THISCALL cm_fn_00803d50(u32 self)
{
    u32 cursor = self + 0x7e6u;
    u32 outer = 16;
    u16 sum = 0;

    do {
        u32 inner = 16;
        do {
            sum = (u16)(sum + *(volatile u16 *)cursor);
            cursor += 0x30u;
            --inner;
        } while (inner != 0);
        --outer;
    } while (outer != 0);
    return sum;
}
