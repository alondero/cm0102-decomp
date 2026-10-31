#include "cmre.h"

u32 CM_THISCALL cm_fn_0079d3b0(u32 self, u32 argument)
{
    u32 index, base;
    u8 value;
    if (argument == 0)
        return 6;
    index = *(volatile u32 *)argument;
    base = *(volatile u32 *)self;
    value = *(volatile u8 *)(base + index * 17u + 0x10);
    if (value == 0xff)
        value = 7;
    return (base & 0xffffff00u) | value;
}
