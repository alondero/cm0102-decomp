#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg2, u32 arg3,
                                      u32 arg4, u32 arg5, u32 arg6);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 arg2, u32 arg3, u32 arg4);

void CM_CDECL cm_fn_0047c0b0(u32 value)
{
    u32 initialized = cm_fn_007fe600(0x00b74450u, 0x0047c180u, 0x004814e0u,
                                    1u, 0u, 0u);
    if (initialized == 0)
        return;

    u32 first_value = value;
    cm_fn_007ff270(0x00b74450u, 0u, first_value, 0u);
    cm_fn_007ff270(0x00b74450u, 1u, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 2u, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 3u, 0u, 0u);
    cm_fn_007ff270(0x00b74450u, 4u, 0x41u, 0u);
    cm_fn_007ff270(0x00b74450u, 5u, 8u, 0u);
    cm_fn_007ff270(0x00b74450u, 6u, 6u, 0u);
    cm_fn_007ff270(0x00b74450u, 7u, 0xffffffffu, 0u);
    cm_fn_007ff270(0x00b74450u, 8u, 0xffffffffu, 0u);
    cm_fn_007ff270(0x00b74450u, 9u, 0x00154e21u, 0u);
}
