#include "cmre.h"

u32 CM_THISCALL cm_fn_0085f7e0(u32 self, u32 selector)
{
    u32 index = (u32)(s32)(s8)selector;
    *(volatile u8 *)(self + index + 0x1cu) = 0;
    return index;
}
