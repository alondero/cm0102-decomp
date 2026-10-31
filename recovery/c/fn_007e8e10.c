#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(void *self, void *local, u32 global);

u32 CM_THISCALL cm_fn_007e8e10(volatile u8 *input, volatile u8 *out,
                               volatile u8 *mode_data, u32 use_mode)
{
    s8 selector_byte = *(volatile s8 *)(mode_data + 0x2c);
    s8 selector;
    if (selector_byte == 0) selector = *(volatile s8 *)(mode_data + 0x2e);
    else if (selector_byte == 1) selector = *(volatile s8 *)(mode_data + 0x2f);
    else return 0;
    if (selector < 9 || selector > 12) return 0;
    u32 table = use_mode != 0 ? 0x007e9028u : 0x007e9038u;
    u32 target = CM_GLOBAL(volatile u32, table + (u32)(selector - 9) * 4u);
    (void)target;

    s32 high = -1;
    s16 low = -1;
    s8 value;
    s32 n;
    if (use_mode != 0) {
        value = *(volatile s8 *)(input + 6);
        switch (selector) {
        case 9:
            if (value < 3) return 0;
            high = value < 8 ? 1 : (value >= 15) + 2;
            break;
        case 10:
            if (value < 3) return 0;
            if (value < 5) high = 1;
            else if (value < 9) high = 2;
            else if (value < 13) high = 3;
            else high = (value >= 17) + 4;
            break;
        case 11:
            n = (s32)value * 8;
            high = n / 20 + 4;
            break;
        case 12:
            n = (s32)value * 28;
            high = n / 20 + 12;
            break;
        default:
            return 0;
        }
    } else {
        value = *(volatile s8 *)(input + 6);
        switch (selector) {
        case 9:
            if (value < 3) return 0;
            if (value < 8) low = 7;
            else low = value < 15 ? 14 : 21;
            break;
        case 10:
            if (value < 3) return 0;
            if (value < 5) low = 7;
            else if (value < 9) low = 14;
            else if (value < 13) low = 21;
            else low = value < 17 ? 28 : 35;
            break;
        case 11:
            n = (s32)value * 8;
            low = (s16)((n / 20 + 4) * 7);
            break;
        case 12:
            n = (s32)value * 28;
            low = (s16)((n / 20 + 12) * 7);
            break;
        default:
            return 0;
        }
    }
    volatile u8 local[8];
    *(volatile u8 *)out = (u8)high;
    *(volatile u8 *)(out + 1) = 0;
    *(volatile u16 *)(out + 2) = (u16)low;
    cm_fn_00417c10((void *)(out + 4), (void *)local,
                   0x00ae2c90);
    *(volatile u8 *)(out + 0x10) = 3;
    u32 id = *(volatile u32 *)mode_data;
    *(volatile u32 *)(out + 0x1c) = id;
    u8 flag = *(volatile u8 *)(mode_data + 0x48);
    *(volatile u8 *)(out + 0x6f) = (u8)(flag == 0);
    return 1;
}
