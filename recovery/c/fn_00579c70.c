#include "cmre.h"

extern short *CM_THISCALL cm_fn_00549f70(short *self, u32 arg1, u32 arg2,
                                        u32 arg3, u32 arg4);
extern u32 CM_THISCALL cm_fn_0054cfb0(void *self);
extern void CM_STDCALL cm_fn_008e8bd0(u32 arg1, u32 arg2, u32 arg3,
                                      u32 arg4, u32 arg5, u32 arg6,
                                      u32 arg7);

void CM_THISCALL cm_fn_00579c70(void *self, u32 arg1, u32 arg2, u32 arg3,
                                u32 arg4, u32 arg5, u32 arg6, u32 arg7)
{
    volatile struct { u32 previous, handler, state; } frame;
    volatile u8 local_area[8];
    short *helper_result;

    (void)self;

    frame.state = 0xffffffffu;
    frame.handler = 0x00958708u;
    frame.previous = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    helper_result = cm_fn_00549f70((short *)&local_area[0], 0x13u, 5u,
                                   0x7ceu, 0xffffffffu);
    (void)helper_result;
    {
        s16 current_month = CM_GLOBAL(volatile s16, 0x00ae2c90u);
        s16 local_month;
        frame.state = 0u;
        local_month = *(volatile s16 *)&local_area[0];
        if (current_month <= local_month ||
            *(volatile s8 *)(arg7 + 0x18u) >= 0x18) {
            u32 month_result = cm_fn_0054cfb0((void *)0x00ae2c90u);
            volatile u8 *month1 = (volatile u8 *)arg3;
            volatile u16 *year1 = (volatile u16 *)arg4;
            volatile u8 *month2 = (volatile u8 *)arg5;
            volatile u16 *year2 = (volatile u16 *)arg6;
            u8 value = (u8)((u8)month_result + 1u);

            *month1 = value;
            *year1 = CM_GLOBAL(volatile u16, 0x00ae2c92u);
            value = *month1;
            if ((s8)value > 11) {
                value = (u8)(value - 12u);
                *month1 = value;
                *year1 = (u16)(*year1 + 1u);
            }
            value = *month1;
            *month2 = (u8)(value + 3u);
            *year2 = *year1;
            value = *month2;
            if ((s8)value > 11) {
                *month2 = (u8)(value - 12u);
                *year2 = (u16)(*year2 + 1u);
            }
        } else {
            cm_fn_008e8bd0(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
    }
    CM_FS(u32, 0) = frame.previous;
}
