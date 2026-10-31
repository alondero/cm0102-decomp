#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007af160(u32 self, u32 first, u32 second, u32 third);

u32 CM_THISCALL cm_fn_00804a40(u32 self, u32 out_first, u32 out_second, u32 argument)
{
    u32 first = self + 0x361e;
    u32 initialized;
    *(volatile u32 *)out_first = first;
    initialized = *(volatile u32 *)(self + 0x3634);
    if (initialized == 0) {
        s32 index = *(volatile s8 *)self;
        u32 receiver = self + (u32)index * 688u + 1;
        u32 result = cm_fn_007af160(receiver, first, self + 0x3630, argument);
        *(volatile u32 *)(self + 0x362c) = result;
        *(volatile u32 *)(self + 0x3634) = 1;
    }
    *(volatile u32 *)out_second = *(volatile u32 *)(self + 0x3630);
    return *(volatile u32 *)(self + 0x362c);
}
