#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0048c580(void *self);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);

u32 CM_THISCALL cm_fn_00682200(void *self)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 *p = (volatile u8 *)self;
    u32 previous;

    frame.state = 0xffffffff;
    frame.handler = 0x0095c926;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    (void)cm_fn_0048c580(self);
    frame.state = 0;
    cm_fn_0054a110((u16 *)(p + 0xc9));
    *(volatile u8 *)&frame.state = 1;
    cm_fn_0054a110((u16 *)(p + 0xd1));
    previous = frame.prev;
    *(volatile u8 *)(p + 0x43) = 2;
    *(volatile u8 *)(p + 0xc4) = 2;
    *(volatile u8 *)(p + 0xe1) = 2;
    *(volatile u16 *)(p + 0xb6) = 0xffff;
    *(volatile u16 *)(p + 0xa7) = 0xffff;
    *(volatile u16 *)(p + 0xe2) = 0xffff;
    *(volatile u16 *)(p + 0xe4) = 0xffff;
    *(volatile u32 *)p = 0x0096d020;
    *(volatile u16 *)(p + 0xab) = 0;
    *(volatile u32 *)(p + 0xad) = 0;
    *(volatile u32 *)(p + 0x0c) = 0;
    *(volatile u8 *)(p + 0xb5) = 0;
    *(volatile u32 *)(p + 0xb1) = 0;
    *(volatile u16 *)(p + 0xdb) = 0;
    *(volatile u16 *)(p + 0xd9) = 3;
    *(volatile u32 *)(p + 0xe6) = 0;
    *(volatile u32 *)(p + 0xea) = 0;
    *(volatile u16 *)(p + 0xb8) = 0;
    *(volatile u32 *)(p + 0xdd) = 0;
    *(volatile u32 *)(p + 0xc5) = 0;
    *(volatile u32 *)(p + 0xa3) = 0;
    CM_FS(u32, 0) = previous;
    return (u32)self;
}
