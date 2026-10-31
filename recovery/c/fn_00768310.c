#include "cmre.h"

extern void CM_THISCALL cm_fn_0054c5b0(u16 *self, u32 *buffer, u32 mode);
extern s32 CM_FASTCALL cm_fn_0054cfb0(u16 *self);
extern void CM_THISCALL cm_fn_00767f60(u8 *self, u8 value, u16 previous);

void CM_THISCALL cm_fn_00768310(u8 *self, u32 argument)
{
    volatile struct { u32 prev; u32 handler; u32 state; } frame;
    volatile u8 scratch[8];

    frame.state = 0xffffffffu;
    frame.handler = 0x0095dbc8u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_0054c5b0((u16 *)argument, (u32 *)&scratch[0], 1u);
    u32 current = *(volatile u32 *)(self + 0x0au);
    frame.state = 0;
    if (current != 0) {
        u8 stored = *(volatile u8 *)(current + 0x0cu);
        u8 converted = (u8)cm_fn_0054cfb0((u16 *)&scratch[0]);
        if (stored == converted) {
            current = *(volatile u32 *)(self + 0x0au);
            u32 local_word = *(volatile u32 *)((u32)&scratch[0] + 2u) & 0xffffu;
            s32 current_word = *(volatile s16 *)(current + 0x0du);
            if (current_word == (s32)local_word)
                goto cleanup;
        }

        u32 vtable = *(volatile u32 *)self;
        u32 slot = *(volatile u32 *)(vtable + 0x0cu);
        ((void (CM_THISCALL *)(u8 *))slot)(self);

        u32 previous = *(volatile u32 *)((u32)&scratch[0] + 2u) & 0xffffu;
        u8 value = (u8)cm_fn_0054cfb0((u16 *)&scratch[0]);
        current = *(volatile u32 *)(self + 0x0au);
        cm_fn_00767f60((u8 *)current, value, (u16)previous);
    }

cleanup:
    u32 saved_prev = frame.prev;
    CM_FS(u32, 0) = saved_prev;
}
