/* Reconstructed from CM0102 3.9.68 instructions at preferred-image
   VA 0x00933e50. DWORD formals retain overwritten caller argument homes;
   volatile word views retain the original short reads without entry copies. */
#include "cmre.h"
typedef s16 home_word __attribute__((__may_alias__));

void cm_fn_00933e50(int param_1, volatile int param_2,
                    volatile u32 param_3, u32 param_4,
                    short param_5, volatile u32 param_6,
                    volatile u32 param_7)
{
    (void)param_4;
    const u32 A = (u32)param_1;
    const u32 B = (u32)param_2;
    const s16 flag = *(volatile home_word *)&param_3; /* 00933e54 */
    const s32 divisor = (s32)(s16)*(volatile u16 *)(A + 0x29c);
    u32 scale;
    u32 rows;
    u32 dest;
    if (flag == 0) {
        scale = *(volatile u32 *)(A + 0x2a2);
        rows = *(volatile u32 *)(A + 0x16);
        dest = *(volatile u32 *)(B + 0x1c);
    }
    else {
        scale = *(volatile u32 *)(A + 0x29e);
        rows = *(volatile u32 *)(A + 0x12);
        dest = *(volatile u32 *)(B + 0x18);
    }
    scale <<= 6;
    const s32 i7 = (s32)*(volatile home_word *)&param_7; /* 00933e8f */
    const u32 mult = *(volatile u32 *)(A + 0x426); /* 00933e94 */
    param_3 = dest;                              /* 00933e9a */
    const s32 i6 = (s32)*(volatile home_word *)&param_6; /* 00933e9e */
    const s32 row7 = (s32)*(volatile s16 *)(rows + (u32)(i7 * 2));
    param_6 = mult;                              /* 00933ea7 */
    param_2 = row7;                              /* 00933eab */
    const s32 row6 = (s32)*(volatile s16 *)(rows + (u32)(i6 * 2));
    const s32 half = divisor >> 1;

    u32 t = (u32)row7 - (u32)row6;
    t = t * scale;
    t = t + (u32)half;
    s32 q = (s32)t / divisor;
    u32 v = (u32)q * param_6;                     /* 00933ec4 */
    v = v + 0x20u;
    s32 sv = (s32)v >> 6;
    sv = sv + ((sv < 0) ? 0x3f : 0);
    dest = param_3;                              /* 00933ed5 */
    sv = sv >> 6;
    u32 val6 = *(volatile u32 *)(dest + (u32)(i6 * 4));
    val6 = val6 + (u32)sv;
    const s32 i5 = (s32)(s16)param_5;             /* 00933ee1 */
    const s32 row5 = (s32)*(volatile s16 *)(rows + (u32)(i5 * 2));
    u32 t2 = (u32)param_2 - (u32)row5;
    t2 = t2 * scale;
    t2 = t2 + (u32)half;
    s32 q2 = (s32)t2 / divisor;
    dest = param_3;                              /* 00933efc */
    u32 v2 = (u32)q2 * *(volatile u32 *)(A + 0x426);
    v2 = v2 + 0x20u;
    s32 s2 = (s32)v2 >> 6;
    s2 = s2 + ((s2 < 0) ? 0x3f : 0);
    s2 = s2 >> 6;
    u32 val5 = *(volatile u32 *)(dest + (u32)(i5 * 4));
    val5 = val5 + (u32)s2;
    const u32 wrap_flag = *(volatile u32 *)(A + 0x42a);
    s32 cand = (s32)val5;
    const s32 other = (s32)val6;
    if (wrap_flag == 0) {
        if (cand < other) {
            cand = (s32)((u32)cand + 0x40u);
            if (cand > other) cand = other;
        }
        else {
            cand = (s32)((u32)cand - 0x40u);
            if (cand < other) cand = other;
        }
    }
    const u32 mode = *(volatile u32 *)(A + 0x2a6);
    u32 res;
    if (mode == 2) {
        u32 w = (u32)cand + (u32)other + 1u;
        s32 sw = (s32)w;
        sw = sw - (sw < 0 ? -1 : 0);
        res = (u32)(sw >> 1);
    }
    else {
        u32 w = (u32)cand + (u32)other * 2u + 1u;
        s32 sw = (s32)w;
        res = (u32)(sw / 3);
    }
    s32 write_index = (s32)*(volatile home_word *)&param_7;
    *(volatile u32 *)(dest + (u32)(write_index * 4)) = res;
    s32 round_index = (s32)*(volatile home_word *)&param_7; /* 00933f7c */
    u32 final = *(volatile u32 *)(dest + (u32)(round_index * 4));
    final = (final + 0x20u) & 0xffffffc0u;
    *(volatile u32 *)(dest + (u32)(round_index * 4)) = final;
}
