#include "cmre.h"

u32 CM_THISCALL cm_fn_005b58e0(u32 self, u32 selector)
{
    u32 index = (u32)(s32)(s8)selector;
    u32 value = *(volatile u32 *)(self + index * 4u + 0x5bfu);
    if ((s32)value <= -1)
        return 0;
    return CM_GLOBAL(volatile u32, 0x00ae23bc) + value * 110u;
}
