#include "cmre.h"

/* Table 1: base pointer read from global 0x00ae23b4, element stride 0x245,
   outer count from global 0x00ae235c (re-read at the outer loop tail).
   Record-pointer table at base + 0xd7, 4-byte entries indexed by the values
   returned from cm_fn_0090cfc0(0x32). Each record: +0x61 pointer to a
   sub-object; +0x18 signed byte; +0x22 byte flag.
   Sub-object: +0x05 and +0x07 16-bit values, +0x11 byte, +0x1b, +0x2b, +0x2e,
   +0x36, +0x3c bytes.
   Table 2: base pointer from global 0x00ae23bc, element stride 0x6e, count
   from global 0x00ae2364; each element's +0x61 pointer gates two calls. */

#define TABLE_U32(base, index) (*(volatile u32 *)((base) + (u32)(index) * 4u + 0xd7u))
#define SUB_PTR(obj) (*(volatile u32 *)((obj) + 0x61u))
#define FIELD_U8(obj, off) (*(volatile u8 *)((obj) + (u32)(off)))
#define FIELD_S8(obj, off) (*(volatile s8 *)((obj) + (u32)(off)))
#define FIELD_U16(obj, off) (*(volatile u16 *)((obj) + (u32)(off)))
#define FIELD_S16(obj, off) (*(volatile s16 *)((obj) + (u32)(off)))

extern s32 CM_CDECL cm_fn_0090cfc0(s32 index);
extern void CM_CDECL cm_fn_00538ec0(u32 object);
extern void CM_CDECL cm_fn_00538bc0(u32 object);

void CM_CDECL cm_fn_00533710(void)
{
    s32 count;
    s32 outer;
    s32 inner;
    u32 base;
    s32 pick_a;
    s32 pick_b;
    u32 rec_a;
    u32 rec_b;
    u32 p_a;
    u32 p_b;
    u32 p1;
    u32 p2;
    u32 t1;
    u32 t2;
    s32 a_val;
    s32 c_val;
    s32 delta;
    s32 magnitude;
    s32 take;
    u8 low_a;
    u8 low_b;
    u16 w_a;
    u16 w_b;
    u16 w_c;
    u16 w_d;
    u32 object;
    s32 index2;
    s32 count2;

    count = CM_GLOBAL(volatile s32, 0x00ae235c);
    base = (u32)CM_GLOBAL(volatile s32, 0x00ae23b4);
    outer = 0;
    if (count > 0) {
        do {
            inner = 100;
            do {
                pick_a = cm_fn_0090cfc0(0x32);
                rec_a = TABLE_U32(base, pick_a);
                pick_b = cm_fn_0090cfc0(0x32);
                rec_b = TABLE_U32(base, pick_b);
                if (rec_a != 0) {
                    p_a = SUB_PTR(rec_a);
                    if (p_a != 0 && rec_b != 0) {
                        p_b = SUB_PTR(rec_b);
                        if (p_b != 0) {
                            take = 1;
                            if (!(FIELD_U8(rec_a, 0x22) == 0 && FIELD_U8(rec_b, 0x22) == 0)) {
                                a_val = FIELD_S16(p_b, 5);
                                delta = FIELD_S16(p_a, 5) - a_val;
                                magnitude = delta < 0 ? -delta : delta;
                                take = magnitude <= 10;
                            }
                            if (take) {
                                c_val = FIELD_S8(rec_b, 0x18);
                                delta = FIELD_S8(rec_a, 0x18) - c_val;
                                magnitude = delta < 0 ? -delta : delta;
                                take = magnitude <= 4;
                            }
                            if (take) {
                                if (cm_fn_0090cfc0(2) != 0) {
                                    /* 0x5337d5: slot a and its +0x61, then slot b and its +0x61. */
                                    p1 = SUB_PTR(TABLE_U32(base, pick_a));
                                    p2 = SUB_PTR(TABLE_U32(base, pick_b));
                                    low_a = FIELD_U8(p1, 0x36);
                                    low_b = FIELD_U8(p2, 0x36);
                                    FIELD_U8(p1, 0x36) = low_b;
                                    p2 = SUB_PTR(TABLE_U32(base, pick_b));
                                    FIELD_U8(p2, 0x36) = low_a;
                                    /* 0x5337ff: both slots (b, a), then both +0x61 (b, a). */
                                    t2 = TABLE_U32(base, pick_b);
                                    t1 = TABLE_U32(base, pick_a);
                                    p2 = SUB_PTR(t2);
                                    p1 = SUB_PTR(t1);
                                    low_b = FIELD_U8(p2, 0x1b);
                                    low_a = FIELD_U8(p1, 0x1b);
                                    FIELD_U8(p1, 0x1b) = low_b;
                                    p2 = SUB_PTR(TABLE_U32(base, pick_b));
                                    FIELD_U8(p2, 0x1b) = low_a;
                                }
                                if (cm_fn_0090cfc0(2) != 0) {
                                    /* 0x533837 */
                                    t2 = TABLE_U32(base, pick_b);
                                    t1 = TABLE_U32(base, pick_a);
                                    p2 = SUB_PTR(t2);
                                    p1 = SUB_PTR(t1);
                                    low_b = FIELD_U8(p2, 0x3c);
                                    low_a = FIELD_U8(p1, 0x3c);
                                    FIELD_U8(p1, 0x3c) = low_b;
                                    p2 = SUB_PTR(TABLE_U32(base, pick_b));
                                    FIELD_U8(p2, 0x3c) = low_a;
                                }
                                p1 = SUB_PTR(TABLE_U32(base, pick_a));
                                if (FIELD_U8(p1, 0x11) != 0x14) {
                                    if (cm_fn_0090cfc0(2) != 0) {
                                        p1 = SUB_PTR(TABLE_U32(base, pick_a));
                                        p2 = SUB_PTR(TABLE_U32(base, pick_b));
                                        low_a = FIELD_U8(p1, 0x2e);
                                        low_b = FIELD_U8(p2, 0x2e);
                                        FIELD_U8(p1, 0x2e) = low_b;
                                        p2 = SUB_PTR(TABLE_U32(base, pick_b));
                                        FIELD_U8(p2, 0x2e) = low_a;
                                        /* 0x5338a9 */
                                        t2 = TABLE_U32(base, pick_b);
                                        t1 = TABLE_U32(base, pick_a);
                                        p2 = SUB_PTR(t2);
                                        p1 = SUB_PTR(t1);
                                        low_b = FIELD_U8(p2, 0x2b);
                                        low_a = FIELD_U8(p1, 0x2b);
                                        FIELD_U8(p1, 0x2b) = low_b;
                                        p2 = SUB_PTR(TABLE_U32(base, pick_b));
                                        FIELD_U8(p2, 0x2b) = low_a;
                                    }
                                }
                                /* 0x5338d3: every store reloads the slot pointers first. */
                                t2 = TABLE_U32(base, pick_b);
                                t1 = TABLE_U32(base, pick_a);
                                p2 = SUB_PTR(t2);
                                p1 = SUB_PTR(t1);
                                w_b = FIELD_U16(p2, 5);
                                w_a = FIELD_U16(p1, 7);
                                w_c = FIELD_U16(p1, 5);
                                FIELD_U16(p1, 5) = w_b;
                                t2 = TABLE_U32(base, pick_b);
                                t1 = TABLE_U32(base, pick_a);
                                p2 = SUB_PTR(t2);
                                p1 = SUB_PTR(t1);
                                w_d = FIELD_U16(p2, 7);
                                FIELD_U16(p1, 7) = w_d;
                                p2 = SUB_PTR(TABLE_U32(base, pick_b));
                                FIELD_U16(p2, 5) = w_c;
                                p2 = SUB_PTR(TABLE_U32(base, pick_b));
                                FIELD_U16(p2, 7) = w_a;
                            }
                        }
                    }
                }
                inner = inner - 1;
            } while (inner != 0);
            outer = outer + 1;
            base = base + 0x245u;
        } while (outer < CM_GLOBAL(volatile s32, 0x00ae235c));
    }

    count2 = CM_GLOBAL(volatile s32, 0x00ae2364);
    object = (u32)CM_GLOBAL(volatile s32, 0x00ae23bc);
    index2 = 0;
    if (count2 > 0) {
        do {
            if (SUB_PTR(object) != 0) {
                cm_fn_00538ec0(object);
                cm_fn_00538bc0(object);
            }
            count2 = CM_GLOBAL(volatile s32, 0x00ae2364);
            index2 = index2 + 1;
            object = object + 0x6eu;
        } while (index2 < count2);
    }
}
