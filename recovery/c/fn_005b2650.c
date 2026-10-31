#include "cmre.h"

u32 CM_THISCALL cm_fn_005b2650(u32 self)
{
    u16 first = *(volatile u16 *)self;
    u16 second = *(volatile u16 *)(self + 2u);
    u16 third;
    *(volatile u16 *)self = (u16)((first >> 8) | (first << 8));
    third = *(volatile u16 *)(self + 4u);
    *(volatile u16 *)(self + 2u) = (u16)((second >> 8) | (second << 8));
    *(volatile u16 *)(self + 4u) = (u16)((third >> 8) | (third << 8));
    return 1;
}
