#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);

u32 CM_THISCALL cm_fn_0055cb60(u32 self, u32 index, u32 value)
{
    u32 base = *(volatile u32 *)(self + 0x14);
    u32 list = self + 0x1c;
    u32 current = *(volatile u32 *)(list + 2);
    u32 row = base + index * 29;
    *(volatile u32 *)(list + 10) = current;
    current = cm_fn_0068d1b0(list);
    if (!current)
        return 0;
    do {
        if (*(volatile s8 *)(row + 0x1c) <= 0)
            return 0;
        if (*(volatile u32 *)current == index && *(volatile u8 *)(current + 4) == (u8)value)
            return current;
        current = cm_fn_0068d1b0(list);
    } while (current);
    return 0;
}
