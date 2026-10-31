#include "cmre.h"

u32 CM_THISCALL cm_fn_0085f7a0(u32 self, u32 selector)
{
    u32 index = (u32)(s32)(s8)selector;
    if ((u8)selector == 1u) {
        *(volatile u32 *)(self + 0x1cu) = 0;
        *(volatile u8 *)(self + 0x20u) = 0;
    } else {
        *(volatile u8 *)(self + 0x1du) = 0;
    }
    *(volatile u8 *)(self + index + 0x1cu) = 1;
    return index;
}
