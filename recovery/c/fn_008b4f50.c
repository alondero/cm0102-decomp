#include "cmre.h"

u32 CM_THISCALL cm_fn_008b4f50(u32 self, u32 selector)
{
    u32 index = (u32)(s32)(s8)selector;
    u8 value = *(volatile u8 *)(self + index + 4u);
    return (index & 0xffffff00u) | value;
}
