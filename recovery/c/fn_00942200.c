#include "cmre.h"

extern u32 cm_fn_00944840(u32 param_1, u32 param_2);

void
cm_fn_00942200(s32 param_1, u32 param_2, volatile u16 *param_3)
{
    u32 shifted_first = (u32)param_1 << 6;
    u32 shifted_second = param_2;
    u32 result;
    u32 bit_count;
    u32 shifted_bit;

    while (((shifted_first | shifted_second) & 1u) == 0) {
        shifted_first = (u32)((s32)shifted_first >> 1);
        shifted_second = (u32)((s32)shifted_second >> 1);
    }
    param_3[8] = 2;
    result = cm_fn_00944840(shifted_first, shifted_second);
    *(volatile u32 *)(param_3 + 6) = result;
    if ((s32)shifted_first > -0x8000 && (s32)shifted_first < 0x8000) {
        param_3[0] = (u16)shifted_first;
        bit_count = 0;
        shifted_bit = shifted_second;
        while ((shifted_bit & 1u) == 0) {
            ++bit_count;
            shifted_bit = (u32)((s32)shifted_second >> (bit_count & 0x1fu));
        }
        param_3[1] = (u16)bit_count;
        *(volatile u32 *)(param_3 + 2) = shifted_second;
        *(volatile u32 *)(param_3 + 4) = (u32)((s32)shifted_second >> 1);
        param_3[8] = (u16)((u32)((s32)shifted_second >> (bit_count & 0x1fu)) != 1u);
    }
}
