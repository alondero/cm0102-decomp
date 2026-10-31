#include "cmre.h"

extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 index);

void CM_THISCALL cm_fn_00580040(volatile u8 *self)
{
    s32 outer;
    s32 inner;
    s32 item_index;
    s32 item_count;
    s32 type_index;
    s32 type_count;
    s32 item;
    s32 type;
    s32 category;
    s32 type_entry;
    s32 type_offset;
    volatile u32 * volatile table;
    u32 list_entry;
    u32 returned;
    u32 returned_value;
    s8 outer_limit;
    s8 initial_limit;

    if (*(volatile u32 *)(self + 0xfb) != 0)
        return;
    initial_limit = *(volatile s8 *)(self + 0xff);
    *(volatile u32 *)(self + 0xfb) = 1;
    if (initial_limit < -1)
        return;

    outer = -1;
    for (;;) {
        if ((s8)outer == -1) {
            item = (s32)(u32)self;
        } else {
            table = *(volatile u32 * volatile *)(self + 0x0c);
            item = (s32)table[outer];
        }
        type_index = (s8)*(volatile s8 *)(self + 0x101);
        table = *(volatile u32 * volatile *)(self + 0x0c);
        type = (s32)table[type_index];
        type_count = *(volatile s16 *)((volatile u8 *)type + 0x3e);
        if (type_count > 0) {
            type_offset = 0;
            for (inner = 0; inner < type_count; ) {
                type_entry = (s32)(*(volatile u32 *)((volatile u8 *)type + 0xa7) +
                                   (u32)type_offset);
                category = *(volatile u8 *)((volatile u8 *)type_entry + 5);
                if (category == 1) {
                    item_count = *(volatile s16 *)((volatile u8 *)item + 0x3e);
                    if (item_count > 0) {
                        item_index = 0;
                        while (item_index < item_count) {
                            returned = cm_fn_006826d0((void *)item, (u32)item_index);
                            returned_value = *(volatile u32 *)returned;
                            list_entry = *(volatile u32 *)((volatile u8 *)type_entry);
                            if (returned_value == list_entry) {
                                returned = cm_fn_006826d0((void *)item,
                                                          (u32)item_index);
                                *(volatile u8 *)(returned + 0x37) = 7;
                                break;
                            }
                            item_count = *(volatile s16 *)((volatile u8 *)item + 0x3e);
                            ++item_index;
                        }
                    }
                }
                type_index = (s8)*(volatile s8 *)(self + 0x101);
                table = *(volatile u32 * volatile *)(self + 0x0c);
                type = (s32)table[type_index];
                type_count = *(volatile s16 *)((volatile u8 *)type + 0x3e);
                type_offset += 6;
                ++inner;
            }
        }
        outer_limit = *(volatile s8 *)(self + 0xff);
        ++outer;
        if (outer > outer_limit)
            break;
    }
}
