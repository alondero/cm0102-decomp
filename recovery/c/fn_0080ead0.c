#include "cmre.h"

u32 CM_THISCALL cm_fn_0080ead0(u32 self, u32 selector)
{
    u8 chosen = (u8)selector;
    u32 index;
    if (chosen == 0xffu)
        chosen = *(volatile u8 *)self;
    index = (u32)(s32)(s8)chosen;
    return *(volatile u32 *)(self + index * 688u + 0x191u);
}
