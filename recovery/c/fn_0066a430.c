#include "cmre.h"

u32 CM_THISCALL cm_fn_0066a430(u32 self, u32 arg)
{
    u32 flag = *(volatile u32 *)(self + 10u);
    s8 left;
    s8 right;
    if (flag != 0)
        left = *(volatile s8 *)(arg + 0x38u);
    else
        left = *(volatile s8 *)(arg + 4u);
    right = *(volatile s8 *)self;
    return (arg & 0xffffff00u) | (left > right);
}
