#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00896c60(u32 self, u32 record, u32 value,
                                      u32 mode_a, u32 mode_b);

void CM_THISCALL cm_fn_0090d860(u8 *self, volatile u32 *record,
                                  u32 unused_argument)
{
    (void)unused_argument;
    u32 index = (u32)(s32)*(volatile s16 *)0x00b77c7au;
    u32 table_address = (u32)(index * 0x300u + 0x00b74c26u);
    u32 table_value = *(volatile u32 *)(u32)table_address;
    s32 field_index;
    if (table_value == 0) {
        field_index = 0x10;
    } else {
        u16 current = CM_GLOBAL(volatile u16, 0x00ae2364u);
        u16 entry = *(volatile u16 *)(u32)table_value;
        field_index = (s16)(u16)(current - entry - 1u);
    }
    u32 field_address = (u32)self + (u32)field_index * 4u + 0x18u;
    *(volatile u32 *)(u32)field_address = 0;

    volatile u32 *cursor = (volatile u32 *)((u32)record + 0xd7u);
    u32 remaining = 0x32u;
    while (remaining != 0) {
        u32 pointer = *cursor;
        if (pointer != 0) {
            u32 pointed_value = *(volatile u32 *)(u32)pointer;
            u32 current_value = CM_GLOBAL(volatile u32, 0x00ae2364u);
            if ((s32)pointed_value >= (s32)current_value) {
                u32 receiver = CM_GLOBAL(volatile u32, 0x00de1d08u);
                (void)cm_fn_00896c60(receiver, (u32)record, pointer, 1, 1);

                pointer = *cursor;
                receiver = CM_GLOBAL(volatile u32, 0x00de1d08u);
                (void)cm_fn_00896c60(receiver, (u32)record, pointer, 0, 1);
                *cursor = 0;
            }
        }
        ++cursor;
        --remaining;
    }
}
