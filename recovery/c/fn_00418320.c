#include "cmre.h"

u32 CM_THISCALL cm_fn_00418320(u32 self)
{
    u32 address = self + 8;
    u32 count = 8;
    *(volatile u16 *)(self + 4) = 0;
    do {
        *(volatile u32 *)address = 0xffffffffu;
        address += 4;
        --count;
    } while (count != 0);
    return 0xffffffffu;
}
