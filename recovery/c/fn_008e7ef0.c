#include "cmre.h"

u32 CM_THISCALL cm_fn_008e7ef0(u32 self)
{
    u32 first = *(volatile u32 *)(self + 0x35u);
    u32 result = (s32)first > 0 ? first : 0;
    u32 second = *(volatile u32 *)(self + 0x3au);
    if ((s32)second > 0)
        result += second;
    return result;
}
