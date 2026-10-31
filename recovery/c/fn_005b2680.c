#include "cmre.h"

u32 CM_THISCALL cm_fn_005b2680(u32 self)
{
    u16 a = *(volatile u16 *)self;
    u16 b;
    u32 i;
    for (i = 0; i < 5; ++i) {
        b = *(volatile u16 *)(self + (i + 1) * 2);
        *(volatile u16 *)(self + i * 2) = (u16)((a << 8) | (a >> 8));
        a = b;
    }
    *(volatile u16 *)(self + 10) = (u16)((a << 8) | (a >> 8));
    return 1;
}
