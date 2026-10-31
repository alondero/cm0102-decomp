#include "cmre.h"

extern void CM_THISCALL cm_fn_005b0260(u8 *self);

void CM_THISCALL cm_fn_005afce0(u32 self)
{
    u32 index;
    for (index = 0; index < 16u; ++index) {
        u32 entry = *(volatile u32 *)(self + index * 4u);
        if (entry != 0) cm_fn_005b0260((u8 *)entry);
    }
}
