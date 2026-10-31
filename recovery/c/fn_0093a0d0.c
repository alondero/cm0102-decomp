#include "cmre.h"

extern u32 CM_CDECL cm_fn_009446e0(u32 value, u32 amount);
extern u32 CM_CDECL cm_fn_009447e0(u32 address, u16 amount);

void CM_CDECL cm_fn_0093a0d0(u8 *context)
{
    u32 object = *(volatile u32 *)(context + 0x28u);
    u8 mode = *(volatile u8 *)(object + 0xb6u);
    u16 value = *(volatile u16 *)(object + 0x0cu);
    u32 result = value;

    if (mode == 0) {
        s16 high = *(volatile s16 *)(context + 0x16u);
        s16 low = *(volatile s16 *)(context + 0x14u);
        u32 amount;
        if (high == 0) {
            amount = *(volatile u32 *)(object + 0xb8u);
        } else {
            if (low == 0) {
                amount = *(volatile u32 *)(object + 0xbcu);
            } else {
                s32 high_square = (s32)high * (s32)high;
                u32 high_scale = (u32)(high_square + 0x2000) >> 14;
                u32 high_base = *(volatile u32 *)(object + 0xbcu);
                u32 first = cm_fn_009447e0(high_base, (u16)high_scale);

                u32 low_base = *(volatile u32 *)(object + 0xb8u);
                s32 low_square = (s32)low * (s32)low;
                u32 low_scale = (u32)(low_square + 0x2000) >> 14;
                u32 second = cm_fn_009447e0(low_base, (u16)low_scale);
                amount = first + second;
            }
        }
        result = cm_fn_009446e0((u32)value, amount);
    }

    u32 output = *(volatile u32 *)(context + 0x20u);
    *(volatile u32 *)output = result & 0xffffu;
    output = *(volatile u32 *)(context + 0x20u);
    *(volatile u32 *)(context + 0x20u) = output + 4u;
}
