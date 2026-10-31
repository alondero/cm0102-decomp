#include "cmre.h"

extern u16 *CM_THISCALL cm_fn_00549f70(
    u16 *self, s16 arg1, s8 arg2, u16 arg3, u32 arg4);

u32 CM_FASTCALL cm_fn_0092d200(volatile u8 *self)
{
    struct seh_frame { u32 prev, handler, state; } volatile frame;
    volatile u8 local[16];
    u16 year;
    u16 result_word;
    u32 vtable;
    u32 slot;
    u32 result;
    u32 restore_prev;

    frame.state = 0xffffffffu;
    frame.handler = 0x00965fd0u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;

    *(volatile u8 *)(self + 0x43) = 2;
    *(volatile u8 *)(self + 0xc7) = 2;
    *(volatile u8 *)(self + 0xbe) = 2;
    year = *(volatile u16 *)(self + 0x40);
    *(volatile u8 *)(self + 0xc2) = 3;
    *(volatile u8 *)(self + 0x4a) = 3;
    *(volatile u16 *)(self + 0x3c) = 1;
    *(volatile u8 *)(self + 0xc3) = 1;
    *(volatile u8 *)(self + 0xc4) = 0x0a;
    *(volatile u8 *)(self + 0xc5) = 4;
    *(volatile u8 *)(self + 0xc6) = 1;
    *(volatile u8 *)(self + 0xc8) = 0;
    *(volatile u8 *)(self + 0xbf) = 0;
    *(volatile u8 *)(self + 0xc0) = 0;
    *(volatile u8 *)(self + 0xc1) = 0;
    *(volatile u16 *)(self + 0x3a) = 0x3e8;
    *(volatile u8 *)(self + 0x49) = 7;

    (void)cm_fn_00549f70((u16 *)local, 0x0f, 11, year, 0xffffffffu);
    year = *(volatile u16 *)(self + 0x40);
    result_word = *(volatile u16 *)local;
    *(volatile u16 *)(self + 0x18e) = result_word;

    frame.state = 0;
    *(volatile u16 *)(self + 0x190) = 0xffff;
    (void)cm_fn_00549f70((u16 *)(local + 8), 0x1b, 11, year, 0xffffffffu);
    result_word = *(volatile u16 *)(local + 8);
    vtable = *(volatile u32 *)self;
    *(volatile u16 *)(self + 0xb6) = result_word;

    *(volatile u8 *)&frame.state = 1;
    *(volatile u16 *)(self + 0xb8) = 0xffff;
    slot = *(volatile u32 *)(vtable + 0x3c);
    result = ((u32 (CM_THISCALL *)(void *, s32, void *, void *, u32))
        (unsigned long)slot)((void *)self, -1, (void *)(self + 0xa9),
            (void *)(self + 0x3a), 0);
    restore_prev = frame.prev;
    *(volatile u32 *)(self + 0xba) = result;
    CM_FS(u32, 0) = restore_prev;
    return 1;
}
