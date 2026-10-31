#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);

void CM_THISCALL cm_fn_0056a090(u32 self, u32 key)
{
    u32 array, list, node;
    s32 index = 0;
    if (key == 0) return;
    array = *(volatile u32 *)(self + 4u);
    if (array == 0) return;
    array += *(volatile u32 *)key;
    if (*(volatile u8 *)array == 0) return;
    *(volatile u8 *)array = 0;
    list = *(volatile u32 *)self;
    if (list == 0) return;
    *(volatile u32 *)(list + 0xau) = *(volatile u32 *)(list + 2u);
    list = *(volatile u32 *)self;
    if (*(volatile s16 *)list <= 0) return;
    do {
        node = cm_fn_0068d1b0(list);
        if (node == 0) return;
        if (*(volatile u32 *)node == *(volatile u32 *)key) {
            list = *(volatile u32 *)self;
            cm_fn_0068d1f0((u8 *)list, (const u8 *)node);
            return;
        }
        list = *(volatile u32 *)self;
        ++index;
    } while (index < *(volatile s16 *)list);
}
