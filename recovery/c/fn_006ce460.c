#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define F(a) CM_GLOBAL(volatile float, (u32)(a))
extern s32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_006ce9c0(u32, u8);
typedef u16 word_6ce460 __attribute__((may_alias));

/* Preferred-image VA006ce460/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full EAX from original006CE9C0. That entire 455-byte callee
   reads only BYTE argument0, then overwrites EAX at006CE9D1 before any
   call; the upper three unwritten bytes of rootL4 are not observed. No
   zero-fill of them is introduced. Root's FILD/FSTP inputs are signed
   bytes, so conversion to float is exact. No full FP-state claim.
   All three original aggregation blocks have the same accesses/order. */
u32 CM_THISCALL cm_fn_006ce460(u32 self, volatile u32 mode_arg)
{
    u16 state = W(self + 0x8ed4);
    u8 classification = state == 0x1ef ? 0 : (state == 0x3de ? 1 : 2);
    s16 mode = (s16)CM_GLOBAL(volatile word_6ce460, (u32)&mode_arg);
    u32 result, left, right, row, sums, group, n, value, i;
    s32 quotient;
    if (mode == 0 || mode == 1) {
        result = (u32)cm_fn_0090cfc0(7);
        mode_arg = (u32)(s32)(s8)(u8)((u8)result + 3);
        F(self + 0xf628) = (float)(s32)mode_arg;
        result = (u32)cm_fn_0090cfc0(7);
        mode_arg = (u32)(s32)(s8)(u8)((u8)result + 3);
        F(self + 0xf62c) = (float)(s32)mode_arg;
        result = (u32)cm_fn_0090cfc0(5);
        left = B(self + 0xf5bd);
        right = B(self + 0xf5f3);
        result += left;
        result += right;
        if ((s32)result < 10) {
            result = 10;
        } else {
            result = (u32)cm_fn_0090cfc0(5);
            left = B(self + 0xf5bd);
            right = B(self + 0xf5f3);
            result += left;
            result += right;
        }
        mode_arg = (u32)(s32)(s8)(u8)result;
        F(self + 0xf630) = (float)(s32)mode_arg;
    }
    if (mode == 0 || mode == 1 || mode == 0x3de) {
        row = self + 0x47b1;
        sums = self + 0xf5c0;
        for (group = 0; group != 2; ++group) {
            B(sums + 2) = 0;
            B(sums) = 0;
            B(sums + 1) = 0;
            B(sums - 2) = 0;
            B(sums - 1) = 0;
            for (i = 0; i != 8; ++i)
                W(sums + 0x22 + i * 2u) = 0;
            B(sums + 0xb) = 0;
            B(sums + 0xc) = 0;
            n = 20;
            do {
                quotient = ((s32)(s16)W(row + 0x1a) + 500) / 1000;
                B(row) = (u8)quotient;
                if ((s8)(u8)quotient > 10)
                    B(row) = 10;
                if ((s8)B(row) < 1)
                    B(row) = 1;
                value = B(row - 0xc);
                result = B(sums + 2);
                row += 0x1be;
                B(sums + 2) = (u8)(result + value);
                value = B(row - 0x1cc);
                B(sums) = (u8)(B(sums) + value);
                value = B(row - 0x1cb);
                B(sums + 1) = (u8)(B(sums + 1) + value);
                value = B(row - 0x1cf);
                B(sums - 2) = (u8)(B(sums - 2) + value);
                value = B(row - 0x1ce);
                B(sums - 1) = (u8)(B(sums - 1) + value);
                value = B(row - 0x1d8);
                W(sums + 0x22) = (u16)(W(sums + 0x22) + value);
                value = B(row - 0x1d7);
                W(sums + 0x24) = (u16)(W(sums + 0x24) + value);
                value = B(row - 0x1d5);
                W(sums + 0x26) = (u16)(W(sums + 0x26) + value);
                value = B(row - 0x1d4);
                W(sums + 0x28) = (u16)(W(sums + 0x28) + value);
                value = B(row - 0x1d2);
                W(sums + 0x2a) = (u16)(W(sums + 0x2a) + value);
                value = B(row - 0x1d1);
                W(sums + 0x2c) = (u16)(W(sums + 0x2c) + value);
                value = B(row - 0x1d0);
                W(sums + 0x2e) = (u16)(W(sums + 0x2e) + value);
                value = B(row - 0x1c7);
                W(sums + 0x30) = (u16)(W(sums + 0x30) + value);
                value = B(row - 0x1b6);
                B(sums + 0xb) = (u8)(B(sums + 0xb) + value);
                value = B(row - 0x1b5);
                result = B(sums + 0xc);
                --n;
                B(sums + 0xc) = (u8)(result + value);
            } while (n != 0);
            sums += 0x36;
        }
    }
    return cm_fn_006ce9c0(self, classification);
}
