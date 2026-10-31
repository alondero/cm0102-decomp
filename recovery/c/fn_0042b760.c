#include "cmre.h"

/* Callees. cm_fn_004afd80 is cdecl: the four calls below push their arguments
   and the single `add esp, 0x20` after the fourth call pops all of them.
   cm_fn_006826d0 is thiscall with one stack argument that it pops, and it
   returns a pointer to a dword. */
extern u32 cm_fn_004afd80(u32 arg_0, u32 arg_1);
extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 arg);

/* Scratch frame: the locals between the saved-register pushes and the return
   address. Offsets are from the frame base B, the esp value just after
   `sub esp, 0x64`. The byte array, call results and table are one block, as
   in the original (0x0c..0x63). */
typedef struct {
    u8 unused_00[2];   /* 0x00-0x01 */
    s8 cnt;            /* 0x02: index of the first pass (al) */
    s8 k2;             /* 0x03: outer index of the last pass */
    u32 elem1;         /* 0x04: element pointer, table[global 0x009cf9e8] */
    u32 elem6;         /* 0x08: element pointer, table[global 0x009cf9ec] */
    s8 ba[24];         /* 0x0c: ba[k] counters for k 0..11, limits at ba[k + 12] */
    u32 r[4];          /* 0x24: results of the four cm_fn_004afd80 calls */
    u32 t[12];         /* 0x34: element pointers table[k], k 0..11 */
} cm_frame;

/* Reads the table entry at `index` (dword array at `tab`). */
static u32 table_at(u32 tab, u32 index)
{
    return *(volatile u32 *)(tab + index * 4u);
}

/* Non-zero when v is one of the four call results. */
static int r_has(const u32 *r, u32 v)
{
    int k;
    for (k = 0; k < 4; k++) {
        if (r[k] == v) {
            return 1;
        }
    }
    return 0;
}

/* Pass body for one table element `elem`: walk its entries j (while j is
   below its signed word count at +0x3e), calling cm_fn_006826d0 twice when
   an entry value is not in r. The first unmatched value is stored at the
   output record for bp (6-byte records, dword at offset 0, base at elem1+0x14). */
static void scan_element(u32 elem1, const u32 *r, u32 elem, s16 bp)
{
    s16 j = 0;

    if (*(volatile s16 *)(elem + 0x3eu) <= 0) {
        return;
    }
    do {
        u32 p = cm_fn_006826d0((void *)elem, (u32)(s32)j);
        u32 v = *(volatile u32 *)p;

        if (!r_has(r, v)) {
            u32 p2 = cm_fn_006826d0((void *)elem, (u32)(s32)j);
            u32 val = *(volatile u32 *)p2;
            u32 base = *(volatile u32 *)(elem1 + 0x14u);

            *(volatile u32 *)(base + (u32)((s32)bp * 6)) = val;
            return;
        }
        j++;
    } while (j < *(volatile s16 *)(elem + 0x3eu));
}

void cm_fn_0042b760(void)
{
    cm_frame f;
    u32 tab;
    u32 idx;
    s16 bp;
    s8 bl;
    u32 g;
    u32 e;
    u32 elem6;

    tab = CM_GLOBAL(volatile u32, 0x00adadfc);
    idx = CM_GLOBAL(volatile u32, 0x009cf9ec);
    f.elem6 = table_at(tab, idx);
    f.t[0] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9e4));
    f.t[1] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9dc));
    f.t[2] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9d4));
    f.t[3] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9d0));
    f.t[4] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9c8));
    f.t[5] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9b4));
    f.t[6] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9bc));
    f.t[7] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9c0));
    f.t[8] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9a4));
    f.t[9] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9b0));
    f.t[10] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf998));
    f.t[11] = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf994));
    f.elem1 = table_at(tab, CM_GLOBAL(volatile u32, 0x009cf9e8));

    /* Byte array: 0 at 0..11, 5 at 12..19, then 7, 5, 7, 7. */
    f.ba[0] = 0;
    f.ba[1] = 0;
    f.ba[2] = 0;
    f.ba[3] = 0;
    f.ba[4] = 0;
    f.ba[5] = 0;
    f.ba[6] = 0;
    f.ba[7] = 0;
    f.ba[8] = 0;
    f.ba[9] = 0;
    f.ba[10] = 0;
    f.ba[11] = 0;
    f.ba[12] = 5;
    f.ba[13] = 5;
    f.ba[14] = 5;
    f.ba[15] = 5;
    f.ba[16] = 5;
    f.ba[17] = 5;
    f.ba[18] = 5;
    f.ba[19] = 5;
    f.ba[20] = 7;
    f.ba[21] = 5;
    f.ba[22] = 7;
    f.ba[23] = 7;

    /* Four calls, pushed in order; the callee is cdecl. */
    f.r[0] = cm_fn_004afd80(*(volatile u32 *)(f.elem6 + 4u), 0);
    f.r[1] = cm_fn_004afd80(*(volatile u32 *)(f.elem6 + 4u), 1);
    f.r[2] = cm_fn_004afd80(*(volatile u32 *)(f.elem1 + 4u), 0);
    g = CM_GLOBAL(volatile u32, 0x009cf9b8);
    e = CM_GLOBAL(volatile u32, 0x00ae23d0);
    f.r[3] = cm_fn_004afd80(e + g * 107u, 0);

    /* Pass 1: table entries 0..4 while bp stays non-negative. */
    bp = 0x46;
    f.cnt = 0;
    while (f.cnt < 5 && bp >= 0) {
        scan_element(f.elem1, f.r, f.t[f.cnt], bp);
        bp--;
        f.cnt++;
    }

    /* Pass 2: table entries 5..9. */
    bl = 5;
    while (bp >= 0 && bl < 10) {
        scan_element(f.elem1, f.r, f.t[bl], bp);
        bp--;
        bl++;
    }

    /* Pass 3: table entries 10..11. */
    bl = 10;
    while (bp >= 0 && bl < 12) {
        scan_element(f.elem1, f.r, f.t[bl], bp);
        bp--;
        bl++;
    }

    /* Pass 4: the element f.elem6, passes with bl from 2 while bl < its count. */
    elem6 = f.elem6;
    bl = 2;
    if (*(volatile s16 *)(elem6 + 0x3eu) > 2) {
        for (;;) {
            int found = 0;

            if (bp < 0) {
                break;
            }
            if (bp < 0x46) {
                u32 base = *(volatile u32 *)(f.elem1 + 0x14u);
                u32 tbl = *(volatile u32 *)(elem6 + 0xb1u);
                u32 v = *(volatile u32 *)(tbl + (u32)((s32)bl * 59));
                s16 cx = (s16)(bp + 1);

                do {
                    if (*(volatile u32 *)(base + (u32)((s32)cx * 6)) == v) {
                        found = 1;
                        break;
                    }
                    cx++;
                } while (cx < 0x47);
            }
            if (!found) {
                u32 tbl2 = *(volatile u32 *)(elem6 + 0xb1u);
                u32 v2 = *(volatile u32 *)(tbl2 + (u32)((s32)bl * 59));

                if (!r_has(f.r, v2)) {
                    u32 base2 = *(volatile u32 *)(f.elem1 + 0x14u);

                    *(volatile u32 *)(base2 + (u32)((s32)bp * 6)) = v2;
                    bp--;
                }
            }
            bl++;
            if (!((s16)bl < *(volatile s16 *)(elem6 + 0x3eu))) {
                break;
            }
        }
    }

    /* Final pass: byte-array counters against their limits, outer k2 1..9. */
    f.k2 = 1;
    do {
        f.cnt = 0;
        do {
            s8 cur = f.ba[f.cnt];
            s8 lim = f.ba[f.cnt + 12];

            if (cur < lim) {
                u32 elem = f.t[f.cnt];

                if ((s16)f.k2 < *(volatile s16 *)(elem + 0x3eu)) {
                    u32 tbl = *(volatile u32 *)(elem + 0xb1u);
                    u32 v = *(volatile u32 *)(tbl + (u32)((s32)f.k2 * 59));

                    if (!r_has(f.r, v)) {
                        int found = 0;

                        if (bp < 0x46) {
                            u32 base = *(volatile u32 *)(f.elem1 + 0x14u);
                            s16 cx = (s16)(bp + 1);

                            do {
                                if (*(volatile u32 *)(base + (u32)((s32)cx * 6)) == v) {
                                    found = 1;
                                    break;
                                }
                                cx++;
                            } while (cx < 0x47);
                        }
                        if (!found) {
                            u32 base2 = *(volatile u32 *)(f.elem1 + 0x14u);

                            *(volatile u32 *)(base2 + (u32)((s32)bp * 6)) = v;
                            bp--;
                            if (bp < 0) {
                                return;
                            }
                            f.ba[f.cnt] = (s8)(cur + 1);
                        }
                    }
                }
            }
            f.cnt++;
        } while (f.cnt < 12);
        if (bp < 0) {
            return;
        }
        f.k2++;
    } while (f.k2 < 10);
}
