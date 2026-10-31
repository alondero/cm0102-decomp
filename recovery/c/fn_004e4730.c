#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2,
                                     u32 arg3, u32 arg4, u32 arg5);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 slot,
                                      u32 value, u32 arg3);

void CM_CDECL cm_fn_004e4730(u32 arg1, s8 arg2, s8 arg3, u32 arg4,
                             u32 arg5, s8 arg6)
{
    u32 initial = cm_fn_007fe600(0x00b74450u, 0x004e4820u, 0x004e4ea0u,
                                 1u, 0u, 0u);
    if (initial == 0) {
        return;
    }

    cm_fn_007ff270(0x00b74450u, 0u, arg1, 0u);
    cm_fn_007ff270(0x00b74450u, 3u, (u32)(s32)arg2, 0u);
    cm_fn_007ff270(0x00b74450u, 4u, (u32)(s32)arg3, 0u);
    cm_fn_007ff270(0x00b74450u, 5u, arg4, 0u);
    cm_fn_007ff270(0x00b74450u, 6u, (u32)(s32)arg6, 0u);
    cm_fn_007ff270(0x00b74450u, 7u, arg5, 0u);

    {
        if (arg1 != 0) {
            u32 object = *(volatile u32 *)arg1;
            if (object != 0) {
                u32 value = *(volatile u32 *)(object + 0x20u);
                cm_fn_007ff270(0x00b74450u, 1u, value, 0u);
                object = *(volatile u32 *)arg1;
                value = *(volatile u32 *)(object + 0x24u);
                cm_fn_007ff270(0x00b74450u, 2u, value, 0u);
            } else {
                cm_fn_007ff270(0x00b74450u, 1u, 0xffffffffu, 0u);
                cm_fn_007ff270(0x00b74450u, 2u, 0xffffffffu, 0u);
            }
        } else {
            cm_fn_007ff270(0x00b74450u, 1u, 0xffffffffu, 0u);
            cm_fn_007ff270(0x00b74450u, 2u, 0xffffffffu, 0u);
        }
    }
}
