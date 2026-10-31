#include "cmre.h"

extern u8 *CM_THISCALL cm_fn_008e84a0(u8 *object, u32 arg1, s32 arg2, s32 arg3, u32 arg4, u32 arg5);

u32 CM_THISCALL cm_fn_00893d80(u32 self, u32 first, s32 second, s32 third, u32 fourth, u32 fifth)
{
    cm_fn_008e84a0((u8 *)self, first, second, third, fourth, fifth);
    *(volatile u32 *)self = 0x00970368u;
    *(volatile u8 *)(self + 8u) = 0;
    *(volatile u32 *)(self + 4u) = 0;
    *(volatile u32 *)(self + 0x15u) = 1;
    return self;
}
