#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);

u32 CM_THISCALL cm_fn_005567d0(u32 self, u32 key)
{
    u32 list = self + 0x1c;
    u32 index = *(volatile u32 *)key;
    u32 base = *(volatile u32 *)(self + 0x14);
    u32 p = *(volatile u32 *)(list + 2);
    u32 row = base + index * 29;
    *(volatile u32 *)(list + 10) = p;
    p = cm_fn_0068d1b0(list);
    while (p) {
        u32 value;
        if (*(volatile s8 *)(row + 0x1c) <= 0)
            return p;
        value = *(volatile u32 *)p;
        if (value == *(volatile u32 *)key) {
            cm_fn_0068d1f0((u8 *)list, (const u8 *)p);
            *(volatile u8 *)(row + 0x1c) = (u8)(*(volatile u8 *)(row + 0x1c) - 1);
        }
        p = cm_fn_0068d1b0(list);
    }
    return 0;
}
