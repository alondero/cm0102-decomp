#include "cmre.h"

u32 CM_THISCALL cm_fn_004d0bd0(u32 self, u32 entry)
{
    u32 index = *(volatile u32 *)entry;
    u32 table = CM_GLOBAL(volatile u32, 0x00ae2d0c);
    u32 value = *(volatile u32 *)(table + index * 79u + 4u);
    u32 base;
    if (value == 0xffffffffu)
        return value;
    base = *(volatile u32 *)self;
    *(volatile u32 *)(base + value * 80u) = 0xffffffffu;
    index = *(volatile u32 *)entry;
    table = CM_GLOBAL(volatile u32, 0x00ae2d0c);
    *(volatile u32 *)(table + index * 79u + 4u) = 0xffffffffu;
    return table;
}
