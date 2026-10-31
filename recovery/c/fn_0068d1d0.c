#include "cmre.h"

u32 CM_THISCALL cm_fn_0068d1d0(u32 self)
{
    u32 node = *(volatile u32 *)(self + 10u);
    if (node != 0) {
        u32 value = *(volatile u32 *)node;
        u32 next = *(volatile u32 *)(node + 10u);
        *(volatile u32 *)(self + 10u) = next;
        return value;
    }
    return 0;
}
