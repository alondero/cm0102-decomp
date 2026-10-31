#include "cmre.h"

/* Callees. cm_fn_0090cfc0 is cdecl with one int argument and returns int.
   cm_fn_00549f70 is thiscall with four stack arguments that it pops; it returns
   a pointer to at least eight bytes that this routine reads back. */
extern int cm_fn_0090cfc0(int param_1);
extern short *CM_THISCALL cm_fn_00549f70(void *self, s16 arg_2, s8 arg_3, u16 arg_4, u32 arg_5);

/* Record fields by byte offset from the record pointer `rec`. The records are
   43 (0x2b) bytes long, and `rec` is 0x16 bytes past the record start. */
#define REC_U8(rec, off) (*(volatile u8 *)((rec) + (u32)(off)))
#define REC_S8(rec, off) (*(volatile s8 *)((rec) + (u32)(off)))
#define REC_S16(rec, off) (*(volatile s16 *)((rec) + (u32)(off)))
#define REC_U32(rec, off) (*(volatile u32 *)((rec) + (u32)(off)))

void cm_fn_0079caa0(void)
{
    /* The callee object lives at the frame base plus 8 (eight bytes). */
    u32 obj[2];
    s32 n;
    u32 base;
    u32 rec;
    s32 i;

    n = CM_GLOBAL(volatile s32, 0x00ae2390);
    base = CM_GLOBAL(volatile u32, 0x00ae23e8);
    if (n <= 0) {
        return;
    }
    i = 0;
    rec = base + 0x16u;
    do {
        /* Block 1: when the dword at +0 is zero, recompute it from the
           table value at the element's +0x37 (through pointer +4, or +8). */
        if (REC_U32(rec, 0) == 0) {
            u32 x = REC_U32(rec, -0x12);
            s32 idx = (s32)REC_U32(x, 0x37);
            u32 b2;

            if (idx < 0) {
                u32 y = REC_U32(rec, -0xe);

                idx = (s32)REC_U32(y, 0x37);
                if (idx < 0) {
                    u32 g = CM_GLOBAL(volatile u32, 0x00ae2350);

                    idx = cm_fn_0090cfc0((int)g);
                    b2 = CM_GLOBAL(volatile u32, 0x00ae23a8);
                    REC_U32(rec, 0) = b2 + (u32)idx * 290u;
                } else {
                    b2 = CM_GLOBAL(volatile u32, 0x00ae23a8);
                    idx = (s32)REC_U32(y, 0x37);
                    REC_U32(rec, 0) = b2 + (u32)idx * 290u;
                }
            } else {
                idx = (s32)REC_U32(x, 0x37);
                b2 = CM_GLOBAL(volatile u32, 0x00ae23a8);
                REC_U32(rec, 0) = b2 + (u32)idx * 290u;
            }
        }

        /* Block 2: word at +0xc. */
        {
            s16 w = REC_S16(rec, 0xc);

            if (w <= 0 || w > 0xc8) {
                u32 d0 = REC_U32(rec, 0);

                if (d0 != 0) {
                    int res = cm_fn_0090cfc0(0x50);
                    u32 d0b = REC_U32(rec, 0);
                    u8 al = (u8)((u8)res + REC_U8(d0b, 0x8e));

                    al = (u8)(al + 1);
                    REC_S16(rec, 0xc) = (s16)(s8)al;
                } else {
                    int res = cm_fn_0090cfc0(0x50);
                    u8 al = (u8)(res + 1);

                    REC_S16(rec, 0xc) = (s16)(s8)al;
                }
            }
        }

        /* Block 3: word at +8. */
        {
            s16 w = REC_S16(rec, 0x8);

            if (w <= 0 || w > 0xc8) {
                u32 d0 = REC_U32(rec, 0);

                if (d0 != 0) {
                    int res = cm_fn_0090cfc0(0x50);
                    u32 d0b = REC_U32(rec, 0);
                    u8 cl = (u8)(REC_U8(d0b, 0x8e) << 2);
                    u8 al = (u8)((u8)res + cl);

                    al = (u8)(al + 1);
                    REC_S16(rec, 0x8) = (s16)(s8)al;
                } else {
                    int res = cm_fn_0090cfc0(0x50);
                    u8 al = (u8)(res + 1);

                    REC_S16(rec, 0x8) = (s16)(s8)al;
                }
            }
        }

        /* Block 4: word at +0xa, then clamped up to the word at +8. */
        {
            s16 w = REC_S16(rec, 0xa);

            if (w <= 0 || w > 0xc8) {
                u32 d0 = REC_U32(rec, 0);

                if (d0 != 0) {
                    int res = cm_fn_0090cfc0(0x50);
                    u32 d0b = REC_U32(rec, 0);
                    u8 dl = (u8)(REC_U8(d0b, 0x8e) << 2);
                    u8 al = (u8)((u8)res + dl);

                    al = (u8)(al + 1);
                    REC_S16(rec, 0xa) = (s16)(s8)al;
                } else {
                    int res = cm_fn_0090cfc0(0x50);
                    u8 al = (u8)(res + 1);

                    REC_S16(rec, 0xa) = (s16)(s8)al;
                }
            }
            if (REC_S16(rec, 0xa) < REC_S16(rec, 0x8)) {
                REC_S16(rec, 0xa) = REC_S16(rec, 0x8);
            }
        }

        /* Block 5: byte at +0xe. */
        {
            s8 b = REC_S8(rec, 0xe);

            if (b <= 0 || b > 0x14) {
                int res;
                u8 al;

                if (REC_S16(rec, 0x8) > 0x64) {
                    res = cm_fn_0090cfc0(0xc);
                    al = (u8)(res + 7);
                } else {
                    res = cm_fn_0090cfc0(0xa);
                    al = (u8)(res + 5);
                }
                REC_U8(rec, 0xe) = al;
            }
        }

        /* Block 6: byte at +0xf, chosen by the word at +8. */
        {
            s8 b = REC_S8(rec, 0xf);

            if (b <= 0 || b > 0x14) {
                s16 w = REC_S16(rec, 0x8);
                int res;
                u8 al;

                if (w > 0x78) {
                    res = cm_fn_0090cfc0(0xe);
                    al = (u8)(res + 7);
                } else if (w > 0x50) {
                    res = cm_fn_0090cfc0(0xf);
                    al = (u8)(res + 5);
                } else {
                    res = cm_fn_0090cfc0(0xa);
                    al = (u8)(res + 5);
                }
                REC_U8(rec, 0xf) = al;
            }
        }

        /* Block 7: byte at +0x10, chosen by the word at +8. */
        {
            s8 b = REC_S8(rec, 0x10);

            if (b <= 0 || b > 0x14) {
                s16 w = REC_S16(rec, 0x8);
                int res;
                u8 al;

                if (w > 0x78) {
                    res = cm_fn_0090cfc0(0xb);
                    al = (u8)(res + 0xa);
                } else if (w > 0x50) {
                    res = cm_fn_0090cfc0(0xf);
                    al = (u8)(res + 5);
                } else {
                    res = cm_fn_0090cfc0(0xa);
                    al = (u8)(res + 5);
                }
                REC_U8(rec, 0x10) = al;
            }
        }

        /* Block 8: byte at +0x11. */
        {
            s8 b = REC_S8(rec, 0x11);

            if (b <= 0 || b > 0x14) {
                int res;
                u8 al;

                if (REC_S16(rec, 0x8) > 0x50) {
                    res = cm_fn_0090cfc0(0xf);
                    al = (u8)(res + 6);
                } else {
                    res = cm_fn_0090cfc0(0x14);
                    al = (u8)(res + 1);
                }
                REC_U8(rec, 0x11) = al;
            }
        }

        /* Block 9: byte at +0x12. */
        {
            s8 b = REC_S8(rec, 0x12);

            if (b <= 0 || b > 0x14) {
                s16 w = REC_S16(rec, 0x8);
                int res;
                u8 al;

                if (w > 0x78) {
                    res = cm_fn_0090cfc0(0xa);
                    al = (u8)(res + 0xb);
                } else if (w > 0x50) {
                    res = cm_fn_0090cfc0(0xf);
                    al = (u8)(res + 5);
                } else {
                    res = cm_fn_0090cfc0(0xa);
                    al = (u8)(res + 7);
                }
                REC_U8(rec, 0x12) = al;
            }
        }

        /* Block 10: byte at +0x13. */
        {
            s8 b = REC_S8(rec, 0x13);

            if (b <= 0 || b > 0x14) {
                s16 w = REC_S16(rec, 0x8);
                int res;
                u8 al;

                if (w > 0x78) {
                    res = cm_fn_0090cfc0(0xd);
                    al = (u8)(res + 8);
                } else if (w > 0x50) {
                    res = cm_fn_0090cfc0(0xe);
                    al = (u8)(res + 7);
                } else {
                    res = cm_fn_0090cfc0(0xf);
                    al = (u8)(res + 6);
                }
                REC_U8(rec, 0x13) = al;
            }
        }

        /* Block 11: byte at +0x14. The second call's result takes +0xb when
           the first call's result is non-zero, otherwise +5. */
        {
            s8 b = REC_S8(rec, 0x14);

            if (b <= 0 || b > 0x14) {
                int r1 = cm_fn_0090cfc0(0x64);
                int r2 = cm_fn_0090cfc0(0xa);
                u8 al;

                if (r1 != 0) {
                    al = (u8)(r2 + 0xb);
                } else {
                    al = (u8)(r2 + 5);
                }
                REC_U8(rec, 0x14) = al;
            }
        }

        /* Block 12: thiscall on the callee object with arguments (1, 1, 0x776, -1).
           Its returned pointer is read as eight bytes; the first dword's low
           word is compared with the word at -8, then the word at -0xa. */
        {
            u32 q = (u32)cm_fn_00549f70((void *)obj, 1, 1, 0x776u, 0xffffffffu);
            u32 p0 = *(volatile u32 *)q;
            u16 dx;
            u16 cx;
            s16 ax;

            (void)*(volatile u32 *)(q + 4u);
            dx = (u16)(p0 >> 16);
            cx = (u16)p0;
            ax = REC_S16(rec, -0x8);

            if (dx > (u16)ax || (dx == (u16)ax && (s16)cx > REC_S16(rec, -0xa))) {
                int r1 = cm_fn_0090cfc0(0xa);

                REC_S16(rec, -0x8) = (s16)(u16)(r1 + 0x7a8);
                {
                    int r2 = cm_fn_0090cfc0(0x12c);

                    REC_S16(rec, -0xa) = (s16)(u16)(r2 + 1);
                }
            }
        }

        i++;
        rec += 0x2bu;
    } while (i < CM_GLOBAL(volatile s32, 0x00ae2390));
}
