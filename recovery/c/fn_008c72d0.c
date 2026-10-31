#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);

u32 CM_THISCALL cm_fn_008c72d0(u32 self, u32 first, u32 second)
{
    u32 list = *(volatile u32 *)(self + 0x946);
    u32 p, a, b;
    if (!list) return 0;
    p = *(volatile u32 *)(list + 2);
    *(volatile u32 *)(list + 10) = p;
    list = *(volatile u32 *)(self + 0x946);
    p = cm_fn_0068d1b0(list);
    while (p) {
        a = *(volatile u32 *)first;
        b = *(volatile u32 *)p;
        if (a == b) {
            a = *(volatile u32 *)second;
            b = *(volatile u32 *)(p + 4);
            if (a == b) return p;
        }
        list = *(volatile u32 *)(self + 0x946);
        p = cm_fn_0068d1b0(list);
    }
    return 0;
}
