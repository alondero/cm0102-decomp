#include "cmre.h"

u32 CM_THISCALL cm_fn_005b57c0(u32 self, u32 index)
{
    u32 extended = (u32)(s32)(s8)index;
    u8 value = *(volatile u8 *)(self + extended + 0x5b4);
    return (extended & 0xffffff00u) | value;
}
