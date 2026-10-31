#include "cmre.h"

u32 CM_THISCALL cm_fn_0068d020(u32 self)
{
    *(volatile u32 *)(self + 2u) = 0;
    *(volatile u32 *)(self + 6u) = 0;
    *(volatile u32 *)(self + 10u) = 0;
    *(volatile u16 *)self = 0;
    return self;
}
