#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090cfc0(s32 value);

u32 CM_STDCALL cm_fn_005c4680(volatile u8 *self)
{
    u32 comparison_object = *(volatile u32 *)(self + 0x53u);
    u16 date_bits = *(volatile u16 *)(self + 0x80u);
    u8 rank;
    s8 best_rank = -1;
    u32 best = 0;
    u32 count;
    u32 row;
    u32 index = 0;

    if ((s16)date_bits < 2000) {
        return 0;
    }
    if ((s16)date_bits < 4000) {
        rank = 2;
    } else if ((s16)date_bits < 6000) {
        rank = 3;
    } else if ((s16)date_bits < 8000) {
        rank = 4;
    } else {
        rank = 5;
    }

    count = CM_GLOBAL(volatile u32, 0x00ae2350u);
    row = CM_GLOBAL(volatile u32, 0x00ae23a8u);
    if ((s32)count > 0) {
        for (;;) {
            if (row != comparison_object &&
                *(volatile s16 *)(row + 0x86u) >= 0x14 &&
                *(volatile u8 *)(row + 0x7eu) == 1u &&
                (s32)*(volatile s8 *)(row + 0x84u) <= 2 &&
                *(volatile s16 *)(row + 0x8eu) >= 5) {
                u8 row_rank = rank;
                u8 qualifies = 1;

                if (row_rank == 2u) {
                    u8 row_country = *(volatile u8 *)(row + 0x75u);
                    u8 base_country = *(volatile u8 *)(comparison_object + 0x75u);
                    if (row_country != base_country) {
                        qualifies = 0;
                    }
                }
                if (qualifies && row_rank < 5u) {
                    u32 row_team = *(volatile u32 *)(row + 0x71u);
                    u32 base_team = *(volatile u32 *)(comparison_object + 0x71u);
                    if (row_team != base_team) {
                        qualifies = 0;
                    }
                }

                if (qualifies) {
                    s32 random = cm_fn_0090cfc0(100);
                    if ((s8)random > best_rank) {
                        best_rank = (s8)random;
                        best = row;
                    }
                }
            }

            count = CM_GLOBAL(volatile u32, 0x00ae2350u);
            index = (u16)(index + 1u);
            row += 0x122u;
            if (!((s16)index < (s32)count)) {
                break;
            }
        }
    }
    return best;
}
