#include "cmre.h"

u32 CM_THISCALL cm_fn_00468b40(u32 self, u32 arg)
{
    u32 value = 0;
    if ((u16)arg != 0xffffu) {
        u32 row = (u32)(s32)*(volatile s16 *)(self + 0x382au);
        u32 index = (u32)(s32)(s16)arg;
        u32 pointer = *(volatile u32 *)(self + row * 0x300u + 0x7fcu);
        value = *(volatile u32 *)(pointer + index * 396u + 0xba966u);
    }
    if ((u16)value == 0)
        return (u32)(s32)*(volatile s16 *)(self + 0x26278eu);
    return (u32)(s32)(s16)value;
}
