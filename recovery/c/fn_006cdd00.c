#include "cmre.h"

u32 CM_THISCALL cm_fn_006cdd00(u32 self, u32 object)
{
    s32 index = *(volatile s8 *)(object + 0x27);
    s32 count;
    u32 counter_address;
    u8 value;
    *(volatile u8 *)(object + 0x102) = 8;
    *(volatile u8 *)(object + 0x103) = 6;
    count = *(volatile s8 *)(self + (u32)index + 0x4592);
    *(volatile u32 *)(self + (u32)(count + index * 11) * 4u + 0x453a) = object;
    index = *(volatile s8 *)(object + 0x27);
    counter_address = self + (u32)index + 0x4592;
    value = *(volatile u8 *)counter_address;
    *(volatile u8 *)(object + 0x28) = value;
    value = *(volatile u8 *)counter_address;
    ++value;
    *(volatile u8 *)counter_address = value;
    return (object & 0xffffff00u) | value;
}
