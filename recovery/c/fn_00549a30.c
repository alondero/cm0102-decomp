#include "cmre.h"

/* First argument (EDX at entry), object fields by byte offset:
   0x57 dword, compared between the object and the second argument;
   0x80 signed word, the rating compared against thresholds;
   0xd7 base of a 50-entry dword pointer table, entry pointers at +0x61 are
   checked for non-zero, and each entry's dword at +0 is compared with the
   global limit at 0x00ae2364. Entries that pass are kept and sorted through
   cm_fn_00945333 with the comparator at 0x00549c30. */

extern void CM_CDECL cm_fn_00945333(u32 arg1, u32 arg2, u32 arg3, u32 arg4);

#define FIELD_U32(base, off) (*(volatile u32 *)((base) + (off)))
#define FIELD_S16(base, off) (*(volatile s16 *)((base) + (off)))
#define TABLE_S8(tbl, idx) (*(volatile s8 *)((tbl) + 6u * (idx) + 4u))

u8 CM_CDECL cm_fn_00549a30(u32 arg1, u32 arg2)
{
    /* Frame: scratch[0] is the running signed sum (frame +0x10, starts at 0);
       scratch[1..50] is the array of selected entry pointers (frame +0x14). */
    u32 scratch[51];
    u32 count;
    u32 probe;
    u32 entry;
    u32 k;
    u32 i;
    s32 limit;
    s32 bound;
    s32 iterations;
    s32 sum;
    s32 quotient;
    s32 w;
    u32 tbl;
    u32 idx;
    s16 ax;
    s16 cx;
    s16 dx;
    u8 result;

    if (arg1 == 0u) {
        return 3u;
    }

    scratch[0] = 0u;
    count = 0u;
    limit = (s32)FIELD_U32(0x00ae2364u, 0);
    probe = arg1 + 0xd7u;
    for (k = 0u; k < 50u; k++) {
        entry = *(volatile u32 *)probe;
        if (entry != 0u && FIELD_U32(entry, 0x61) != 0u && (s32)FIELD_U32(entry, 0) < limit) {
            scratch[1u + count] = entry;
            count++;
        }
        probe += 4u;
    }

    cm_fn_00945333((u32)&scratch[1], count, 4u, 0x00549c30u);

    bound = (s32)count - 1;
    iterations = 0;
    i = 0u;
    while ((s32)i < bound) {
        u32 record;
        u32 detail;
        record = scratch[1u + i];
        sum = (s32)scratch[0];
        detail = FIELD_U32(record, 0x61);
        w = FIELD_S16(detail, 5);
        sum = sum + w;
        iterations++;
        i++;
        scratch[0] = (u32)sum;
        if (i >= 16u) {
            break;
        }
    }

    if (iterations != 0) {
        quotient = (s32)scratch[0] / iterations;
        if (quotient > 0x9b) {
            result = 6u;
        } else if (quotient > 0x87) {
            result = 5u;
        } else if (quotient > 0x6e) {
            result = 4u;
        } else if (quotient > 0x55) {
            result = 3u;
        } else if (quotient > 0x3c) {
            result = 2u;
        } else {
            result = (u8)(quotient > 0x28);
        }
    } else {
        ax = FIELD_S16(arg1, 0x80);
        if (ax > 0x1c52) {
            result = 6u;
        } else if (ax > 0x1676) {
            result = 5u;
        } else if (ax > 0x128e) {
            result = 4u;
        } else if (ax > 0xcb2) {
            result = 3u;
        } else {
            result = (u8)((ax > 0x8ca) + 1);
        }
    }

    if (arg2 == 0u) {
        goto done;
    }
    cx = FIELD_S16(arg2, 0x80);
    if (cx > 0x1c52) {
        dx = FIELD_S16(arg1, 0x80);
        if (dx < 0x109a && (s8)result > 2) {
            result = (u8)(result - 2u);
        }
        if (dx < 0x1c52 && (s8)result > 0) {
            result = (u8)(result - 1u);
        }
    } else {
        if (cx < 0x109a && (s8)result < 5) {
            result = (u8)(result + 1u);
        }
    }

    if (FIELD_U32(arg2, 0x57) != FIELD_U32(arg1, 0x57)) {
        goto done;
    }

    cx = (s16)(cx - FIELD_S16(arg1, 0x80));
    tbl = FIELD_U32(0x00ae2d14u, 0);

    if (cx > 0x4e2) {
        if ((s8)result > 0) {
            idx = FIELD_U32(arg2, 0);
            if (TABLE_S8(tbl, idx) >= 0) {
                idx = FIELD_U32(arg1, 0);
                if (TABLE_S8(tbl, idx) <= 0) {
                    result = (u8)(result - 1u);
                    goto done;
                }
            }
        }
    }

    if (cx >= 0x4e2) {
        goto done;
    }
    if ((s8)result >= 5) {
        goto done;
    }
    idx = FIELD_U32(arg1, 0);
    if (TABLE_S8(tbl, idx) < 0) {
        goto done;
    }
    idx = FIELD_U32(arg2, 0);
    if (TABLE_S8(tbl, idx) > 0) {
        goto done;
    }
    result = (u8)(result + 1u);

done:
    if ((s8)result < 0) {
        result = 0u;
    }
    if ((s8)result > 6) {
        result = 6u;
    }
    return result;
}
