#include "cmre.h"

/* WIP: caller-visible EAX mismatch, case 0 */

u32 CM_THISCALL cm_fn_00403200(void *self, s16 index)
{
    volatile u8 *base = (volatile u8 *)self;
    s16 count;
    u32 table;
    u32 minimum;
    u32 item_offset;
    u32 item_value;

    if (index == -1)
        return (u32)(s32)index;
    count = *(volatile s16 *)(base + 0xb74);
    if (count >= 0x4b0)
        return (u32)(s32)count;
    if ((*(volatile u32 *)(base + 0x18) & 0x80000u) != 0)
        return (u32)(s32)count;

    item_offset = (u32)(s32)index * 0x18cu;
    table = *(volatile u32 *)(base + 0x208);
    minimum = *(volatile u32 *)(base + 0x10);
    *(volatile u16 *)(base + 0xb74) = (u16)(count + 1);
    item_value = *(volatile u32 *)(u8 *)(table + item_offset + 0x24);
    if ((s32)item_value < (s32)minimum)
        *(volatile u32 *)(base + 0x10) = item_value;

    item_value = *(volatile u32 *)(u8 *)(table + item_offset + 0x24);
    {
        u32 maximum = *(volatile u32 *)(base + 0x14);
        if ((s32)item_value > (s32)maximum)
            *(volatile u32 *)(base + 0x14) = item_value;
    }

    {
        s16 position = (s16)((u16)(count + 1) - 2);
        if (position < 0) {
            *(volatile s16 *)(base + 0x212) = index;
            return item_value;
        }
        for (;;) {
            s16 previous;
            s32 previous_index;
            u32 previous_value;
            u32 current_table;
            s16 old_position = position;

            current_table = *(volatile u32 *)(base + 0x208);
            previous = *(volatile s16 *)(base + 0x212 + (u32)(s32)old_position * 2u);
            previous_index = (s32)previous;
            previous_value = *(volatile u32 *)(u8 *)(current_table +
                (u32)previous_index * 0x18cu + 0x24u);
            item_value = *(volatile u32 *)(u8 *)(current_table + item_offset + 0x24u);
            if ((s32)item_value >= (s32)previous_value) {
                *(volatile s16 *)(base + 0x214 + (u32)(s32)old_position * 2u) = index;
                return ((u32)previous_index * 0x18cu & 0xffff0000u) | (u16)index;
            }
            position = (s16)((u16)position - 1u);
            *(volatile s16 *)(base + 0x214 + (u32)(s32)old_position * 2u) = previous;
            if (position < 0)
                break;
        }
    }
    *(volatile s16 *)(base + 0x212) = index;
    return item_value;
}
