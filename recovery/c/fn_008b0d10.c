#include "cmre.h"
extern u8 CM_THISCALL cm_fn_008b5000(u8 *self, u32 arg1, u32 arg2, u32 arg3);

u8 CM_THISCALL cm_fn_008b0d10(u32 self, u32 argument)
{
    s32 index;
    if ((u8)argument == 0xff)
        index = *(volatile s8 *)(self + 0x7d9);
    else
        index = (s8)argument;
    return cm_fn_008b5000((u8 *)(self + (u32)index * 155 + 0xfa), 0, self, 0);
}
