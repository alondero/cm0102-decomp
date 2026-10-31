#include "cmre.h"

u32 CM_THISCALL cm_fn_00567ae0(u32 self, volatile u32 first,
                                volatile u32 second, volatile u32 third,
                                volatile u32 fourth)
{
    u32 a = first;
    u32 c = third;
    u32 b, d;
    *(volatile u32 *)(self + 0x10u) = a;
    b = second;
    *(volatile u32 *)(self + 0x18u) = c;
    d = fourth;
    *(volatile u32 *)(self + 0x14u) = b;
    *(volatile u32 *)(self + 0x1cu) = d;
    return b;
}
