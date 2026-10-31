#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);

u32 CM_CDECL cm_fn_008f4c70(u32 argument)
{
    u32 result = cm_fn_007fe600(0x00b74450, 0x008f4cb0, 0x008f4ed0, 1, 0, 0);
    if (result)
        result = cm_fn_007ff270(0x00b74450, 0, argument, 0);
    return result;
}

