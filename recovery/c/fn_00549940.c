#include "cmre.h"

extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 item);

u8 CM_CDECL cm_fn_00549940(volatile u8 *self)
{
    u8 accepted = 0;
    u16 date_bits;
    s32 date;
    long long first_product;
    s32 first_high;
    s32 first_quotient;
    s16 first_limit;
    long long second_product;
    s32 second_high;
    s32 second_quotient;
    s16 second_limit;
    u32 cursor;
    u32 remaining = 0x32u;

    if (self == 0) {
        return 0;
    }

    date_bits = *(volatile u16 *)(self + 0x80u);
    date = (s16)date_bits;
    first_product = (long long)date * (long long)0x1b4e81b5;
    first_high = (s32)(first_product >> 32);
    first_quotient = first_high >> 3;
    first_limit = (s16)(first_quotient + ((u32)first_quotient >> 31) + 0x19);
    if (first_limit < 0x28) {
        first_limit = 0x28;
    } else if (first_limit > 0x87) {
        first_limit = 0x87;
    }

    second_product = (long long)date * (long long)0x10624dd3;
    second_high = (s32)(second_product >> 32);
    second_quotient = second_high >> 5;
    second_limit = (s16)(second_quotient + ((u32)second_quotient >> 31) + 5);
    if (second_limit > 0x0e) {
        second_limit = 0x0e;
    }

    cursor = (u32)self + 0xd7u;
    do {
        u32 item = *(volatile u32 *)cursor;
        if (item != 0) {
            u32 entry = *(volatile u32 *)(item + 0x61u);
            if (entry != 0) {
                u8 slot14 = *(volatile u8 *)(entry + 0x14u);
                u8 eligible = slot14 == 0x14u;
                if (!eligible) {
                    u8 slot12 = *(volatile u8 *)(entry + 0x12u);
                    eligible = slot12 == 0x14u;
                    if (!eligible) {
                        u8 slot13 = *(volatile u8 *)(entry + 0x13u);
                        eligible = slot13 == 0x14u;
                    }
                }
                if (eligible) {
                    u32 manager = CM_GLOBAL(volatile u32, 0x00b63d28u);
                    u8 status = cm_fn_0062c400(manager, item);
                    if (status != 2u) {
                        s32 item_key = *(volatile s32 *)item;
                        s32 global_key = CM_GLOBAL(volatile s32, 0x00ae2364u);
                        if (item_key < global_key) {
                            entry = *(volatile u32 *)(item + 0x61u);
                            if (*(volatile s16 *)(entry + 5u) > first_limit &&
                                (s32)*(volatile s8 *)(entry + 0x43u) > second_limit &&
                                (s32)*(volatile s8 *)(entry + 0x37u) > second_limit) {
                                accepted = (u8)(accepted + 1u);
                            }
                        }
                    }
                }
            }
        }
        cursor += 4u;
        --remaining;
    } while (remaining != 0);

    return (s8)accepted > 1 ? 1u : 0u;
}
