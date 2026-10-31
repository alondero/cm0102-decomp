#include "cmre.h"

long double CM_THISCALL cm_fn_007bd960(u32 self)
{
    u8 cursor = *(volatile u8 *)(self + 0x1a0u);
    s8 count = *(volatile s8 *)(self + 0x1a1u);
    u16 sum = 0;
    s32 remaining = count;

    while (remaining > 0) {
        u32 address = self + (u32)((s32)(s8)cursor * 41) + 0x1bu;
        sum = (u16)(sum + *(volatile u8 *)address);
        cursor = (u8)(cursor + 1u);
        if (cursor == 10u)
            cursor = 0;
        --remaining;
    }
    if (count == 0)
        return (long double)CM_GLOBAL(volatile double, 0x009689a0u);
    return (long double)(s32)(s16)sum / (long double)(s32)count;
}
