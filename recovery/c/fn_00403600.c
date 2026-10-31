#include "cmre.h"

void CM_THISCALL cm_fn_00403600(volatile u8 *self, s32 first, s32 second)
{
    u32 delta_a;
    u32 delta_b;
    u32 left;
    u32 right;
    u32 table;
    u32 packed_index;
    u32 scaled_index;
    u32 scan_width;
    u32 scan_count;
    u32 scan_start;
    u32 target;
    u32 numerator;
    u32 denominator;
    s32 limit;
    s32 quotient;
    s32 initial_limit;
    u16 i;
    u8 width;

    delta_a = *(volatile u32 *)(self + 8) - (u32)first;
    *(volatile u32 *)(self + 0xb82) = delta_a;
    left = delta_a - 0x13u;
    delta_b = *(volatile u32 *)(self + 0x0c) - (u32)second;
    *(volatile u32 *)(self + 0xb7a) = left;
    right = *(volatile u32 *)(self + 4) + (u32)second;
    *(volatile u32 *)(self + 0xb86) = delta_b;
    scan_start = delta_b - 0x14u;
    scan_count = right + 0x14u;
    initial_limit = *(volatile s32 *)(self + 0x1fc);
    *(volatile u32 *)(self + 0xb7e) = right;
    *(volatile u32 *)(self + 0xb8a) = scan_count;
    *(volatile u32 *)(self + 0xb8e) = scan_start;

    /* The listing reads the original limit before the three output stores. */
    if ((u32)initial_limit == 0xffffffffu) {
        *(volatile u32 *)(self + 0xb96) = 0;
    } else {
        if (initial_limit < 0) {
            *(volatile u32 *)(self + 0x1fc) = 0;
        }
        scan_count = *(volatile u32 *)(self + 0x14);
        limit = *(volatile s32 *)(self + 0x1fc);
        if ((s32)scan_count < limit) {
            *(volatile u32 *)(self + 0x1fc) = scan_count;
        }
        target = 0;
        i = 0;
        packed_index = (u32)(s32)*(volatile s16 *)(self + 0xb74);
        if ((s32)packed_index > 0) {
            table = *(volatile u32 *)(self + 0x208);
            for (;;) {
                packed_index = (u32)(s32)*(volatile s16 *)(self + 0x212 + i * 2u);
                scaled_index = packed_index * 0x18cu;
                target = *(volatile u32 *)(self + 0x1fc);
                if ((s32)*(volatile u32 *)(table + scaled_index + 0x24) >=
                    (s32)target)
                    break;
                i = (u16)(i + 1u);
                if ((s16)i >= *(volatile s16 *)(self + 0xb74))
                    break;
            }
        }
        target = *(volatile u32 *)(self + 0x1fc);
        *(volatile u16 *)(self + 0xb76) = i;
        *(volatile u32 *)(self + 0xb96) = target;
    }

    scan_count = *(volatile u32 *)(self + 0x14);
    width = *(volatile u8 *)(self + 0xbac);
    scan_count -= (u32)width;
    scan_count += 1u;
    *(volatile u32 *)(self + 0xb92) = scan_count;
    numerator = (scan_start - (right + 0x14u) + 1u) * (u32)width;
    denominator = scan_count + (u32)width;
    quotient = (s32)numerator / (s32)denominator;
    *(volatile u32 *)(self + 0xb9a) = (u32)quotient;
    scan_width = (u32)quotient;
    if (quotient <= 10)
        scan_width = 10;
    *(volatile u32 *)(self + 0xb9a) = scan_width;

    *(volatile u32 *)(self + 0xba6) = 0;
    *(volatile u8 *)(self + 0xbaa) = 0;
    *(volatile u32 *)(self + 0xbed) = 1;
    numerator = (scan_start - (right + 0x14u) - scan_width + 1u) *
                *(volatile u32 *)(self + 0xb96);
    quotient = (s32)numerator / (s32)scan_count;
    target = (u32)quotient + right + 0x14u;
    *(volatile u32 *)(self + 0xb9e) = target;
    *(volatile u32 *)(self + 0xba2) = target + scan_width - 1u;
    CM_GLOBAL(volatile u32, 0x00ad9c58) = (u32)(self + 0xb7a);
}
