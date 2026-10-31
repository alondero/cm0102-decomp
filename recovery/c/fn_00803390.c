#include "cmre.h"

extern void CM_STDCALL cm_fn_005621f0(u32 first, u32 second);

void CM_THISCALL cm_fn_00803390(u32 self, u32 first, u32 second)
{
    s16 index = *(volatile s16 *)(self + 0x382au);
    u32 receiver = *(volatile u32 *)(self + 0x7fcu + (u32)(s32)index * 0x300u);
    (void)receiver;
    cm_fn_005621f0(first, second);
}
