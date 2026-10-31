#include "cmre.h"

extern short *CM_THISCALL cm_fn_00549f70(short *self, short a, char b, u16 c, u32 d);
typedef u32 (CM_THISCALL *cm_slot_3c)(void *self, u32 a, u32 b, u32 c, u32 d);

u32 CM_THISCALL cm_fn_004c3890(void *self)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_area[16];
    volatile u8 *p = (volatile u8 *)self;
    volatile u8 *vtable;
    cm_slot_3c slot;
    u16 base;
    u16 local_word;
    u32 result;
    u32 previous;

    frame.state = 0xffffffff;
    frame.handler = 0x00956a78;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    *(volatile u16 *)(p + 0x3c) = 2;
    *(volatile u8 *)(p + 0xc6) = 2;
    *(volatile u8 *)(p + 0xbe) = 2;
    *(volatile u8 *)(p + 0x42) = 2;
    base = *(volatile u16 *)(p + 0x40);
    *(volatile u8 *)(p + 0xc2) = 3;
    *(volatile u8 *)(p + 0xc7) = 3;
    *(volatile u8 *)(p + 0x4a) = 3;
    *(volatile u8 *)(p + 0xc3) = 1;
    *(volatile u8 *)(p + 0xc4) = 7;
    *(volatile u8 *)(p + 0xc5) = 1;
    *(volatile u8 *)(p + 0xbf) = 0;
    *(volatile u8 *)(p + 0xc0) = 0;
    *(volatile u8 *)(p + 0xc1) = 0;
    *(volatile u32 *)(p + 0x1c) = 0xffffffff;
    *(volatile u32 *)(p + 0x20) = 0xffffffff;
    *(volatile u8 *)(p + 0x49) = 7;
    (void)cm_fn_00549f70((short *)(local_area + 8), 15, 0, base, 0xffffffff);
    local_word = *(volatile u16 *)(local_area + 8);
    vtable = *(volatile u8 * volatile *)p;
    *(volatile u16 *)(p + 0xb6) = local_word;
    frame.state = 0;
    *(volatile u16 *)(p + 0xb8) = 0;
    *(volatile u32 *)(p + 0x28) = 0x001e8480;
    slot = *(cm_slot_3c volatile *)(vtable + 0x3c);
    result = slot(self, 0xffffffff, (u32)(p + 0xa9), (u32)(p + 0x3a), 0);
    previous = frame.prev;
    *(volatile u32 *)(p + 0xba) = result;
    CM_FS(u32, 0) = previous;
    return 1;
}
