#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);

u32 CM_CDECL cm_fn_008f8120(u32 first, u32 second)
{
    u32 result = cm_fn_007fe600(0x00b74450, 0x008f8170, 0x008f8450, 1, 0, 0);
    if (result) {
        cm_fn_007ff270(0x00b74450, 0, first, 0);
        result = cm_fn_007ff270(0x00b74450, 1, second, 0);
    }
    return result;
}

