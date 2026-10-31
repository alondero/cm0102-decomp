#include "cmre.h"

u32 CM_THISCALL cm_fn_005b5910(u32 self, u32 selector, u32 entry)
{
    u32 value = 0xffffffffu;
    u32 index;
    if (entry != 0)
        value = *(volatile u32 *)entry;
    index = (u32)(s32)(s8)selector;
    *(volatile u32 *)(self + index * 4u + 0x5bfu) = value;
    *(volatile u32 *)(self + 0x187fu) = 0;
    return 1;
}
