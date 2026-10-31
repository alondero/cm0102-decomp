#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);

u32 CM_THISCALL cm_fn_00795750(u32 self, u32 date, u32 key)
{
    u32 initial = *(volatile u32 *)date;
    u32 index = CM_GLOBAL(volatile u32, 0x00ae2364) - initial - 1;
    u32 list, p, value, i = 0;
    if ((s32)index < 0 || (s32)index >= 16)
        return 0;
    list = *(volatile u32 *)(self + index * 8);
    p = *(volatile u32 *)(list + 2);
    *(volatile u32 *)(list + 10) = p;
    list = *(volatile u32 *)(self + index * 8);
    if (*(volatile s16 *)list <= 0)
        return 0;
    do {
        p = cm_fn_0068d1b0(list);
        if (!p) return 0;
        value = *(volatile u32 *)p;
        if (value == *(volatile u32 *)key) return p;
        list = *(volatile u32 *)(self + index * 8);
        ++i;
    } while ((s32)i < *(volatile s16 *)list);
    return 0;
}
