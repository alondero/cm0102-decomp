#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);

u32 CM_CDECL cm_fn_0058acd0(u32 argument)
{
    u32 result = cm_fn_007fe600(0x00b74450, 0x0058ad30, 0x0058b0f0,
                                1, 0x005a8f60, 0);
    if (result) {
        u32 value = CM_GLOBAL(volatile u32, argument);
        cm_fn_007ff270(0x00b74450, 0, value, 0);
        cm_fn_007ff270(0x00b74450, 1, 0, 0);
        result = cm_fn_007ff270(0x00b74450, 2, 0, 0);
    }
    return result;
}

