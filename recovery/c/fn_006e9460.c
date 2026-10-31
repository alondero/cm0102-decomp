#include "cmre.h"

u32 CM_THISCALL cm_fn_006e9460(u32 self)
{
    *(volatile u32 *)self = 0;
    *(volatile u32 *)(self + 4u) = 0;
    return self;
}
