#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
u32 CM_CDECL cm_fn_00826a60(u32 argument)
{
    u32 option = CM_GLOBAL(volatile u32, 0x00dd7118);
    u32 result = cm_fn_007fe600(0x00b74450, 0x00826aa0, 0x00826c20, option, 0, 0);
    if (result) result = cm_fn_007ff270(0x00b74450, 0, argument, 0);
    return result;
}
