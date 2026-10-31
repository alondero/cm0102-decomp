#include "cmre.h"

unsigned long long CM_THISCALL cm_fn_005b5270(u32 self, u32 index)
{
    u32 offset = (u32)(s32)(s8)index * 8u;
    u32 low = *(volatile u32 *)(self + offset + 0x188b);
    u32 high = *(volatile u32 *)(self + offset + 0x188f);
    return ((unsigned long long)high << 32) | low;
}
