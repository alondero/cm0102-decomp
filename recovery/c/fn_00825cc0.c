#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);

u32 CM_CDECL cm_fn_00825cc0(void)
{
    u32 result = cm_fn_007fe600(0x00b74450, 0x00825d10, 0x00826690, 0, 0, 0);
    if (result) {
        cm_fn_007ff270(0x00b74450, 0, 0, 0);
        cm_fn_007ff270(0x00b74450, 1, 0, 0);
        result = cm_fn_007ff270(0x00b74450, 2, 0, 0);
    }
    return result;
}

