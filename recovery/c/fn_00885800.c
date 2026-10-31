#include "cmre.h"

u32 CM_THISCALL cm_fn_00885800(u32 self, u32 entry)
{
    u32 index, base, offset;
    u16 value;
    if (entry == 0)
        return 0;
    index = *(volatile u32 *)entry;
    base = *(volatile u32 *)self;
    offset = index * 73u;
    value = *(volatile u16 *)(base + offset + 6u);
    return (offset & 0xffff0000u) | value;
}
