#include "cmre.h"

u32 CM_THISCALL cm_fn_0080eaa0(u32 self, u32 selector)
{
    u8 chosen = (u8)selector;
    u32 index, product;
    u8 value;
    if (chosen == 0xffu)
        chosen = *(volatile u8 *)self;
    index = (u32)(s32)(s8)chosen;
    product = index * 21u;
    value = *(volatile u8 *)(self + index * 688u + 11u);
    return (product & 0xffffff00u) | value;
}
