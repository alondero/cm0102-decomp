#include "cmre.h"

u32 CM_THISCALL cm_fn_00768290(u32 self)
{
    *(volatile u32 *)self = 0x0096da10u;
    *(volatile u32 *)(self + 4u) = 0;
    *(volatile u16 *)(self + 8u) = 0;
    *(volatile u32 *)(self + 10u) = 0;
    return self;
}
