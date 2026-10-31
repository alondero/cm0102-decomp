#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);

u8 CM_THISCALL cm_fn_00419070(u32 self, u32 key)
{
    u32 list, node;
    s32 index = 0;
    if (key == 0) return 0;
    if (*(volatile s32 *)key >= CM_GLOBAL(volatile s32, 0x00ae2364u)) return 0;
    list = *(volatile u32 *)(self + 8u);
    if (list == 0) return 0;
    *(volatile u32 *)(list + 0xau) = *(volatile u32 *)(list + 2u);
    list = *(volatile u32 *)(self + 8u);
    if (*(volatile s16 *)list <= 0) return 0;
    do {
        node = cm_fn_0068d1b0(list);
        if (node == 0) return 0;
        if (*(volatile u32 *)node == *(volatile u32 *)key) return 1;
        list = *(volatile u32 *)(self + 8u);
        ++index;
    } while (index < *(volatile s16 *)list);
    return 0;
}
