#include "cmre.h"

extern u32 CM_CDECL cm_fn_009446e0(u32 scale, u32 input);

u32 CM_CDECL cm_fn_0093f9a0(u32 input_address, u32 output_address, s32 count,
                            u32 config_address)
{
    s32 mode = *(volatile s16 *)(config_address + 0x10);
    u32 base = *(volatile u32 *)(config_address + 8);
    s16 scale_word = *(volatile s16 *)config_address;

    if (mode == 0) {
        s32 remaining = count;
        s16 shift_word = *(volatile s16 *)(config_address + 2);
        if (remaining <= 0)
            return (config_address & 0xffff0000u) | (u16)shift_word;

        u32 shift = (u32)(s32)shift_word & 31u;
        u32 input = input_address;
        u32 output = output_address;
        do {
            s32 value = *(volatile s16 *)input;
            u32 product = (u32)value * (u32)(s32)scale_word;
            s32 mixed = (s32)(base + product);
            *(volatile s32 *)output = mixed >> shift;
            input += 2u;
            output += 4u;
            remaining -= 1;
        } while (remaining != 0);
        return input;
    }

    if (mode == 1) {
        s32 divisor = *(volatile s32 *)(config_address + 4);
        s32 remaining = count;
        if (remaining <= 0)
            return (u32)remaining;

        u32 input = input_address;
        u32 output = output_address;
        u32 scale = (u32)(s32)scale_word;
        do {
            s32 input_word = *(volatile s16 *)input;
            u32 product_bits = (u32)input_word * scale;
            s32 product = (s32)product_bits;
            s32 result;
            if (product < 0) {
                s32 numerator = (s32)(base - product_bits);
                result = (s32)(0u - (u32)(numerator / divisor));
            } else {
                s32 numerator = (s32)(product_bits + base);
                result = numerator / divisor;
            }
            *(volatile s32 *)output = result;
            input += 2u;
            output += 4u;
            remaining -= 1;
        } while (remaining != 0);
        return 0;
    }

    if (mode == 2) {
        s32 remaining = count;
        u32 scale = *(volatile u32 *)(config_address + 0x0c);
        if (remaining <= 0)
            return config_address;

        u32 input = input_address;
        u32 output = output_address;
        u32 last_result = config_address;
        do {
            u32 input_word = (u32)(s32)*(volatile s16 *)input;
            s32 result = (s32)cm_fn_009446e0(scale, input_word);
            last_result = (u32)result;
            *(volatile s32 *)output = result;
            input += 2u;
            output += 4u;
            remaining -= 1;
        } while (remaining != 0);
        return last_result;
    }
    return config_address;
}
