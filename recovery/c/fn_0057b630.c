#include "cmre.h"

extern short *CM_THISCALL cm_fn_00549f70(short *self, short arg1, char arg2,
                                         u16 arg3, u32 arg4);
typedef u32 (CM_THISCALL *cm_slot_3c)(void *self, u32 arg1, void *arg2,
                                      void *arg3, u32 arg4);

u32 CM_FASTCALL cm_fn_0057b630(void *self)
{
    volatile u8 *object = (volatile u8 *)self;
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_area[16];
    u16 early_40;
    u16 helper_two_result;
    u32 vtable;
    u32 slot_result;
    u32 restore_prev;
    cm_slot_3c slot;

    frame.state = 0xffffffffu;
    frame.handler = 0x009587d0u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    *(volatile u8 *)(object + 0x43) = 2;
    *(volatile u8 *)(object + 0xc7) = 2;
    *(volatile u8 *)(object + 0xbe) = 2;
    early_40 = *(volatile u16 *)(object + 0x40);
    *(volatile u8 *)(object + 0xc2) = 3;
    *(volatile u8 *)(object + 0x4a) = 3;
    *(volatile u16 *)(object + 0x3c) = 1;
    *(volatile u8 *)(object + 0xc3) = 1;
    *(volatile u8 *)(object + 0xc4) = 0x0a;
    *(volatile u8 *)(object + 0xc5) = 4;
    *(volatile u8 *)(object + 0xc6) = 1;
    *(volatile u8 *)(object + 0xbf) = 0;
    *(volatile u8 *)(object + 0xc0) = 0;
    *(volatile u8 *)(object + 0xc1) = 0;
    *(volatile u16 *)(object + 0x3a) = 1000;
    *(volatile u8 *)(object + 0x49) = 7;

    cm_fn_00549f70((short *)(local_area + 0), 10, 0, early_40, 5);
    early_40 = *(volatile u16 *)(object + 0x40);
    helper_two_result = *(volatile u16 *)(local_area + 0);
    *(volatile u16 *)(object + 0x13e) = helper_two_result;
    frame.state = 0;
    *(volatile u16 *)(object + 0x140) = 0;
    cm_fn_00549f70((short *)(local_area + 8), 10, 1, early_40, 5);

    helper_two_result = *(volatile u16 *)(local_area + 8);
    vtable = *(volatile u32 *)object;
    *(volatile u16 *)(object + 0xb6) = helper_two_result;

    *(volatile u8 *)&frame.state = 1;
    *(volatile u16 *)(object + 0xb8) = 0;
    slot = *(cm_slot_3c volatile *)(vtable + 0x3c);
    slot_result = slot(self, 0xffffffffu, (void *)(object + 0xa9),
                       (void *)(object + 0x3a), 0);
    restore_prev = frame.prev;
    *(volatile u32 *)(object + 0xba) = slot_result;
    CM_FS(u32, 0) = restore_prev;
    return 1;
}
