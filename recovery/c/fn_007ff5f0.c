#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fc690(u32 self);
extern void CM_THISCALL cm_fn_0077a890(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);

u32 CM_THISCALL cm_fn_007ff5f0(volatile u8 *self)
{
    u32 pointer = *(volatile u32 *)(self + 0x3840);
    u32 result;
    if (!pointer)
        return 0;
    result = cm_fn_007fc690((u32)self);
    pointer = *(volatile u32 *)(self + 0x3840);
    if (pointer) {
        cm_fn_0077a890(pointer);
        result = (u32)cm_fn_00944c94((void *)pointer);
    }
    *(volatile u32 *)(self + 0x3840) = 0;
    return result;
}
