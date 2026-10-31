#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
u32 CM_THISCALL cm_fn_008e82f0(u32 self, u32 target)
{
    u32 list = D(self + 4);
    u32 item;
    if (!list) return 0;
    D(list + 0xa) = D(list + 2);
    item = cm_fn_0068d1b0(D(self + 4));
    while (item) {
        u32 value = D(item);
        if (value == D(target)) return item;
        item = cm_fn_0068d1b0(D(self + 4));
    }
    return 0;
}

