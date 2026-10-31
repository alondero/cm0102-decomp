#include "cmre.h"

u32 CM_THISCALL cm_fn_00802ce0(u32 self, u32 selector)
{
    u32 row = (u32)(s32)*(volatile s16 *)(self + 0x382au);
    u32 column = (u32)(s32)(s8)selector;
    u32 index = row * 10u + column;
    *(volatile u32 *)(self + index * 4u + 0x26250au) = 0;
    return index;
}
