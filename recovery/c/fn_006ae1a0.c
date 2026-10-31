#include "cmre.h"

/* The global object at 0x00b74450 is passed as ECX to every call here. */
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3,
                                      u32 param_4, u32 param_5, u32 param_6);
extern void CM_THISCALL cm_fn_007ff270(u32 self, s32 index, u32 value,
                                       s32 zero);

void CM_CDECL cm_fn_006ae1a0(void)
{
    u32 object;
    u32 table_index;
    u32 p;
    u32 value;
    u32 q;

    if (cm_fn_007fe600(0x00b74450u, 0x006ae2a0u, 0x006b05e0u, 0u,
                       0x006b09d0u, 0u) == 0) {
        return;
    }

    table_index = ((u32)(s32)(s16)CM_GLOBAL(volatile u16, 0x00b77c7au) * 3u) << 8;
    p = CM_GLOBAL(volatile u32, 0x00b74c26u + table_index);
    if (p == 0) {
        value = 0xffffffffu;
    } else {
        object = *(volatile u32 *)(p + 0x39u);
        if (object != 0) {
            q = *(volatile u32 *)(object + 0x53u);
            value = *(volatile u32 *)q;
        } else {
            object = *(volatile u32 *)(p + 0x24u);
            if (object != 0) {
                q = *(volatile u32 *)(object + 0x53u);
                value = *(volatile u32 *)q;
            } else {
                q = *(volatile u32 *)(p + 0x1au);
                if ((*(volatile u8 *)(q + 0x11cu) & 2u) != 0) {
                    value = *(volatile u32 *)q;
                } else {
                    value = 0xffffffffu;
                }
            }
        }
    }

    cm_fn_007ff270(0x00b74450u, 0, value, 0);
    cm_fn_007ff270(0x00b74450u, 1, 0, 0);
    cm_fn_007ff270(0x00b74450u, 2, 0, 0);
    cm_fn_007ff270(0x00b74450u, 3, 1, 0);
    cm_fn_007ff270(0x00b74450u, 4, 1, 0);
    cm_fn_007ff270(0x00b74450u, 5, 0, 0);
    cm_fn_007ff270(0x00b74450u, 6, 0, 0);
    cm_fn_007ff270(0x00b74450u, 7, 1, 0);
    cm_fn_007ff270(0x00b74450u, 8, 1, 0);
}
