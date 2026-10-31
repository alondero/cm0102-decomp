/* WIP: fault-outcome mismatch, case 15, after five failed iterations. */
#include "cmre.h"

s32 CM_CDECL cm_fn_009349a0(volatile u16 *base_a, s16 flag_a,
    volatile u16 *base_b, u32 unused4, s32 target_or_flag,
    u32 unused6, s32 target_b)
{
    s32 best_index = -1;
    s32 best_distance = 0x7fff;
    u32 i;
    volatile u16 *table;
    s16 center;
    s16 lower;
    s16 upper;
    s16 delta;
    s32 distance;

    (void)unused4;
    (void)unused6;
    if (flag_a != 0) {
        table = (volatile u16 *)((volatile u8 *)base_a + 0x38a);
        for (i = 0; i < 12; ++i) {
            center = table[i];
            if (center == 0)
                continue;
            delta = (s16)((u16)center - (u16)target_or_flag);
            distance = delta < 0 ? -(s32)delta : (s32)delta;
            lower = (s16)(center - (center >> 1));
            upper = (s16)(center + (center >> 1));
            if (distance < best_distance && target_or_flag >= lower && target_or_flag <= upper) {
                best_distance = distance;
                best_index = (s32)i;
            }
        }
        if (best_index >= 0)
            return best_index + 0x48;
    } else if ((u16)target_or_flag != 0) {
        table = (volatile u16 *)((volatile u8 *)base_b + 0x3a2);
        for (i = 0; i < 12; ++i) {
            center = table[i];
            if (center == 0)
                continue;
            delta = (s16)((u16)center - (u16)target_b);
            distance = delta < 0 ? -(s32)delta : (s32)delta;
            lower = (s16)(center - (center >> 1));
            upper = (s16)(center + (center >> 1));
            if (distance < best_distance && target_b >= lower && target_b <= upper) {
                best_distance = distance;
                best_index = (s32)i;
            }
        }
        if (best_index >= 0)
            return best_index + 0x54;
    }
    return -1;
}
