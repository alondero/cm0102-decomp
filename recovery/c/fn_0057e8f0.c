#include "cmre.h"

extern short *CM_THISCALL cm_fn_00549f70(short *self, short a, char b, u16 c, u32 d);
typedef u32 (CM_THISCALL *cm_slot_3c)(void *self, u32 a, u32 b, u32 c, u32 d);

u32 CM_THISCALL cm_fn_0057e8f0(void *self)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_area[32];
    volatile u8 *p = (volatile u8 *)self;
    volatile u16 *vtable;
    cm_slot_3c slot;
    u16 local_word;
    u16 saved_base;
    u32 result;
    u32 previous;

    frame.state = 0xffffffff;
    frame.handler = 0x009588a8;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    *(volatile u8 *)(p + 0xc2) = 3;
    *(volatile u8 *)(p + 0x4a) = 3;
    saved_base = *(volatile u16 *)(p + 0x40);
    *(volatile u16 *)(p + 0x3c) = 2;
    *(volatile u8 *)(p + 0xc3) = 1;
    *(volatile u8 *)(p + 0xc4) = 10;
    *(volatile u8 *)(p + 0x43) = 2;
    *(volatile u8 *)(p + 0xc5) = 1;
    *(volatile u8 *)(p + 0xc6) = 2;
    *(volatile u8 *)(p + 0xc7) = 4;
    *(volatile u8 *)(p + 0xc8) = 5;
    *(volatile u8 *)(p + 0xbe) = 1;
    *(volatile u8 *)(p + 0xbf) = 1;
    *(volatile u8 *)(p + 0xc0) = 0;
    *(volatile u8 *)(p + 0xc1) = 0;
    *(volatile u8 *)(p + 0x49) = 7;
    *(volatile u16 *)(p + 0xa7) = 0xffff;
    (void)cm_fn_00549f70((short *)(local_area + 16), 10, 2,
                          saved_base, 5);
    local_word = *(volatile u16 *)(local_area + 16);
    vtable = *(volatile u16 * volatile *)p;
    *(volatile u16 *)(p + 0xb6) = local_word;
    frame.state = 0;
    *(volatile u16 *)(p + 0xb8) = 0;
    slot = *(cm_slot_3c volatile *)((volatile u8 *)vtable + 0x3c);
    result = slot(self, 0xffffffff, (u32)(p + 0xa9), (u32)(p + 0x3a), 0);
    previous = frame.prev;
    *(volatile u32 *)(p + 0xba) = result;
    CM_FS(u32, 0) = previous;
    return 1;
}
