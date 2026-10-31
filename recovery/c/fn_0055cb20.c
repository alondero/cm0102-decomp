#include "cmre.h"

u32 CM_THISCALL cm_fn_0055cb20(u32 self, u32 first, u32 second, u32 third, volatile u32 fourth)
{
    u32 index = first * 29u + (u32)(s32)(s8)second * 5u;
    u32 base = *(volatile u32 *)(self + 0x14u);
    u32 value;
    *(volatile u8 *)(base + index) = (u8)third;
    base = *(volatile u32 *)(self + 0x14u);
    value = fourth;
    *(volatile u32 *)(base + index + 1u) = value;
    return index;
}
