#include "cmre.h"

/* WIP after two failed invocations (one build, one semantic). O0/O2 coverage
   is 0.8696 with 22 inconclusive cases; cases 1, 13 and 15 first mismatch in
   the 94fb44/9525f9 formatting path. The listing proves the explicit 8-byte
   double input, but no caller-side stack-map defect is established. */

extern u8 *CM_CDECL cm_fn_0094fb44(double value);
extern void CM_CDECL cm_fn_0094facd(u32 output, s32 length, u32 info);
extern void CM_CDECL cm_fn_0094ab27(u32 output, s32 length);
extern unsigned long long CM_CDECL cm_fn_0094a640(u32 destination,
                                                  u32 value, u32 count);

u8 *CM_CDECL cm_fn_0094a913(const double *value, u8 *output,
                            s32 requested_count)
{
    volatile u8 *global_flag = (volatile u8 *)0x00de3028;
    volatile u32 *global_count = (volatile u32 *)0x00de302c;
    volatile u32 *info;
    volatile s32 *count_from_stack = &requested_count;
    u32 info_address;
    u32 output_address = (u32)output;
    u32 configured_count;
    u32 cursor;
    u32 count_bits;
    s32 count;
    s32 length;
    s32 remaining;
    u32 index;

    if (*global_flag != 0) {
        configured_count = *global_count;
        count = *count_from_stack;
        info_address = CM_GLOBAL(volatile u32, 0x00de3024);
        info = (volatile u32 *)info_address;
        if (configured_count == (u32)count) {
            index = *info == 0x2d ? 1u : 0u;
            index += configured_count;
            index += output_address;
            cursor = index;
            *(volatile u8 *)cursor = 0x30;
            cursor += 1u;
            *(volatile u8 *)cursor &= 0;
        }
    } else {
        info_address = (u32)cm_fn_0094fb44(*value);
        info = (volatile u32 *)info_address;
        count = *count_from_stack;
        cursor = info_address + 4u;
        length = (s32)*(volatile u32 *)cursor;
        cursor = info_address;
        index = *(volatile u32 *)cursor == 0x2d ? 1u : 0u;
        cm_fn_0094facd(output_address + index,
                       (s32)((u32)length + (u32)count), info_address);
    }

    if (*info == 0x2d) {
        *(volatile u8 *)output_address = 0x2d;
        cursor = output_address + 1u;
    } else {
        cursor = output_address;
    }

    length = (s32)info[1];
    if (length <= 0) {
        cm_fn_0094ab27(cursor, 1);
        *(volatile u8 *)cursor = 0x30;
        cursor += 1u;
    } else {
        cursor += (u32)length;
    }

    if (count > 0) {
        cm_fn_0094ab27(cursor, 1);
        *(volatile u8 *)cursor = CM_GLOBAL(volatile u8, 0x00ad91e0);
        cursor += 1u;
        length = (s32)info[1];
        if (length < 0) {
            if (*global_flag != 0) {
                remaining = (s32)(0u - (u32)length);
                count = remaining;
            } else {
                remaining = (s32)(0u - (u32)length);
                if (count >= remaining)
                    count = remaining;
            }
            count_bits = (u32)count;
            cm_fn_0094ab27(cursor, (s32)count_bits);
            (void)cm_fn_0094a640(cursor, 0x30, count_bits);
        }
    }
    return output;
}
