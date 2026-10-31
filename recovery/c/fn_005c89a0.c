#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004a7f20(u32 self, u32 first, u32 unused);

u32 CM_THISCALL cm_fn_005c89a0(u32 self, u32 index, u32 key)
{
    u32 row, first, second;
    if (!(self + 0xb1))
        return 0;
    row = self + 0xb1 + (u32)(s32)(s8)index * 105;
    if (key) {
        second = *(volatile u32 *)(key + 4);
        first = *(volatile u32 *)key;
        if (!cm_fn_004a7f20(row + 4, first, second))
            return 0;
    }
    if (!*(volatile u32 *)row)
        return 0;
    return row;
}
