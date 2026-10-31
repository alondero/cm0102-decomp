#include "cmre.h"

extern u8 *CM_THISCALL cm_fn_00549f70(u8 *record, u32 arg1, u32 arg2,
                                    u16 year, u32 arg4);
extern void CM_THISCALL cm_fn_0085a9b0(u32 owner, u32 mode);

void CM_CDECL cm_fn_009058e0(void)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local[0x20];
    u16 year;
    u32 first;
    u32 second;
    u32 mode = 0u;

    frame.state = 0xffffffffu;
    frame.handler = 0x00965268u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;

    year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    (void)cm_fn_00549f70((u8 *)&local[8], 4u, 7u, year, 4u);
    year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    frame.state = 0u;
    (void)cm_fn_00549f70((u8 *)&local[0x10], 1u, 8u, year, 4u);
    year = CM_GLOBAL(volatile u16, 0x00ae2c92u);
    *(volatile u8 *)&frame.state = 1u;
    (void)cm_fn_00549f70((u8 *)&local[0x18], 2u, 1u, year, 1u);

    first = CM_GLOBAL(volatile u32, 0x00ae2c90u);
    second = CM_GLOBAL(volatile u32, 0x00ae2c94u);
    *(volatile u32 *)&local[0] = first;
    {
        int first_high_matches =
            *(volatile u16 *)&local[2] ==
            *(volatile u16 *)&local[0x0au];
        *(volatile u8 *)&frame.state = 2u;
        *(volatile u32 *)&local[4] = second;

        if (first_high_matches &&
            (u16)first == *(volatile u16 *)&local[8]) {
            mode = 1u;
        } else {
            *(volatile u32 *)&local[0] = first;
            {
                int second_high_matches =
                    *(volatile u16 *)&local[2] ==
                    *(volatile u16 *)&local[0x12u];
                *(volatile u32 *)&local[4] = second;
                if (second_high_matches &&
                    (u16)first ==
                    *(volatile u16 *)&local[0x10u]) {
                    mode = 2u;
                } else {
                    *(volatile u32 *)&local[0] = first;
                    *(volatile u32 *)&local[4] = second;
                    if (*(volatile u16 *)&local[2] ==
                            *(volatile u16 *)&local[0x1au] &&
                        (u16)first ==
                            *(volatile u16 *)&local[0x18u] &&
                        CM_GLOBAL(volatile u16, 0x00ae2c92u) != 0x07d1u)
                        mode = 3u;
                }
            }
        }
    }

    if (mode != 0u)
        cm_fn_0085a9b0(CM_GLOBAL(volatile u32, 0x00dd7ef0u), mode);

    CM_FS(u32, 0) = frame.prev;
}
