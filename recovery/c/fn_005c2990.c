#include "cmre.h"

u32 CM_THISCALL cm_fn_005c2990(u32 self, u32 arg)
{
    u32 value = *(volatile u32 *)(self + 4u);
    return arg == value;
}
