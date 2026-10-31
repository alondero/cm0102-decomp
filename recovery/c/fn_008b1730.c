#include "cmre.h"

u32 CM_THISCALL cm_fn_008b1730(u32 self, u32 source)
{
    s32 value = *(volatile s16 *)source;
    s32 quotient = value / 7;
    if (value % 7 == 0) {
        u16 count = *(volatile u16 *)(self + 0x688du);
        *(volatile u16 *)(self + 0x688du) = (u16)(count + 1u);
    }
    return (u32)quotient;
}
