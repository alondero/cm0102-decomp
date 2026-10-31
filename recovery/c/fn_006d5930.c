#include "cmre.h"

u8 CM_THISCALL cm_fn_006d5930(u32 self)
{
    u8 value = *(volatile u8 *)(self + 0xf638u);
    if (value == 0xffu) return 0;
    return (s16)((s16)(s8)value * 11) < *(volatile s16 *)(self + 0x8ed0u);
}
