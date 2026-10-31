#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00685d30(u8 *self, u32 first, u32 second);
extern void CM_THISCALL cm_fn_005f6ad0(u8 *self, u8 argument);

typedef void (CM_THISCALL *cm_fn_005f65b0_slot)(u8 *self, u32 first, u32 second);

u32 CM_THISCALL cm_fn_005f65b0(u8 *self, u32 first_argument, u32 second_argument)
{
    u8 *child = (u8 *)first_argument;
    u32 category;
    s16 value;
    s8 child_state;
    s32 current_state;
    u32 *rows;
    u8 *row;
    u32 vtable;
    u32 target;
    s32 index;

    (void)cm_fn_00685d30(self, first_argument, second_argument);
    child_state = *(volatile s8 *)(child + 0x42);
    if (child_state != -1) {
        return 1;
    }

    value = *(volatile s16 *)(child + 0x34);
    if (value < 8)
        category = 0;
    else if (value < 0x10)
        category = 1;
    else if (value < 0x1a)
        category = 2;
    else
        category = 3;
    *(volatile u8 *)(child + 0x42) = (u8)category;

    child_state = *(volatile s8 *)(child + 0x42);
    current_state = *(volatile s32 *)(self + 0x30);
    while (current_state < (s32)child_state) {
        u8 current_byte = *(volatile u8 *)(self + 0x30);
        u8 next_state = (u8)(current_byte + 1u);
        cm_fn_005f6ad0(self, next_state);
        child_state = *(volatile s8 *)(child + 0x42);
        current_state = *(volatile s32 *)(self + 0x30);
    }

    rows = (u32 *)*(volatile u32 *)(self + 0x0c);
    {
        s32 saved_value = *(volatile s32 *)(self + 8);
        *(volatile s32 *)(self + 8) = 0;
        index = *(volatile s8 *)(child + 0x42);
        row = (u8 *)*(volatile u32 *)(rows + index);
        vtable = *(volatile u32 *)row;
        target = *(volatile u32 *)(vtable + 4u);
        ((cm_fn_005f65b0_slot)target)(row, first_argument, second_argument);
        *(volatile s32 *)(self + 8) = saved_value;
    }
    *(volatile u8 *)(child + 0x42) = 0xff;
    return 1;
}
