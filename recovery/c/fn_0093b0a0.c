#include "cmre.h"

extern int cm_fn_009446e0(u32 param_1, u32 param_2);
extern int cm_fn_009447e0(u32 param_1, u16 param_2);

/* Callback called for each matching entry; the listing calls it through the stack slot that
   holds the second argument. */
typedef void (CM_CDECL *cm_entry_cb)(u8 *state, u32 extra, u32 entry, u32 value);

/* Cdecl with four stack arguments (ret 0): arg1 is a state record S; arg2 is the callback;
   arg3 is a signed word; arg4's low byte is a shift count.
   Fields of S, by offset: +0x14 and +0x16 words, +0x20 a dword cursor into a table of
   dwords (updated twice; the table holds entries of two dwords), +0x28 a pointer to a
   structure P (bytes at +0xb6, dword values at +0xb8 and +0xbc, word at +0xc), and +0x8.
   The routine computes a key from P and the table count, narrows an index by a binary
   search over the table, then calls the callback for each matching pair of entries. */
void CM_CDECL cm_fn_0093b0a0(u8 *param_1, u32 param_2, u32 param_3, u32 param_4)
{
    u8 *s;
    u8 *p;
    u32 a20;
    u32 x;
    u32 newcur;
    u32 n;
    u32 step;
    u32 idx;
    u32 arr;
    u32 key;
    u32 e;
    u32 e2;
    u32 bp;
    u32 v;
    u32 q;
    s32 r;
    s32 w1;
    s32 wx;
    u16 ax;
    u16 cx;
    u8 al;
    int have_value;

    s = param_1;
    bp = 0;
    a20 = CM_GLOBAL(volatile u32, (u32)s + 0x20U);
    p = (u8 *)CM_GLOBAL(volatile u32, (u32)s + 0x28U);
    a20 = a20 - 4U;
    CM_GLOBAL(volatile u32, (u32)s + 0x20U) = a20;
    x = CM_GLOBAL(volatile u32, a20);
    x = x << 1;
    newcur = a20 - 4U * x;
    *(u32 *)(s + 0x20) = newcur;

    al = CM_GLOBAL(volatile u8, (u32)p + 0xb6U);
    bp = CM_GLOBAL(volatile u16, (u32)p + 0xcU);
    have_value = 0;
    if (al == 0) {
        ax = CM_GLOBAL(volatile u16, (u32)s + 0x16U);
        cx = CM_GLOBAL(volatile u16, (u32)s + 0x14U);
        if (ax == 0) {
            v = CM_GLOBAL(volatile u32, (u32)p + 0xb8U);
        } else if (cx == 0) {
            v = CM_GLOBAL(volatile u32, (u32)p + 0xbcU);
        } else {
            w1 = (s32)(s16)ax;
            q = (u32)(((s32)w1 * w1 + 0x2000) >> 14);
            r = cm_fn_009447e0(CM_GLOBAL(volatile u32, (u32)p + 0xbcU), (u16)q);
            q = CM_GLOBAL(volatile u32, (u32)p + 0xb8U);
            wx = (s32)(s16)(u16)x;
            e = (u32)(((s32)wx * wx + 0x2000) >> 14);
            e2 = (u32)cm_fn_009447e0(q, (u16)e);
            v = e2 + (u32)r;
        }
        have_value = 1;
    }
    if (have_value) {
        bp = (u32)cm_fn_009446e0(bp, v);
    }
    bp = bp - (u32)(s32)(s16)param_3;

    if ((s32)bp >= 0x10 || (s32)bp < 0) {
        return;
    }

    n = (u32)(((s32)x) >> 1);
    n = n & ~1U;
    bp = bp << 4;
    idx = 0;
    step = n;
    if ((s32)n > 2) {
        do {
            arr = *(u32 *)(s + 0x20);
            key = (u32)(idx + step);
            e = *(u32 *)(arr + key * 4U) & 0xfffffff0U;
            if ((s32)e < (s32)bp) {
                idx = key;
            }
            step = (u32)((s32)step >> 1) & ~1U;
        } while ((s32)step > 2);
    }

    if ((s32)idx >= (s32)x) {
        return;
    }
    for (;;) {
        arr = *(u32 *)(s + 0x20);
        e = *(u32 *)(arr + idx * 4U);
        if ((e & 0xfffffff0U) != bp) {
            if ((s32)(e & 0xfffffff0U) > (s32)bp) {
                return;
            }
        } else {
            n = e & 0xfU;
            key = (u32)((s32)n < 8);
            step = n - (7U + key);
            e2 = *(u32 *)(arr + idx * 4U + 4U);
            v = (u32)(((s32)(step << 6)) >> (param_4 & 31U));
            ((cm_entry_cb)param_2)(s, *(u32 *)(s + 8), e2, v);
        }
        idx = idx + 2U;
        if ((s32)idx >= (s32)x) {
            break;
        }
    }
}
