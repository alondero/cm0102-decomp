#include "cmre.h"

extern u32 CM_CDECL cm_fn_005e4800(u32 arg1, u32 arg2, u32 arg3, u32 *arg4);
extern void CM_CDECL cm_fn_005e49f0(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4);

u32 CM_THISCALL cm_fn_004041d0(volatile u8 *self, u32 mode)
{
    volatile u8 local[40];
    u32 result, third, second, first;
    *(volatile u32 *)(local + 24) = mode ? 0xf800 : 0x7c00;
    *(volatile u32 *)(local + 28) = mode ? 0x7e0 : 0x3e0;
    *(volatile u32 *)(local + 32) = 0x1f;
    cm_fn_005e49f0(*(volatile u16 *)(self + 0x20c), (u32)(local + 4),
                   (u32)local, (u32)&mode, (u32)(local + 8));
    third = mode;
    second = *(volatile u32 *)local;
    first = *(volatile u32 *)(local + 4);
    result = cm_fn_005e4800(first, second, third, 0);
    *(volatile u16 *)(self + 0x20c) = (u16)result;
    return result;
}
