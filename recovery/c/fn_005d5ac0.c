#include "cmre.h"

u32 CM_THISCALL cm_fn_005d5ac0(u32 self, u32 arg)
{
    u32 base = *(volatile u32 *)(self + 0x651du);
    u32 pointer = *(volatile u32 *)(arg + 0x61u);
    u32 index = *(volatile u32 *)pointer;
    return base + index * 6u;
}
