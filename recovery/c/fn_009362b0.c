#include "cmre.h"

/* First argument (ESI), fields by byte offset:
   0x29c signed word, scales the match threshold;
   0x444 u32 handle passed to cm_fn_009443a0 and cm_fn_009445d0;
   0x50 signed dword record count;
   0x58 record array base, 8-byte records.
   Record: byte 0 kind (3 = candidate), byte 1 subkind (1 or 2),
   signed word 4 and signed word 6 are indices into a word table.
   Second argument (EBP) holds the two word-table pointers at +0x0c (subkind 1)
   and +0x10 (subkind 2). */

extern u32 CM_CDECL cm_fn_009443a0(u32 arg1, u32 arg2);
extern void CM_CDECL cm_fn_009445d0(u32 arg1, u32 arg2);

#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define OBJ_S16(off) (*(volatile s16 *)(self + (off)))
#define ARG_U32(base, off) (*(volatile u32 *)((base) + (off)))
#define REC_U8(off) (*(volatile u8 *)(rec + (off)))
#define REC_S16(off) (*(volatile s16 *)(rec + (off)))
#define TBL_S16(tbl, idx) (*(volatile s16 *)((tbl) + 2u * (u32)(s32)(idx)))

u32 CM_CDECL cm_fn_009362b0(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6)
{
    u32 self = arg1;
    u32 pairs = arg2;
    long long product;
    u32 hi;
    u32 scaled;
    s16 threshold;
    s32 n;
    s32 i;
    u32 left;
    u32 rec;
    u32 table;
    u8 subkind;
    s16 idx_a;
    s16 idx_b;
    s16 v_a;
    s16 v_b;
    s16 d;
    u32 match1 = 0;
    u32 match2 = 0;
    u32 out1;
    u32 out2;
    u32 cursor1;
    u32 cursor2;
    u32 count1 = 0;
    u32 count2 = 0;
    u32 status = 0;

    /* Threshold: high dword of 0x55555556 * word(0x29c), plus its sign bit, plus one. */
    product = (long long)(s32)0x55555556 * (long long)OBJ_S16(0x29c);
    hi = (u32)((long long)product >> 32);
    scaled = hi + (hi >> 31) + 1u;
    threshold = (s16)(u16)scaled;

    /* First pass: count candidates of each subkind within the threshold. */
    n = (s32)OBJ_U32(0x50);
    if (n > 0) {
        rec = OBJ_U32(0x58);
        left = (u32)n;
        do {
            if (REC_U8(0) == 3) {
                subkind = REC_U8(1);
                if (subkind == 1) {
                    idx_a = REC_S16(6);
                    table = ARG_U32(pairs, 0x0c);
                    idx_b = REC_S16(4);
                    v_a = TBL_S16(table, idx_a);
                    v_b = TBL_S16(table, idx_b);
                    d = (s16)(v_a - v_b);
                    if (d < 0) {
                        d = (s16)-d;
                    }
                    if (d <= threshold) {
                        match1++;
                    }
                } else if (subkind == 2) {
                    idx_a = REC_S16(6);
                    table = ARG_U32(pairs, 0x10);
                    idx_b = REC_S16(4);
                    v_a = TBL_S16(table, idx_a);
                    v_b = TBL_S16(table, idx_b);
                    d = (s16)(v_a - v_b);
                    if (d < 0) {
                        d = (s16)-d;
                    }
                    if (d <= threshold) {
                        match2++;
                    }
                }
            }
            rec += 8u;
        } while (--left != 0u);
    }

    /* Two output buffers sized 2 * count + 2; the second call follows the first. */
    out1 = cm_fn_009443a0(OBJ_U32(0x444), match1 * 2u + 2u);
    out2 = cm_fn_009443a0(OBJ_U32(0x444), match2 * 2u + 2u);

    if (out1 == 0u || out2 == 0u) {
        status = 0xffffffffu;
        cm_fn_009445d0(OBJ_U32(0x444), out1);
        cm_fn_009445d0(OBJ_U32(0x444), out2);
        out1 = 0u;
        out2 = 0u;
        count1 = 0u;
        count2 = 0u;
    } else {
        /* Second pass: store the absolute differences that pass, cursors advance by 2. */
        status = 0u;
        cursor1 = out1;
        cursor2 = out2;
        count1 = 0u;
        count2 = 0u;
        i = 0;
        n = (s32)OBJ_U32(0x50);
        if (n > 0) {
            do {
                rec = OBJ_U32(0x58) + (u32)i * 8u;
                if (REC_U8(0) == 3) {
                    subkind = REC_U8(1);
                    if (subkind == 1) {
                        idx_a = REC_S16(6);
                        idx_b = REC_S16(4);
                        table = ARG_U32(pairs, 0x0c);
                        v_a = TBL_S16(table, idx_a);
                        v_b = TBL_S16(table, idx_b);
                        d = (s16)(v_a - v_b);
                        if (d < 0) {
                            d = (s16)-d;
                        }
                        if (d <= threshold) {
                            *(volatile s16 *)cursor1 = d;
                            cursor1 += 2u;
                            count1++;
                        }
                    } else if (subkind == 2) {
                        idx_a = REC_S16(6);
                        idx_b = REC_S16(4);
                        table = ARG_U32(pairs, 0x10);
                        v_a = TBL_S16(table, idx_a);
                        v_b = TBL_S16(table, idx_b);
                        d = (s16)(v_a - v_b);
                        if (d < 0) {
                            d = (s16)-d;
                        }
                        if (d <= threshold) {
                            *(volatile s16 *)cursor2 = d;
                            cursor2 += 2u;
                            count2++;
                        }
                    }
                }
                i++;
                n = (s32)OBJ_U32(0x50);
            } while (i < n);
        }
    }

    *(volatile u32 *)arg3 = out1;
    *(volatile u32 *)arg4 = count1;
    *(volatile u32 *)arg5 = out2;
    *(volatile u32 *)arg6 = count2;
    return status;
}
