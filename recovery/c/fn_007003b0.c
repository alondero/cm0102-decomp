#include "cmre.h"

void CM_THISCALL cm_fn_007003b0(u32 self, u8 value)
{
    u32 address;
    u8 current;
    *(volatile u8 *)(self + 0x8eabu) = value;
    if (value == 0xffu) return;
    address = self + 0x8d46u + (u32)(s32)(s8)value;
    current = *(volatile u8 *)address;
    *(volatile u8 *)address = (u8)(current + 1u);
    address = self + 0x8d46u + (value == 0);
    current = *(volatile u8 *)address;
    if ((s8)current > 0) *(volatile u8 *)address = (u8)(current - 10u);
}
