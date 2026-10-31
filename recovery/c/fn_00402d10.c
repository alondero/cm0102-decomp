#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00403760(u32 self, volatile u32 first_out, volatile u32 second_out);
extern u32 CM_THISCALL cm_fn_00403a70(u32 self, u32 x, u32 y);
extern void CM_THISCALL cm_fn_00402e70(u32 self, u32 option);
extern u32 CM_CDECL cm_fn_005e2eb0(volatile u32 first, volatile u32 second, volatile u32 third, volatile u32 fourth);

u32 CM_THISCALL cm_fn_00402d10(volatile u8 *self)
{
    volatile u32 output[2];
    u32 x, y, current, a, b, c, d;
    cm_fn_00403760((u32)self, (u32)&output[0], (u32)&output[1]);
    x = output[0];
    current = *(volatile u32 *)self;
    y = output[1];
    if (current != x || *(volatile u32 *)(self + 4) != y)
        cm_fn_00403a70((u32)self, x, y);
    *(volatile u32 *)(self + 0xbe9) = 1;
    cm_fn_00402e70((u32)self, 0);
    d = *(volatile u32 *)(self + 0xc);
    c = *(volatile u32 *)(self + 8);
    b = *(volatile u32 *)(self + 4);
    a = *(volatile u32 *)self;
    return cm_fn_005e2eb0(a, b, c, d);
}
