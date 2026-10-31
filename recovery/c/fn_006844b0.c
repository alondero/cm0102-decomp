/* WIP: memory mismatch on page 0x03000000, case 1 after five iterations */
#include "cmre.h"

u32 CM_STDCALL cm_fn_006844b0(volatile u32 *rows, s32 n)
{
    s32 r, c;
    if (n >= 1) {
        r = n;
        volatile u32 *row_slot = rows + 1;
        do {
            volatile u32 *row = (volatile u32 *)*row_slot;
            c = 1;
            if (n > 1) do {
                ++c;
                row[c - 1] = 0;
            } while (c < n);
            --r;
            ++row_slot;
        } while (r != 0);
    }
    s32 i = 1;
    s32 j = n - 1;
    if (n > 1) {
        s32 k = -1;
        volatile u32 *slot = rows;
        do {
            ++slot;
            volatile u32 *row = (volatile u32 *)*slot;
            if (n / 2 < i) row[j] = (u32)-n;
            else row[j] = (u32)n;
            volatile u32 *last = (volatile u32 *)rows[n];
            if (n / 2 < i) last[j] = (u32)i;
            else last[j] = (u32)k;
            if (j == n - 2) j = 1;
            else if (j == n - 1) j = 2;
            else j += 2;
            ++i;
            --k;
        } while (i < n);
    }

    j = n - 1;
    if (j >= 2) {
        s32 neg = -j;
        volatile u32 *last_slot = rows + j;
        do {
            s32 col = j - 1;
            s32 toggle = (((s32)n - 1 + neg) & (s32)0x80000001);
            if (toggle < 0) toggle = ((toggle - 1) | (s32)0xfffffffe) + 1;
            s32 rowno = 1;
            if (n > 0) {
                s32 value = -1;
                s32 x = col;
                volatile u32 *slot = rows;
                do {
                    ++slot;
                    volatile u32 *row = (volatile u32 *)*slot;
                    volatile u32 *cell = row + x;
                    if (*cell == 0) {
                        if (toggle == 0) {
                            *cell = (u32)value;
                            volatile u32 *last = (volatile u32 *)*last_slot;
                            last[x] = (u32)rowno;
                        } else {
                            *cell = (u32)j;
                            volatile u32 *last = (volatile u32 *)*last_slot;
                            last[x] = (u32)value;
                        }
                        toggle = 1 - toggle;
                    }
                    if (x == n - 1) x = 1;
                    else ++x;
                    ++rowno;
                    --value;
                } while (rowno <= n);
            }
            ++neg;
            --last_slot;
            --j;
        } while (j >= 2);
    }
    return 1;
}
