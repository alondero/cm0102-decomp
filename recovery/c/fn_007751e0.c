#include "cmre.h"

u32 CM_THISCALL cm_fn_007751e0(u32 self, u32 entry)
{
    u32 index, base, value;
    if (entry == 0)
        return 0;
    index = *(volatile u32 *)entry;
    base = *(volatile u32 *)(self + 4u);
    value = *(volatile u32 *)(base + index * 110u + 4u);
    if ((s32)value <= -1)
        return 0;
    base = CM_GLOBAL(volatile u32, 0x00ae23d0);
    return base + value * 107u;
}
