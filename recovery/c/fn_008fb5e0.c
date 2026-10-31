#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2,
                                      u32 arg3, u32 arg4, u32 arg5);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 arg1, u32 arg2, u32 arg3);

void CM_CDECL cm_fn_008fb5e0(u32 first, s8 second, u16 third, s8 fourth,
                             u32 fifth, u32 sixth, u32 seventh)
{
    u32 initialized = cm_fn_007fe600(0x00b74450u, 0x008fb6a0u,
                                     0x008fb9e0u, 1u, 0u, 0u);
    if (initialized == 0)
        return;

    cm_fn_007ff270(0x00b74450u, 0u, first, 0u);
    cm_fn_007ff270(0x00b74450u, 1u, 0xffffffffu, 0u);
    cm_fn_007ff270(0x00b74450u, 2u, (u32)(s32)second, 0u);
    cm_fn_007ff270(0x00b74450u, 3u, (u16)third, 0u);
    cm_fn_007ff270(0x00b74450u, 4u, (u32)(s32)fourth, 0u);
    cm_fn_007ff270(0x00b74450u, 5u, fifth, 0u);
    cm_fn_007ff270(0x00b74450u, 6u, sixth, 0u);
    cm_fn_007ff270(0x00b74450u, 7u, seventh, 0u);
}
