#include "cmre.h"

extern short *CM_THISCALL cm_fn_0054c8e0(
    void *self, u32 *output, s32 code);

void CM_THISCALL cm_fn_0058dfc0(volatile u8 *self)
{
    u32 object;
    u32 offset;
    u32 field_list;
    u32 term;
    u32 term2;
    u32 total;
    u32 weighted;
    u32 value;
    s32 count;
    s32 outer_index;
    u8 inner_index;
    s32 first_sum;
    u32 sum_bits;
    s32 quotient;
    short *date;
    u16 returned_high;
    u16 returned_low;
    u16 global_high;
    u16 global_low;
    int date_later;
    volatile u8 date_local[0x1c];
    volatile u8 *item;
    volatile u8 *base;

    count = CM_GLOBAL(volatile s32, 0x00ae2350);
    if (count <= 0)
        return;
    offset = 0;
    outer_index = 0;
    do {
        object = *(volatile u32 *)self;
        item = (volatile u8 *)(object + offset);
        count = *(volatile s16 *)(item + 0x0a);
        first_sum = 0;
        sum_bits = 0;
        inner_index = 0;
        if (count > 0) {
            while ((s8)inner_index < 8 && (s8)inner_index < count) {
                field_list = *(volatile u32 *)(item + 4);
                first_sum = (s32)((u32)first_sum +
                    (u32)(s32)*(volatile s16 *)(field_list +
                                              (u32)((s32)(s8)inner_index * 5)));
                inner_index = (u8)(inner_index + 1u);
            }
            sum_bits = (u32)first_sum;
            while ((s8)inner_index < count) {
                field_list = *(volatile u32 *)(item + 4);
                sum_bits += (u32)(s32)*(volatile s16 *)(field_list +
                                             (u32)((s32)(s8)inner_index * 5));
                inner_index = (u8)(inner_index + 1u);
            }
        }
        if (count > 8) {
            quotient = (s32)((s32)(sum_bits * 8u) / count);
            quotient = (s32)((u32)quotient + (u32)first_sum);
            *(volatile s32 *)(item + 0x14) = quotient / 2;
        } else {
            *(volatile s32 *)(item + 0x14) = first_sum;
        }

        term = CM_GLOBAL(volatile u32, 0x00dd6ec8);
        term2 = CM_GLOBAL(volatile u32, 0x00dd6ecc);
        *(volatile u32 *)(date_local + 0) = term;
        *(volatile u32 *)(date_local + 4) = term2;
        date = cm_fn_0054c8e0((short *)0x00ae2c90,
                              (u32 *)(date_local + 8), 0x16d);
        returned_high = *(volatile u16 *)((volatile u8 *)date + 2);
        global_high = *(volatile u16 *)(date_local + 2);
        date_later = global_high > returned_high;
        if (global_high == returned_high) {
            global_low = (u16)(s16)term;
            returned_low = *(volatile s16 *)date;
            date_later = (s16)global_low > (s16)returned_low;
        }
        if (date_later) {
            base = (volatile u8 *)*(volatile u32 *)self;
            item = base + offset;
            count = *(volatile s16 *)(item + 0x0a);
            if (count < 9) {
                count = *(volatile s16 *)(item + 0x0a);
                if (count == 0) {
                    *(volatile u32 *)(item + 0x14) = *(volatile u32 *)(item + 0x18);
                } else {
                    quotient = (s32)((u32)*(volatile s32 *)(item + 0x18) +
                                     (u32)first_sum) / (count + 8);
                    *(volatile u32 *)(item + 0x14) = (u32)quotient << 3;
                }
            }
        }

        base = (volatile u8 *)*(volatile u32 *)self;
        item = base + offset;
        term = (u32)((s32)(*(volatile u32 *)(item + 0x24) * 2u) / 6);
        total = term;
        term2 = (u32)((s32)(*(volatile u32 *)(item + 0x20) * 3u) / 6);
        total += term2;
        weighted = *(volatile u32 *)(item + 0x1c) * 4u;
        total += (u32)((s32)weighted / 6);
        value = *(volatile u32 *)(item + 0x18) * 5u;
        total += (u32)((s32)value / 6);
        total += (u32)((s32)*(volatile u32 *)(item + 0x28) / 6);
        total += *(volatile u32 *)(item + 0x14);
        {
            volatile double factor = CM_GLOBAL(volatile double, 0x0096a350);
            *(volatile float *)(item + 0x0c) = (float)((double)(s32)total * factor);
        }

        offset += 0x2cu;
        outer_index = (s32)((u32)outer_index + 1u);
    } while (outer_index < CM_GLOBAL(volatile s32, 0x00ae2350));
}
