#include "cmre.h"

u32 CM_THISCALL cm_fn_0081b800(u32 self)
{
    *(volatile u8 *)(self + 8u) = 0xffu;
    *(volatile u8 *)(self + 9u) = 0xffu;
    *(volatile u32 *)(self + 4u) = 4u;
    *(volatile u32 *)self = 0x81u;
    *(volatile u32 *)(self + 10u) = 0;
    *(volatile u16 *)(self + 14u) = 1;
    *(volatile u8 *)(self + 16u) = 1;
    return self;
}
