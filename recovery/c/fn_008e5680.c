#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0054a110(u32 self);

u32 CM_THISCALL cm_fn_008e5680(u32 self)
{
    cm_fn_0054a110(self + 0x12u);
    *(volatile u32 *)self = 0;
    *(volatile u32 *)(self + 4u) = 0;
    return self;
}
