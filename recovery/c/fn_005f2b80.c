#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 a, u32 b, u32 c, u32 d, u32 e);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 slot, u32 value, u32 flag);

u32 CM_CDECL cm_fn_005f2b80(u32 value)
{
    u32 result = cm_fn_007fe600(0x00b74450, 0x005f2c00, 0x005f31c0, 0, 0x005f33d0, 0);
    if (result == 0)
        return 0;
    cm_fn_007ff270(0x00b74450, 0, value, 0);
    cm_fn_007ff270(0x00b74450, 1, 0, 0);
    cm_fn_007ff270(0x00b74450, 2, 0, 0);
    cm_fn_007ff270(0x00b74450, 3, 0, 0);
    return cm_fn_007ff270(0x00b74450, 4, 0, 0);
}
