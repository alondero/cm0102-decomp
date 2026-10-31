#include "cmre.h"

/* Object arg1 (P) fields by byte offset: 0x1c pointer to an s16 array (the
   "next" links, indexed by node), 0x28 pointer to a u16 array whose low byte
   bit 0x20 is a per-node flag, 0x3cc pointer to a u32 array (T).
   Object arg2 (Q) fields: 0x00 s16 count, 0x04 pointer to s16 array (lo),
   0x08 pointer to s16 array (hi), 0x1c pointer to a u32 array (W). */

#define RD_S16(a) (*(volatile s16 *)(a))
#define RD_U16(a) (*(volatile u16 *)(a))
#define RD_U8(a) (*(volatile u8 *)(a))
#define RD_U32(a) (*(volatile u32 *)(a))
#define WR_U32(a, v) (*(volatile u32 *)(a) = (v))

#define FLAG_BIT 0x20u
#define DIFF_LIMIT 0x2000

extern s32 CM_CDECL cm_fn_009446e0(s32 a, s32 b);
extern u32 CM_CDECL cm_fn_00944840(s32 a, s32 b);

void CM_CDECL cm_fn_009347a0(u32 arg1, u32 arg2)
{
    u32 arr_w_base;
    u32 flag_base;
    s32 k;
    s32 x;
    s32 y;
    s32 j;
    s32 jcur;
    s32 jstart;
    s32 t;
    s32 r;
    s32 cnt;
    u32 p2;
    u32 tbl_t;
    u32 tbl_w;
    u32 tj;
    u32 wj;
    u32 tr;
    u32 den;
    u32 d;
    u32 wr;
    u32 rr;
    u32 dw;

    arr_w_base = RD_U32(arg1 + 0x1c);
    flag_base = RD_U32(arg1 + 0x28);
    if (RD_S16(arg2) <= 0) {
        return;
    }

    k = 0;
    for (;;) {
        x = RD_S16(RD_U32(arg2 + 0x08) + (u32)k * 2u);
        y = RD_S16(RD_U32(arg2 + 0x04) + (u32)k * 2u);

        if (x > y) {
            j = y;
            if ((RD_U8(flag_base + (u32)j * 2u) & FLAG_BIT) == 0) {
                do {
                    if (j > x) {
                        goto next_k;
                    }
                    j = j + 1;
                } while ((RD_U8(flag_base + (u32)j * 2u) & FLAG_BIT) == 0);
            }
            if (j > x) {
                goto next_k;
            }

            /* [esp+0x24] keeps the first flagged node; the walk ends when the
               link chain returns to it (0x00934921). */
            jstart = j;
            do {
                jcur = j;
                p2 = arr_w_base + (u32)j * 2u;

                /* Follow the links from arr[j] until a flagged node is reached. */
                r = RD_S16(p2);
                while ((RD_U8(flag_base + (u32)r * 2u) & FLAG_BIT) == 0) {
                    r = RD_S16(arr_w_base + (u32)r * 2u);
                }

                tbl_t = RD_U32(arg1 + 0x3cc);
                tbl_w = RD_U32(arg2 + 0x1c);
                tj = RD_U32(tbl_t + (u32)jcur * 4u);
                wj = RD_U32(tbl_w + (u32)jcur * 4u);
                tr = RD_U32(tbl_t + (u32)r * 4u);
                den = tj - tr;

                if (den == 0) {
                    t = RD_S16(p2);
                    dw = wj - tj;
                    if (t != r) {
                        do {
                            u32 wbase = RD_U32(arg2 + 0x1c);
                            u32 wv = RD_U32(wbase + (u32)t * 4u);
                            WR_U32(wbase + (u32)t * 4u, wv + dw);
                            t = RD_S16(arr_w_base + (u32)t * 2u);
                        } while (t != r);
                    }
                    goto done;
                }

                wr = RD_U32(tbl_w + (u32)r * 4u);
                d = wj - wr;
                if ((s32)d > DIFF_LIMIT || (s32)d < -DIFF_LIMIT) {
                    u32 call_res;
                    call_res = cm_fn_00944840((s32)d, (s32)den);
                    rr = call_res;
                    t = RD_S16(p2);
                    if (t != r) {
                        do {
                            u32 tt = RD_U32(RD_U32(arg1 + 0x3cc) + (u32)t * 4u);
                            s32 a = (s32)(tt - tj);
                            s32 res = cm_fn_009446e0(a, (s32)rr);
                            u32 wbase = RD_U32(arg2 + 0x1c);
                            WR_U32(wbase + (u32)t * 4u, (u32)res + wj);
                            t = RD_S16(arr_w_base + (u32)t * 2u);
                        } while (t != r);
                    }
                    goto done;
                }

                t = RD_S16(p2);
                if (t != r) {
                    do {
                        u32 tt = RD_U32(RD_U32(arg1 + 0x3cc) + (u32)t * 4u);
                        s32 prod = (s32)((tt - tj) * d);
                        s32 q = prod / (s32)den;
                        u32 wbase = RD_U32(arg2 + 0x1c);
                        WR_U32(wbase + (u32)t * 4u, (u32)q + wj);
                        t = RD_S16(arr_w_base + (u32)t * 2u);
                    } while (t != r);
                }

            done:
                j = r;
            } while (r != jstart);
        }

    next_k:
        cnt = RD_S16(arg2);
        k = k + 1;
        if (!(k < cnt)) {
            break;
        }
    }
}
