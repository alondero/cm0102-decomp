#include "cmre.h"

/* Callee at 0x00945333: cdecl, four arguments (caller pops them with add esp, 0x10). */
extern void CM_CDECL cm_fn_00945333(u32 a0, u32 a1, u32 a2, u32 a3);

/* Record table: the global at 0x00ae23bc is the base of 0x6e-byte records.
   Element records are 0x15 bytes apart. Offsets are the only names used. */
#define RD_U8(addr) (*(volatile u8 *)(addr))
#define RD_S8(addr) (*(volatile s8 *)(addr))
#define RD_U16(addr) (*(volatile u16 *)(addr))
#define RD_U32(addr) (*(volatile u32 *)(addr))
#define RD_S32(addr) (*(volatile s32 *)(addr))
#define TBL_REC(tbl, x, off) ((tbl) + 0x6eu * (x) + (off))

/* Arguments, by stack offset from the first argument (entry esp + 4):
     arg_20  array of u32 record pointers, filled by this routine
     arg_24  s16 limit on the number of records kept
     arg_28  s32 test on record dword +4 (-1 disables)
     arg_2c  s32 test on the table record's sub-object dword +0x53 (-1 disables)
     arg_30  s32 reject when record dword +4 equals it (-1 disables)
     arg_34  s32 test on +0x53 and on record dword +4 (-1 disables)
     arg_38  s32 test on the table record's sub-object dword +0x57 (-1 disables)
     arg_3c  u8 mode selector for the initial call (0..5)
     arg_40  s8 threshold on the table record's byte +0x18 (-1 disables)
   Returns the number of kept records as a 16-bit value (ret 0x24). */
u16 CM_THISCALL cm_fn_00931ae0(u32 self, u32 arg_20, s16 arg_24, s32 arg_28,
                               s32 arg_2c, s32 arg_30, s32 arg_34, s32 arg_38,
                               u8 arg_3c, s8 arg_40)
{
    s16 level;
    s32 n_out;
    u32 out;
    s32 edi_out;
    u32 tbl;
    u32 rec_off;
    s32 counter;
    s32 n;
    s32 limit;
    s32 i;
    u32 E;
    u32 x;
    u32 P;
    u32 Q;
    u32 first;
    u32 second;
    s16 lvl;

    n_out = 0;
    edi_out = 0;
    level = 0x19;
    if (CM_GLOBAL(volatile u16, 0x00ae2c92) == 0x7d1 && RD_U16(self + 0xc) > 0x168) {
        level = 0xf;
    }

    if (arg_3c == 0) {
        second = RD_U32(self + 4);
        first = RD_U32(self);
        cm_fn_00945333(first, second, 0x15u, 0x00931f90u);
    } else if (arg_3c == 1) {
        second = RD_U32(self + 4);
        first = RD_U32(self);
        cm_fn_00945333(first, second, 0x15u, 0x009320e0u);
    } else if (arg_3c == 2) {
        second = RD_U32(self + 4);
        first = RD_U32(self);
        cm_fn_00945333(first, second, 0x15u, 0x00932230u);
    } else if (arg_3c == 3) {
        second = RD_U32(self + 4);
        first = RD_U32(self);
        cm_fn_00945333(first, second, 0x15u, 0x009323b0u);
    } else if (arg_3c == 4) {
        second = RD_U32(self + 4);
        first = RD_U32(self);
        cm_fn_00945333(first, second, 0x15u, 0x00932500u);
    } else if (arg_3c == 5) {
        second = RD_U32(self + 4);
        first = RD_U32(self);
        cm_fn_00945333(first, second, 0x15u, 0x00932590u);
    }

    if (level > 0) {
        tbl = CM_GLOBAL(volatile u32, 0x00ae23bc);
        for (;;) {
            n = RD_S32(self + 4);
            counter = 0;
            if (n > 0) {
                limit = (s32)arg_24;
                out = arg_20 + (u32)n_out * 4u;
                rec_off = 0;
                for (;;) {
                    if (n_out >= limit) break;
                    if (counter >= n) return RD_U16(self + 4);

                    first = RD_U32(self);
                    lvl = (s16)RD_S8(first + rec_off + 0xe);
                    E = first + rec_off;
                    if (lvl < level) goto next_rec;

                    /* Skip the record if it is already in the output list. */
                    if (limit > 0) {
                        for (i = 0; i < limit; i++) {
                            if (RD_U32(arg_20 + (u32)i * 4u) == E) goto next_rec;
                        }
                    }

                    if (arg_28 != -1 && RD_S32(E + 4) != arg_28) goto next_rec;

                    if (arg_2c != -1) {
                        x = RD_U32(E);
                        P = RD_U32(TBL_REC(tbl, x, 0x39));
                        if (P == 0) goto next_rec;
                        Q = RD_U32(P + 0x53);
                        if (Q == 0) goto next_rec;
                        if (RD_U32(Q) != (u32)arg_2c) goto next_rec;
                    }

                    if (arg_30 != -1 && RD_S32(E + 4) == arg_30) goto next_rec;

                    if (arg_34 != -1) {
                        x = RD_U32(E);
                        P = RD_U32(TBL_REC(tbl, x, 0x39));
                        if (P == 0) goto next_rec;
                        Q = RD_U32(P + 0x53);
                        if (Q == 0) goto next_rec;
                        if (RD_U32(Q) != (u32)arg_34) goto next_rec;
                        if (RD_S32(E + 4) != arg_34) goto next_rec;
                    }

                    if (arg_40 != -1) {
                        x = RD_U32(E);
                        if (RD_S8(TBL_REC(tbl, x, 0x18)) > arg_40) goto next_rec;
                    }

                    if (arg_38 != -1) {
                        x = RD_U32(E);
                        P = RD_U32(TBL_REC(tbl, x, 0x39));
                        if (P == 0) goto next_rec;
                        Q = RD_U32(P + 0x57);
                        if (Q == 0) goto next_rec;
                        if (RD_U32(Q) != (u32)arg_38) goto next_rec;
                    }

                    /* Keep the record. */
                    RD_U32(out) = E;
                    tbl = CM_GLOBAL(volatile u32, 0x00ae23bc);
                    out = out + 4u;
                    n_out = n_out + 1;

next_rec:
                    counter = counter + 1;
                    rec_off = rec_off + 0x15u;
                    n = RD_S32(self + 4);
                    if (counter >= n) break;
                }
                edi_out = n_out;
            }

            if (edi_out == (s32)arg_24) break;
            level = (s16)(level - 5);
            if (level <= 0) break;
        }
    }

    return (u16)edi_out;
}
