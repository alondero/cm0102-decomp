#include "cmre.h"

u32 CM_THISCALL cm_fn_005b2f10(u32 self, u32 value)
{
    u32 i;
    for (i = 0; i < 11u; ++i)
        if (*(volatile u16 *)(self + 0x12du + i * 2u) == (u16)value)
            return i;
    return 0xffu;
}
