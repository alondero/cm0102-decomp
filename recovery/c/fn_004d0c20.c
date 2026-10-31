#include "cmre.h"

u32 CM_THISCALL cm_fn_004d0c20(u32 self, u32 arg)
{
    u32 index, count, table, value, base, address;
    if (arg == 0)
        return 0;
    index = *(volatile u32 *)arg;
    count = CM_GLOBAL(volatile u32, 0x00ae2364);
    if ((s32)index >= (s32)count)
        return 0;
    table = CM_GLOBAL(volatile u32, 0x00ae2d0c);
    value = *(volatile u32 *)(table + index * 79u);
    if ((s32)value < 0)
        return 0;
    base = *(volatile u32 *)self;
    address = base + value * 80u;
    if (address == 0)
        return 0;
    return *(volatile u32 *)(address + 12u);
}
