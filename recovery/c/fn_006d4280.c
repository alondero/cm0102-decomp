#include "cmre.h"

u32 CM_THISCALL cm_fn_006d4280(u32 self)
{
    u32 pointer = *(volatile u32 *)(self + 0x4792u);
    u32 value = *(volatile u32 *)pointer;
    return (pointer & 0xffffff00u) | (value == 0xfffffffeu);
}
