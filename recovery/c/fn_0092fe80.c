#include "cmre.h"

extern void CM_THISCALL cm_fn_00412e40(u32 *self, u32 arg1, u32 arg2,
                                       s32 arg3, u16 arg4, u16 arg5,
                                       s32 arg6, u32 arg7, u32 arg8);
extern u32 * CM_THISCALL cm_fn_00417440(u32 *self, u32 arg1, s32 arg2);
extern u32 CM_THISCALL cm_fn_0092fff0(u32 *self, s32 arg1, s32 arg2);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);

u32 * CM_THISCALL cm_fn_0092fe80(u32 *self,
                                  u32 arg1, u32 arg2, u32 arg3,
                                  u32 arg4, u32 arg5, u32 arg6,
                                  u32 arg7, u32 arg8, u32 arg9,
                                  u32 arg10, u32 arg11)
{
    volatile struct {
        u32 prev;
        u32 handler;
        u32 state;
    } frame;

    frame.state = 0xffffffffu;
    frame.handler = 0x00966083u;
    frame.prev = (u32)CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)(void *)&frame;

    cm_fn_00412e40(self, arg1, arg2, (s32)arg3,
                   (u16)arg4, (u16)arg5, (s32)arg6, arg10, arg11);

    u8 saved_arg7 = *(volatile u8 *)&arg7;
    u8 saved_arg9 = *(volatile u8 *)&arg9;
    u32 saved_arg8 = *(volatile u32 *)&arg8;
    frame.state = 0u;
    *(volatile u32 *)self = 0x009713b8u;
    *(volatile u32 *)((u8 *)self + 0x1f) = 0u;
    *(volatile u8 *)((u8 *)self + 0x1d) = saved_arg7;
    *(volatile u8 *)((u8 *)self + 0x1e) = saved_arg9;
    *(volatile u32 *)((u8 *)self + 0x23) = 0xffffffffu;
    *(volatile u32 *)((u8 *)self + 0x27) = 0xffffffffu;
    *(volatile u8 *)((u8 *)self + 4) = 1u;

    if (arg10 != 0u &&
        cm_fn_0092fff0(self, (s32)saved_arg8, (s32)arg10) != 0u) {
        CM_FS(u32, 0) = frame.prev;
        return self;
    }

    if (arg8 != 0u) {
        unsigned long long allocation = cm_fn_00944cf1(0x40u);
        *(volatile u8 *)&frame.state = 1u;
        u32 *object = (u32 *)(u32)allocation;
        u32 result;
        if (object != (u32 *)0)
            result = (u32)cm_fn_00417440(object, saved_arg8, 0);
        else
            result = 0u;
        *(volatile u32 *)((u8 *)self + 0x1f) = result;
    }

    CM_FS(u32, 0) = frame.prev;
    return self;
}
