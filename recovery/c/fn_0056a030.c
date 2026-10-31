#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);

u32 CM_THISCALL cm_fn_0056a030(u32 self, u32 key)
{
    u32 list, p, current, i = 0;
    if (!key)
        return 0xffffffff;
    p = *(volatile u32 *)(self + 4);
    if (!p)
        return 0xffffffff;
    current = *(volatile u32 *)key;
    if (!*(volatile u8 *)(p + current))
        return 0xffffffff;
    list = *(volatile u32 *)self;
    if (!list)
        return 0xffffffff;
    p = *(volatile u32 *)(list + 2);
    *(volatile u32 *)(list + 10) = p;
    list = *(volatile u32 *)self;
    if (*(volatile s16 *)list <= 0)
        return 0xffffffff;
    do {
        current = cm_fn_0068d1b0(list);
        if (!current)
            return 0xffffffff;
        p = *(volatile u32 *)current;
        if (p == *(volatile u32 *)key)
            return *(volatile u32 *)(current + 4);
        list = *(volatile u32 *)self;
        ++i;
    } while ((s32)i < *(volatile s16 *)list);
    return 0xffffffff;
}
