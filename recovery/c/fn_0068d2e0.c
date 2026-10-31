#include "cmre.h"

u32 CM_THISCALL cm_fn_0068d2e0(u32 self)
{
    u32 pointer = *(volatile u32 *)(self + 0xau);
    if (pointer != 0)
        return *(volatile u32 *)pointer;
    return 0;
}
