/* Preferred-image VA 0x0058d460, image base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Private proposal only. Original callees remain native boundaries.
   Qsort below has original CDECL4 ABI, not accepted C's regparm(2) ABI.
   Scratch L is the original ESP immediately after SUB ESP,18: month L3,
   reverse offset L4, iterations L8, outer index Lc, copied date L10..17.
   count_out aliases the initialized incoming argument slot, not fresh scratch. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u16 CM_THISCALL cm_fn_0054cf70(u32 date);
extern u8 CM_THISCALL cm_fn_0054cfb0(u32 date);
extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 output, u32 input);
extern void CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 width, u32 compare);
extern void CM_THISCALL cm_fn_0058dfc0(u32 self);
extern u32 CM_THISCALL cm_fn_0058d610(u32 self, u32 count_out);
extern void CM_CDECL cm_fn_009452ca(u32 allocation);

void CM_THISCALL cm_fn_0058d460(u32 self, volatile u32 date_arg)
{
    u8 scratch[0x18];
    u32 l = (u32)scratch, date = date_arg;
    u32 count, offset = 0, base, record, list, destination, source;
    u32 value, year, current_year, inner_count, outer_index;
    u32 position, shift_count, array, cursor, index;
    u8 low, month;
    if (cm_fn_0054cf70(date) != 0x12u) return;
    B(l + 3u) = cm_fn_0054cfb0(date);
    cm_fn_00417c10(self + 5u, l + 0x10u, date);
    count = D(0x00ae2350);
    D(l + 0xcu) = 0;
    if ((s32)count > 0) {
        do {
            base = D(self);
            low = (u8)(B(base + offset + 0xau) - 1u);
            if ((s8)low >= 0) {
                value = (u32)(s32)(s8)low;
                D(l + 4u) = value * 5u;
                D(l + 8u) = value + 1u;
                do {
                    base = D(self);
                    value = D(l + 4u);
                    list = D(base + offset + 4u);
                    record = base + offset;
                    destination = value + list;
                    year = (u32)W(destination + 3u) + 1u;
                    D(l + 0x10u) = year;
                    current_year = W(date + 2u);
                    year = D(l + 0x10u);
                    if ((s32)year < (s32)current_year) goto remove_item;
                    if (year != current_year) goto next_item;
                    month = B(l + 3u);
                    low = B(destination + 2u);
                    if (low >= month) goto next_item;
remove_item:
                    value = (u32)(s32)(s16)W(record + 0xau);
                    source = list + value * 5u - 5u;
                    value = D(source);
                    D(destination) = value;
                    low = B(source + 4u);
                    B(destination + 4u) = low;
                    base = D(self);
                    W(base + offset + 0xau) = (u16)(W(base + offset + 0xau) - 1u);
next_item:
                    value = D(l + 4u);
                    inner_count = D(l + 8u);
                    date = date_arg;
                    D(l + 4u) = value - 5u;
                    --inner_count;
                    D(l + 8u) = inner_count;
                } while (inner_count != 0);
            }
            base = D(self);
            count = (u32)(s32)(s16)W(base + offset + 0xau);
            record = base + offset;
            list = D(record + 4u);
            cm_fn_00945333(list, count, 5, 0x0058e220);
            outer_index = D(l + 0xcu);
            count = D(0x00ae2350);
            ++outer_index;
            offset += 0x2cu;
            D(l + 0xcu) = outer_index;
        } while ((s32)outer_index < (s32)count);
    }
    month = B(l + 3u);
    low = B(self + 4u);
    if (month == low) {
        year = W(date + 2u);
        current_year = W(0x00dd6eca);
        if (year != current_year && (s32)count > 0) {
            index = 0;
            offset = 0;
            do {
                position = offset + 0x28u;
                shift_count = 5;
                do {
                    base = D(self);
                    position -= 4u;
                    --shift_count;
                    value = D(position + base);
                    D(position + base + 4u) = value;
                } while (shift_count != 0);
                base = D(self);
                ++index;
                D(offset + base + 0x14u) = 0;
                count = D(0x00ae2350);
                offset += 0x2cu;
            } while ((s32)index < (s32)count);
        }
    }
    cm_fn_0058dfc0(self);
    array = cm_fn_0058d610(self, (u32)&date_arg);
    count = date_arg;
    index = 0;
    if ((s32)count > 0) {
        cursor = array;
        do {
            record = D(cursor);
            cursor += 4u;
            year = W(record + 0x10u);
            W(record + 0x12u) = (u16)year;
            record = D(cursor - 4u);
            value = index + 1u;
            ++index;
            W(record + 0x10u) = (u16)value;
            count = date_arg;
        } while ((s32)index < (s32)count);
    }
    cm_fn_009452ca(array);
}
