#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first, u32 second,
                                      u32 third, u32 fourth, u32 fifth);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 first, u32 second, u32 third);

u32 CM_CDECL cm_fn_008f8be0(volatile u32 argument)
{
    if (!cm_fn_007fe600(0x00b74450, 0x008f8c30, 0x008f9080, 1, 0, 0))
        return 0;
    cm_fn_007ff270(0x00b74450, 0, argument, 0);
    return cm_fn_007ff270(0x00b74450, 1, 2, 0);
}
