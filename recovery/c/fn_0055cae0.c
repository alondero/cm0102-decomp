#include "cmre.h"

u32 CM_THISCALL cm_fn_0055cae0(u32 self, u32 first, u32 second, u32 third)
{
    u32 base = *(volatile u32 *)(self + 0x14u);
    u32 offset = first * 29u + (u32)(s32)(s8)second * 5u;
    u32 address = base + offset;
    u8 tag = *(volatile u8 *)address;
    if (tag == (u8)third)
        return *(volatile u32 *)(address + 1u);
    return 0xffffffffu;
}
