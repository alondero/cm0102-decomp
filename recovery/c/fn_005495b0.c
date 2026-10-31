#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0062c400(u32 self, u32 row);

u8 CM_CDECL cm_fn_005495b0(u32 data)
{
    u8 qualifying = 0u;
    s16 input;
    long long product;
    s32 high;
    s32 first_limit;
    s32 second_limit;
    u32 list;
    u32 remaining;

    if (data == 0u)
        return 0u;

    input = *(volatile s16 *)(data + 0x80u);
    product = (long long)input * 0x1b4e81b5ll;
    high = (s32)((unsigned long long)product >> 32);
    first_limit = (high >> 3) + (s32)((u32)high >> 31) + 0x19;
    if (first_limit < 0x28)
        first_limit = 0x28;
    else if (first_limit > 0x87)
        first_limit = 0x87;

    product = (long long)input * 0x10624dd3ll;
    high = (s32)((unsigned long long)product >> 32);
    second_limit = (high >> 5) + (s32)((u32)high >> 31) + 5;
    if (second_limit > 0xd)
        second_limit = 0xd;

    list = data + 0xd7u;
    remaining = 0x32u;
    do {
        u32 row = *(volatile u32 *)list;
        if (row != 0u) {
            u32 child = *(volatile u32 *)(row + 0x61u);
            if (child != 0u && *(volatile u8 *)(child + 0x11u) == 0x14u) {
                u32 manager = CM_GLOBAL(volatile u32, 0x00b63d28u);
                u32 result = cm_fn_0062c400(manager, row);
                if ((u8)result != 2u) {
                    s32 row_time = *(volatile s32 *)row;
                    s32 global_time = CM_GLOBAL(volatile s32, 0x00ae2364u);
                    if (row_time < global_time) {
                        child = *(volatile u32 *)(row + 0x61u);
                        if (*(volatile s16 *)(child + 5u) > (s16)first_limit) {
                            if (*(volatile s8 *)(child + 0x2eu) > (s8)second_limit)
                                ++qualifying;
                        }
                    }
                }
            }
        }
        list += 4u;
        --remaining;
    } while (remaining != 0u);

    return qualifying <= 1u;
}
