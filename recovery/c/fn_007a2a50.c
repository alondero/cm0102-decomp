#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first, u32 second,
                                      u32 third, u32 fourth, u32 fifth);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 first, u32 second, u32 third);

void CM_CDECL cm_fn_007a2a50(volatile u32 *first, volatile u32 *second)
{
    if (!first || !second)
        return;
    if (!cm_fn_007fe600(0x00b74450, 0x007a2ab0, 0x007a2f10, 1, 0x005a8f60, 0))
        return;
    cm_fn_007ff270(0x00b74450, 0, *first, 0);
    cm_fn_007ff270(0x00b74450, 1, *second, 0);
}
