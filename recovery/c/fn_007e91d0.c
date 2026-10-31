#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32, u32);
u32 CM_THISCALL cm_fn_007e91d0(u32 self, u32 target)
{
    u32 list = D(self + 7);
    u32 item;
    if (!list) return 0;
    D(list + 0xa) = D(list + 2);
    item = cm_fn_0068d1b0(D(self + 7));
    while (item) {
        if (D(item + 0x1c) == target)
            cm_fn_0068d1f0(D(self + 7), item);
        item = cm_fn_0068d1b0(D(self + 7));
    }
    return 0;
}

