#include "cmre.h"

extern void CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);
extern void CM_CDECL cm_fn_0090ba90(void *arg1, void *arg2, u32 count);

/* Thiscall with four stack arguments (ret 0x10). Fields of the receiver, by offset: a signed
   word index at +0x382a, a table of 16 signed words at +0x380a, and 768-byte rows starting
   at +0x7d4 (a dword key at row+2 of +0x7d6 is compared with arg1 when arg1 is nonzero).
   The routine saves the index word in a stack slot, runs the index from 0 to 15 (the index
   word is re-read after each call, so callees may change it), and restores the saved word
   at the end. Memory moves use the 0x0090ba90 copy routine with 0x30-byte records. */
void CM_THISCALL cm_fn_007ff130(u8 *param_1, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u16 saved;
    s16 w;
    s16 wcur;
    s16 j;
    u32 row;
    u32 b;

    b = arg1;
    saved = *(volatile u16 *)(param_1 + 0x382a);
    *(volatile u16 *)(param_1 + 0x382a) = 0;

    for (;;) {
        w = (s16)*(volatile u16 *)(param_1 + 0x382a);
        if (*(volatile u16 *)(param_1 + 0x380a + (u32)(s32)w * 2U) != 0) {
            if (b == 0 || b == *(volatile u32 *)(param_1 + 0x7d6 + (u32)(s32)w * 768U)) {
                cm_fn_007ff270((int)param_1, (int)arg2, (char *)arg3, (int)arg4);
            }
            wcur = (s16)*(volatile u16 *)(param_1 + 0x382a);
            j = 1;
            if ((s16)*(volatile u16 *)(param_1 + 0x380a + (u32)(s32)wcur * 2U) > j) {
                do {
                    row = (u32)param_1 + 0x7d4U + (u32)(s32)wcur * 768U;
                    cm_fn_0090ba90((void *)row, (void *)(row + 0x30U * (u32)(s32)j), 0x30U);
                    if (b == 0 ||
                        b == *(volatile u32 *)(param_1 + 0x7d6 +
                                               (u32)(s32)(s16)*(volatile u16 *)(param_1 + 0x382a) * 768U)) {
                        cm_fn_007ff270((int)param_1, (int)arg2, (char *)arg3, (int)arg4);
                    }
                    wcur = (s16)*(volatile u16 *)(param_1 + 0x382a);
                    j++;
                } while (j < (s16)*(volatile u16 *)(param_1 + 0x380a + (u32)(s32)wcur * 2U));
            }
            wcur = (s16)*(volatile u16 *)(param_1 + 0x382a);
            if ((s16)*(volatile u16 *)(param_1 + 0x380a + (u32)(s32)wcur * 2U) > 1) {
                row = (u32)param_1 + 0x7d4U + (u32)(s32)wcur * 768U;
                cm_fn_0090ba90((void *)row, (void *)(row + 0x30U), 0x30U);
            }
        }
        *(volatile u16 *)(param_1 + 0x382a) = (u16)(*(volatile u16 *)(param_1 + 0x382a) + 1U);
        if ((s16)*(volatile u16 *)(param_1 + 0x382a) >= 0x10) {
            break;
        }
    }
    *(volatile u16 *)(param_1 + 0x382a) = saved;
}
