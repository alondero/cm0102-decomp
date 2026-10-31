#include "cmre.h"

extern short *CM_THISCALL cm_fn_00549f70(short *self, short arg1, char arg2,
                                         u16 arg3, u32 arg4);
typedef u32 (CM_THISCALL *cm_slot_3c)(void *self, u32 arg1, void *arg2,
                                      void *arg3, u32 arg4);

u32 CM_FASTCALL cm_fn_00921040(void *self)
{
    volatile u8 *object = (volatile u8 *)self;
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_area[8];
    u32 entry_prev;
    u32 table;
    u32 index;
    u32 item;
    u8 factor;
    u16 field_40;
    u16 helper_result;
    u32 vtable;
    u32 result;
    u32 restore_prev;
    cm_slot_3c slot;

    entry_prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00965be8u;
    frame.prev = entry_prev;
    table = CM_GLOBAL(volatile u32, 0x00adadfc);
    CM_FS(u32, 0) = (u32)&frame;

    if (table != 0) {
        index = CM_GLOBAL(volatile u32, 0x009cf76c);
        item = *(volatile u32 *)(table + index * 4u);
        if (item != 0)
            *(volatile u8 *)(object + 0xef) = *(volatile u8 *)(item + 0xee);
    }

    factor = *(volatile u8 *)(object + 0xef);
    *(volatile u16 *)(object + 0x3c) = 2;
    *(volatile u8 *)(object + 0x43) = 2;
    *(volatile u8 *)(object + 0xc6) = 2;
    factor = (u8)(4u - factor);
    item = *(volatile u8 *)(object + 0xee);
    factor = (u8)(factor - item);
    field_40 = *(volatile u16 *)(object + 0x40);
    *(volatile u8 *)(object + 0xbe) = factor;
    *(volatile u8 *)(object + 0xc2) = 3;
    *(volatile u8 *)(object + 0xc3) = 1;
    *(volatile u8 *)(object + 0xc4) = 0x0a;
    *(volatile u16 *)(object + 0xab) = 0;
    *(volatile u16 *)(object + 0xdb) = 0;
    *(volatile u8 *)(object + 0xc5) = 1;
    *(volatile u8 *)(object + 0xc7) = 3;
    *(volatile u8 *)(object + 0xbf) = 1;
    *(volatile u8 *)(object + 0xc0) = 0;
    *(volatile u8 *)(object + 0xc1) = 0;
    *(volatile u8 *)(object + 0x49) = 7;
    *(volatile u8 *)(object + 0x4a) = 3;
    *(volatile u16 *)(object + 0xa7) = 0xffff;

    cm_fn_00549f70((short *)local_area, 0x19, 0, field_40, 5);
    helper_result = *(volatile u16 *)local_area;
    vtable = *(volatile u32 *)object;
    *(volatile u16 *)(object + 0xb6) = helper_result;

    frame.state = 0;
    *(volatile u16 *)(object + 0xb8) = 0;
    slot = *(cm_slot_3c volatile *)(vtable + 0x3c);
    result = slot(self, 0xffffffffu, (void *)(object + 0xa9),
                  (void *)(object + 0x3a), 0);
    restore_prev = frame.prev;
    *(volatile u32 *)(object + 0xba) = result;
    CM_FS(u32, 0) = restore_prev;
    return 1;
}
