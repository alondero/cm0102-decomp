#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 a, u32 b, u32 c, u32 d, u32 e);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 slot, u32 value, u32 flag);

u32 CM_CDECL cm_fn_00778f60(u32 first, u32 second, u32 third)
{
    u32 result = cm_fn_007fe600(0x00b74450, 0x00778fd0, 0x00779610, 1, 0, 0);
    if (result == 0)
        return 0;
    cm_fn_007ff270(0x00b74450, 0, first, 0);
    cm_fn_007ff270(0x00b74450, 1, (u32)(s32)(s8)second, 0);
    cm_fn_007ff270(0x00b74450, 2, third, 0);
    return cm_fn_007ff270(0x00b74450, 3, 0xffffffffu, 0);
}
