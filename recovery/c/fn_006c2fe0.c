#include "cmre.h"

u32 CM_THISCALL cm_fn_006c2fe0(u32 self, u32 first, u32 second)
{
    u32 a = (u32)(s32)(s8)first;
    u32 b = (u32)(s32)(s8)second;
    u32 index = a * 20u + b;
    return self + index * 446u + 0x4796u;
}
