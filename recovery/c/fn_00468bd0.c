#include "cmre.h"

u32 CM_THISCALL cm_fn_00468bd0(u32 self, u32 arg)
{
    u32 row, index, pointer, offset;
    u16 value;
    if ((u16)arg == 0xffffu)
        return 0xffffffffu;
    row = (u32)(s32)*(volatile s16 *)(self + 0x382au);
    index = (u32)(s32)(s16)arg;
    pointer = *(volatile u32 *)(self + row * 0x300u + 0x7fcu);
    offset = index * 99u;
    value = *(volatile u16 *)(pointer + offset * 4u + 0xba9dcu);
    return (offset & 0xffff0000u) | value;
}
