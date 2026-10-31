#include "cmre.h"

u16 CM_CDECL cm_fn_00933b20(u32 object, u32 index)
{
    u32 p_a, p_b, p_temp, p_flags;
    s32 a, top_ok = 0, bottom_ok = 0;
    s16 bx, si, current;
    s32 day, quarter, sixteenth, best, temp_flag;
    s32 initial_flag;
    s32 i, result = -1;
    volatile s16 *row;

    p_a = *(volatile u32 *)(object + 0x40);
    a = *(volatile s32 *)(p_a + index * 4);
    if (a <= 0x3f07) {
        p_b = *(volatile u32 *)(object + 0x48);
        top_ok = (*(volatile s32 *)(p_b + index * 4) <= 0x3f07);
    }
    if (a >= -0x3f07) {
        p_b = *(volatile u32 *)(object + 0x48);
        bottom_ok = (*(volatile s32 *)(p_b + index * 4) >= -0x3f07);
    }

    p_a = *(volatile u32 *)(object + 0x1c);
    p_b = *(volatile u32 *)(object + 0x20);
    bx = *(volatile s16 *)(p_a + index * 2);
    p_temp = *(volatile u32 *)(object + 0x16);
    si = *(volatile s16 *)(p_b + index * 2);
    current = *(volatile s16 *)(p_temp + index * 2);

    if (top_ok && ((*(volatile u8 *)(*(volatile u32 *)(object + 0x28) + index * 2) & 8) != 0)) {
        s16 x = *(volatile s16 *)(p_temp + (s32)si * 2);
        if (current > x) {
            x = *(volatile s16 *)(p_temp + (s32)bx * 2);
            if (current > x) top_ok = 0;
        }
    }
    if (bottom_ok && ((*(volatile u8 *)(*(volatile u32 *)(object + 0x28) + index * 2) & 8) != 0)) {
        s16 x = *(volatile s16 *)(p_temp + (s32)si * 2);
        if (current < x) {
            x = *(volatile s16 *)(p_temp + (s32)bx * 2);
            if (current < x) bottom_ok = 0;
        }
    }
    if (top_ok && bottom_ok) return 0xffff;

    day = (s32)*(volatile s16 *)(object + 0x29c);
    quarter = (day + ((day >> 31) & 3)) >> 2;
    sixteenth = (day + ((day >> 31) & 15)) >> 4;
    best = day / 66;
    p_flags = *(volatile u32 *)(object + 0x0e);
    initial_flag = *(volatile u8 *)(p_flags + index);
    temp_flag = *(volatile s16 *)(object + 0x1a);

    if (initial_flag != 0) {
        if (*(volatile u8 *)(p_flags + (s32)bx) != 0) {
            u32 p_date = *(volatile u32 *)(object + 0x12);
            s16 here = *(volatile s16 *)(p_date + index * 2);
            s16 there = *(volatile s16 *)(p_date + (s32)bx * 2);
            s16 d = (s16)((u16)here - (u16)there);
            if (d < 0) d = (s16)-d;
            if (d > sixteenth && (a > 0x3fc2 || a < -0x3fc2)) goto prefer_false;
        }
        if (*(volatile u8 *)(p_flags + (s32)si) != 0) {
            u32 p_date = *(volatile u32 *)(object + 0x12);
            s16 here = *(volatile s16 *)(p_date + index * 2);
            s16 there = *(volatile s16 *)(p_date + (s32)si * 2);
            s16 d = (s16)((u16)here - (u16)there);
            if (d < 0) d = (s16)-d;
            if (d > sixteenth) {
                p_b = *(volatile u32 *)(object + 0x48);
                a = *(volatile s32 *)(p_b + index * 4);
                if (a > 0x3fc2 || a < -0x3fc2) goto prefer_false;
            }
        }
    }
    i = 1;
    goto scan;
prefer_false:
    i = 0;
scan:
    row = (volatile s16 *)(object + 0x34e);
    {
        s32 k = 0;
        for (;;) {
            s32 bound = *row;
            s32 allowed = (bound > quarter) ? top_ok : bottom_ok;
            if (!allowed && ((k == 2 || k == 6) ? temp_flag != 0 : temp_flag == 0)) {
                s32 delta = (s32)bound - (s32)current;
                if (delta < 0) delta = -delta;
                if (delta < best) {
                    best = delta;
                    result = k * 2;
                    if (delta == 0) break;
                }
                if (i != 0) {
                    s32 neighbor = (s32)row[1];
                    delta = (s32)current - neighbor;
                    if (delta < 0) delta = -delta;
                    if (delta < best) {
                        best = delta;
                        result = k * 2 + 1;
                        if (delta == 0) break;
                    }
                }
            }
            ++k;
            row += 3;
            if (k > 9) break;
        }
    }
    return (u16)result;
}
