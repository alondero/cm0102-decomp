#include "cmre.h"

u32 CM_THISCALL cm_fn_0085f7d0(u32 self, u32 selector, u32 value)
{
    u32 index = (u32)(s32)(s8)selector;
    *(volatile u32 *)(self + index * 4u + 0x10u) = value;
    return index;
}
