#include "cmre.h"

extern short *CM_THISCALL cm_fn_00549f70(short *self, short arg1, char arg2, u16 arg3, u32 arg4);
typedef u32 (CM_THISCALL *cm_slot_3c)(void *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);

u32 CM_THISCALL cm_fn_00912eb0(void *self)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_result[8];
    volatile u8 *p = (volatile u8 *)self;
    u16 field_40;
    u16 result_value;
    u8 *vtable;
    cm_slot_3c slot;
    u32 result;
    u32 mixed;
    u32 previous;

    frame.state = 0xffffffff;
    frame.handler = 0x00965748;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    p[0xc2] = 3;
    p[0xc7] = 3;
    field_40 = *(volatile u16 *)(p + 0x40);
    mixed = ((u32)self & 0xffff0000u) | field_40;
    *(volatile u16 *)(p + 0x3c) = 2;
    p[0xc3] = 1;
    p[0xc4] = 0x0a;
    p[0x43] = 2;
    p[0xc5] = 1;
    p[0xc6] = 2;
    p[0xbe] = 1;
    p[0xbf] = 0;
    p[0xc0] = 0;
    p[0xc1] = 0;
    (void)cm_fn_00549f70((short *)local_result, 0x19, 2, (u16)mixed, 5);
    result_value = *(volatile u16 *)local_result;
    vtable = *(u8 * volatile *)p;
    *(volatile u16 *)(p + 0xb6) = result_value;
    frame.state = 0;
    *(volatile u16 *)(p + 0xb8) = 1;
    slot = *(cm_slot_3c volatile *)(vtable + 0x3c);
    result = slot(self, 0xffffffff, (u32)(p + 0xa9), (u32)(p + 0x3a), 0);
    previous = frame.prev;
    *(volatile u32 *)(p + 0xba) = result;
    CM_FS(u32, 0) = previous;
    return 1;
}
