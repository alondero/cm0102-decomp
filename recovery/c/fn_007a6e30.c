#include "cmre.h"

extern void CM_THISCALL cm_fn_007a82b0(void *self, u32 key);

void CM_THISCALL cm_fn_007a6e30(u32 self, u32 first, u32 second, u32 third)
{
    u32 source = *(volatile u32 *)(self + 0x1cu);
    *(volatile u32 *)first = *(volatile u32 *)source;
    source = *(volatile u32 *)(self + 0x1cu);
    *(volatile u32 *)second = *(volatile u32 *)(source + 4u);
    source = *(volatile u32 *)(self + 0x1cu);
    *(volatile u32 *)third = *(volatile u32 *)(source + 8u);
    cm_fn_007a82b0((void *)self, *(volatile u32 *)first);
}
