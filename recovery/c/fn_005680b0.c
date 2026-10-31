#include "cmre.h"

u32 CM_THISCALL cm_fn_005680b0(u32 self, u32 first, u32 second)
{
    u32 index = (u32)(s32)*(volatile s16 *)(self + 0x382a) * 768u;
    *(volatile u16 *)(self + index + 0x7f6) = (u16)first;
    index = (u32)(s32)*(volatile s16 *)(self + 0x382a) * 768u;
    *(volatile u32 *)(self + index + 0x7f8) = second;
    return index;
}
