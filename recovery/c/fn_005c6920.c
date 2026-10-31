#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);

u32 CM_THISCALL cm_fn_005c6920(u32 self)
{
    u32 list = D(self + 0x34cc);
    u32 current, target, item;
    if (!list) return 0;
    current = (u32)(s32)(s16)CM_GLOBAL(volatile u16, 0x00b77c7a);
    current = D(0x00b74c26u + current * 0x300u);
    if (!current) return 0;
    target = D(current + 0x39);
    if (!target) return 0;
    D(list + 0xa) = D(list + 2);
    item = cm_fn_0068d1b0(D(self + 0x34cc));
    while (item) {
        if (D(item) == target && D(item + 0x103)) return item;
        item = cm_fn_0068d1b0(D(self + 0x34cc));
    }
    return 0;
}

