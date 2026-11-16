/* fn_006beeb0 - preferred-image VA 0x006beeb0 (image base 0x00400000).
   Plain C reconstruction from the disassembly listing.

   Boundary notes (from the listing):
     - every CM_THISCALL callee takes `self` in ECX (the listing loads ECX
       before each call) and the listed push order is the argument order;
     - 0x006cda20 / 0x006d6f70 / 0x00717410 set ECX from the listing too;
     - 0x0090cfc0 is called with one pushed dword and `add esp, 4` (cdecl);
     - float arguments the listing pushes as raw dwords are declared u32 so
       the exact bit pattern crosses the boundary;
     - `call 0x00945640` is __ftol (float -> 64-bit integer), written as a
       cast through `long long` of the long double the x87 sequence builds.
     - [esp+..] locals are named after their listing slot when they are
       reloaded with a different width later. */

#include "cmre.h"

extern u32  CM_THISCALL cm_fn_006c0c60(u32 self, u32 arg);
extern void CM_THISCALL cm_fn_006c06b0(u32 self, u32 index, u32 value);
extern void CM_THISCALL cm_fn_006c07a0(u32 self, u32 index, u32 value);
extern void CM_THISCALL cm_fn_006c0890(u32 self, u32 index, u32 value);
extern void CM_THISCALL cm_fn_006c0980(u32 self, u32 index, u32 value);
extern void CM_THISCALL cm_fn_006c0a70(u32 self, u32 index, u32 value);
extern void CM_THISCALL cm_fn_006c0b70(u32 self, u32 index, u32 value);
extern u8   CM_THISCALL cm_fn_006c2050(u32 self, u32 factor_bits, u32 mode);
extern u8   CM_THISCALL cm_fn_006c2310(u32 self, s8 slot, u8 mode);
extern u8   CM_THISCALL cm_fn_006c23f0(u32 self, u32 factor_bits, u32 mode);
extern s8   CM_THISCALL cm_fn_006c2750(u32 self, u32 factor_bits);
extern u8   CM_THISCALL cm_fn_006c2880(u32 self, u32 argument);
extern void CM_THISCALL cm_fn_006cda20(u32 self, char *param_1, u32 param_2);
extern u32  CM_THISCALL cm_fn_006d0980(u32 self, u32 first, u32 second,
                                       u32 item, u32 other);
extern void CM_THISCALL cm_fn_006d6f70(u32 self, u32 a, u32 b, u32 c, u32 d,
                                       s32 e, s32 f, u32 g);
extern u16  CM_THISCALL cm_fn_00717410(void *self);
extern u32  CM_CDECL cm_fn_0090cfc0(u32 bound);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);

#define B(p) (*(volatile u8 *)(u32)(p))
#define H(p) (*(volatile u16 *)(u32)(p))
#define W(p) (*(volatile u32 *)(u32)(p))

/* 0x0090cfc0: one pushed bound, result in EAX. */
static u32 rnd(u32 bound)
{
    return (u32)cm_fn_0090cfc0(bound);
}

/* 0x006beefa..0x006bf074, four identical blocks:
     fild a ; fild b ; fld st(0) ; fmul [0x968968] ; fstp double m1
     a < m1 (C0 set)            -> a = ftol(m1)
     a >= m1 and a >= 2*b       -> a = ftol(2*b)
   ftol is 0x00945640. */
static void clamp16(u8 *s, u32 aoff, u32 boff)
{
    long double a;
    double b, m1, m2;

    a = (long double)(s16)H(s + aoff);
    b = (double)(s16)H(s + boff);
    m1 = (double)((long double)b * (long double)CM_GLOBAL(double, 0x968968));
    if (!(a >= (long double)m1)) {
        H(s + aoff) = (u16)(long long)m1;
    } else {
        m2 = b + b;
        if (a >= (long double)m2)
            H(s + aoff) = (u16)(long long)m2;
    }
}

void CM_THISCALL cm_fn_006beeb0(void *self)
{
    u8 *s = (u8 *)self;
    u8 fail13 = 0;      /* [esp+0x13] */
    u8 inner = 0;       /* [esp+0x14] */
    u8 flag15 = 0;      /* [esp+0x15] */
    u8 outer = 0;       /* [esp+0x16] */
    u8 pick = 0;        /* [esp+0x17] */
    u8 side = 0;        /* [esp+0x18] */
    u8 flag20 = 0;      /* [esp+0x20] */
    u8 flag28 = 0;      /* [esp+0x28] */
    u8 flagd = 0;       /* L+0x0c */
    u8 r23 = 0;         /* L+0x04 (6c23f0 result) */
    u8 c0 = 0;
    u32 ix = 0;
    u32 oth = 0;
    u32 d = 0;          /* L+0x14 (rnd difference) */
    u32 d20 = 0;        /* L+0x20 */
    u32 dres = 0;       /* L+0x1c */
    u32 o = 0;
    u32 r1 = 0, r2 = 0;
    s16 w26 = 0;
    u16 ax = 0;
    long double prev = 0;
    u8 *ent = s;

    H(s + 0x8ed0) = 0;
    H(s + 0x8ed4) = 0x1ef;

lbl_beeda:
    cm_fn_006c0c60((u32)s, 0);
    cm_fn_006c0c60((u32)s, 1);

    clamp16(s, 0x1da, 0x1dc);
    clamp16(s, 0x1de, 0x1e0);
    clamp16(s, 0x1e2, 0x1e4);
    clamp16(s, 0x1e6, 0x1e8);

    pick = (u8)rnd(2);
    outer = 0;

lbl_6bf088:
    if (B(s + 0x1a8) != 0)
        goto lbl_6bfdfd;
    if (pick != 0)
        side = outer;
    else
        side = (u8)(outer == 0);
    inner = 0;

lbl_6bf0b4:
    if ((s8)inner < 2)
        goto lbl_6bf104;
    oth = (u32)(side == 0);
    r1 = rnd((u32)(s16)H(s + 2u * side + 0x1de) +
             (u32)(s16)H(s + 2u * side + 0x1da));
    r2 = rnd((u32)(s16)H(s + 2u * oth + 0x1da) +
             (u32)(s16)H(s + 2u * oth + 0x1de));
    if ((s32)r1 <= (s32)r2)
        goto lbl_6bf1e7;

lbl_6bf104:
    {
        u32 att = 0;
        u32 r = 0;
        u32 k = 0;
        u8 *bp;

        ix = (u32)(s32)(s8)side;
        oth = (u32)(side == 0);
        bp = s + ix * 54u;
        H(bp + 0xf5e2) = (u16)(H(bp + 0xf5e2) + 1);

        for (;;) {
            r = rnd(0x14);
            att = (u8)(att + 1);
            k = (u32)(s32)(s8)(u8)r + ix * 20u;
            if ((s8)B(s + 446u * k + 0x47af) >= 0)
                break;
            if ((s8)(u8)att >= 0x78)
                goto lbl_6bf1fa;
        }
        if ((s8)(u8)att >= 0x78)
            goto lbl_6bf1fa;

        ent = s + 446u * k;
        {
            s32 ao = (s32)(s16)H(s + 2u * oth + 0x1da);
            float sum = (float)((long double)*(float *)(ent + 0x485b) +
                                (long double)CM_GLOBAL(float, 0x96afb0));
            u32 rr = rnd((u32)((ao + 200) / 19));

            if ((long double)(s32)rr >= (long double)sum) {
                H(ent + 0x47cb) = (u16)(H(ent + 0x47cb) - 0x1e);
            } else {
                H(bp + 0xf5e4) = (u16)(H(bp + 0xf5e4) + 1);
                H(ent + 0x47cb) = (u16)(H(ent + 0x47cb) + 0x28);
            }
        }
    }

lbl_6bf1e7:
    inner = (u8)(inner + 1);
    if ((s8)inner >= 3)
        goto lbl_6bf203;
    goto lbl_6bf0b4;

lbl_6bf1fa:
    fail13 = 1;
    goto lbl_6bf203;

lbl_6bf203:
    oth = (u32)(side == 0);
    ix = (u32)(s32)(s8)side;
    r1 = rnd((u32)(s16)H(s + 2u * side + 0x1de) +
             (u32)(s16)H(s + 2u * side + 0x1da));
    r2 = rnd((u32)(s16)H(s + 2u * oth + 0x1da) +
             (u32)(s16)H(s + 2u * oth + 0x1de));
    if ((s32)r1 <= (s32)r2)
        goto lbl_6bf2f0;
    H(s + ix * 54u + 0xf5e6) = (u16)(H(s + ix * 54u + 0xf5e6) + 1);
    {
        u8 res = (u8)cm_fn_006c2880((u32)s, (u32)side);
        if (res == 0xff)
            goto lbl_6bfd3b;
        {
            u32 k = (u32)(s32)(s8)res + ix * 20u;
            s32 bo = (s32)(s16)H(s + 2u * oth + 0x1de);
            float sum;
            u32 rr;

            ent = s + 446u * k;
            sum = (float)((long double)*(float *)(ent + 0x4873) +
                          (long double)CM_GLOBAL(float, 0x96b020));
            rr = rnd((u32)(bo / 5));
            if ((long double)(s32)rr >= (long double)sum) {
                H(ent + 0x47cb) = (u16)(H(ent + 0x47cb) - 0xa);
            } else {
                H(ent + 0x47cb) = (u16)(H(ent + 0x47cb) + 0x28);
                H(s + ix * 54u + 0xf5e8) =
                    (u16)(H(s + ix * 54u + 0xf5e8) + 1);
            }
        }
    }

lbl_6bf2f0:
    r1 = rnd((u32)(s16)H(s + 2u * side + 0x1de) +
             (u32)(s16)H(s + 2u * side + 0x1da));
    r2 = rnd((u32)(s16)H(s + 2u * oth + 0x1da) +
             (u32)(s16)H(s + 2u * oth + 0x1de));
    if ((s32)r1 > (s32)r2) {
        u32 cnt = 0;
        u32 r3 = 0;
        u32 k3 = 0;

        H(s + ix * 54u + 0xf5ea) = (u16)(H(s + ix * 54u + 0xf5ea) + 1);
        for (;;) {
            r3 = rnd(0x14);
            cnt = (u8)(cnt + 1);
            k3 = (u32)(s32)(s8)(u8)r3 + ix * 20u;
            if ((s8)B(s + 446u * k3 + 0x47af) >= 0)
                break;
            if ((s8)(u8)cnt >= 0x78)
                goto lbl_6bfd3b;
        }
        if ((s8)(u8)cnt >= 0x78)
            goto lbl_6bfd3b;
        {
            s32 bo = (s32)(s16)H(s + 2u * oth + 0x1f2);
            float sum;
            u32 rr;

            ent = s + 446u * k3;
            sum = (float)((long double)*(float *)(ent + 0x4843) +
                          (long double)CM_GLOBAL(float, 0x96b020));
            rr = rnd((u32)((bo + 100) / 7));
            if ((long double)(s32)rr >= (long double)sum) {
                H(ent + 0x47cb) = (u16)(H(ent + 0x47cb) - 8);
            } else {
                H(s + ix * 54u + 0xf5ec) =
                    (u16)(H(s + ix * 54u + 0xf5ec) + 1);
                H(ent + 0x47cb) = (u16)(H(ent + 0x47cb) + 0x14);
            }
        }
    }

    if (rnd(10) == 0)
        B(s + ix * 54u + 0xf5c8) = (u8)(B(s + ix * 54u + 0xf5c8) + 1);

    /* 0x006bf43a */
    {
        s8 pr = (s8)cm_fn_006c2750((u32)s, (u32)side);
        u32 k = (u32)(s32)pr + ix * 20u;
        long double t;
        u32 p1, p2;

        ent = s + 446u * k;
        t = (long double)*(float *)(ent + 0x484f) +
            (long double)CM_GLOBAL(float, 0x96afb0);
        p1 = rnd((u32)(long long)t);
        p2 = rnd(2);
        if ((s32)p1 < (s32)p2) {
            H(ent + 0x47cb) = (u16)(H(ent + 0x47cb) - 0xf);
            B(s + ix * 54u + 0xf5ca) = (u8)(B(s + ix * 54u + 0xf5ca) + 1);
        }
    }

    /* 0x006bf4b6 */
    flag20 = 0;
    flag28 = 0;
    r1 = rnd((u32)(s16)H(s + 2u * oth + 0x1ea));
    r2 = rnd(0x640);
    if ((s32)r1 <= (s32)r2) {
        r1 = rnd((u32)(s16)H(s + 2u * side + 0x1de));
        r2 = rnd(0xbb8);
        if ((s32)r1 > (s32)r2) {
            flag28 = 1;
            B(s + ix * 54u + 0xf5c7) = (u8)(B(s + ix * 54u + 0xf5c7) + 1);
        }
        goto lbl_6bf720;
    }

    d20 = (u32)(s + oth * 54u);
    B(d20 + 0xf5c9) = (u8)(B(d20 + 0xf5c9) + 1);
    r1 = rnd((u32)(s16)H(s + 2u * oth + 0x1ea));
    r2 = rnd(0x61a8);
    if ((s32)r1 > (s32)r2) {
        /* 0x006bf51e severe incident, slot mode 4 */
        s8 slot;
        u8 bm = (u8)(side == 0);

        flagd = bm;
        slot = (s8)cm_fn_006c2310((u32)s, (s8)bm, 4);
        {
            u32 k = (u32)(s32)slot + oth * 20u;

            ent = s + 446u * k;
            H(ent + 0x47cb) = (u16)(H(ent + 0x47cb) - 0xfa);
            B(ent + 0x47b9) = (u8)(B(ent + 0x47b9) + 1);
            B(d20 + 0xf5cb) = (u8)(B(d20 + 0xf5cb) + 1);
            cm_fn_006cda20((u32)s, (char *)(ent + 0x4796), 1);
            cm_fn_006c0c60((u32)s, (u32)flagd);
            cm_fn_006d6f70(W(s + 0x8f7a), 0x1ff8, (u32)s, (u32)(u8)slot,
                           (u32)flagd, -1, -1, 0);
        }
    } else {
        r1 = rnd((u32)(s16)H(s + 2u * oth + 0x1ea));
        r2 = rnd(0x6a4);
        if ((s32)r1 > (s32)r2) {
            /* 0x006bf5c0 lesser incident, slot mode 2 */
            s8 slot;
            u8 bm = (u8)(side == 0);

            flagd = bm;
            slot = (s8)cm_fn_006c2310((u32)s, (s8)bm, 2);
            {
                u32 k = (u32)(s32)slot + oth * 20u;

                ent = s + 446u * k;
                H(ent + 0x47cb) = (u16)(H(ent + 0x47cb) - 0x32);
                B(ent + 0x47ba) = (u8)(B(ent + 0x47ba) + 1);
                B(d20 + 0xf5cc) = (u8)(B(d20 + 0xf5cc) + 1);
                if (B(ent + 0x47ba) == 2) {
                    B(ent + 0x47b9) = (u8)(B(ent + 0x47b9) + 1);
                    B(d20 + 0xf5cb) = (u8)(B(d20 + 0xf5cb) + 1);
                    cm_fn_006cda20((u32)s, (char *)(ent + 0x4796), 1);
                    cm_fn_006c0c60((u32)s, (u32)flagd);
                    cm_fn_006d6f70(W(s + 0x8f7a), 0x1ff8, (u32)s,
                                   (u32)(u8)slot, (u32)flagd, -1, -1, 0);
                }
            }
            if (B(d20 + 0xf5cb) > 4)
                goto lbl_6bfd45;
        }
    }

    {
        u32 rr = rnd(0x1e);

        if (rr != 0) {
            flag28 = 1;
            B(d20 + 0xf5c6) = (u8)(B(d20 + 0xf5c6) + 1);
        } else {
            flag20 = 1;
        }
    }

lbl_6bf720:
    if (side == 0) {
        u32 c1;
        s32 diff, rel;

        o = W(s + 0x4792);
        c0 = B(s + 0xf5bd);
        c1 = B(s + 0xf5f3);
        diff = (s32)c0 - (s32)c1;
        rel = (s32)(s8)B(o + 0x4b) - (s32)(s8)B(o + 0x4c);
        d20 = c0;
        if (rel + diff > 3) {
            prev = (long double)(s32)diff;
        } else if ((s32)(c0 + c1) != 0) {
            prev = (long double)CM_GLOBAL(float, 0x968e70);
        } else if ((s16)H(s + 0x8ed0) <= 0x3c) {
            prev = (long double)CM_GLOBAL(float, 0x96d248);
        } else {
            prev = (long double)CM_GLOBAL(float, 0x96d320);
        }
    } else {
        u32 cs, co;
        s32 diff, rel;

        o = W(s + 0x4792);
        cs = B(s + ix * 54u + 0xf5bd);
        co = B(s + oth * 54u + 0xf5bd);
        diff = (s32)cs - (s32)co;
        rel = (s32)(s8)B(o + 0x4c) - (s32)(s8)B(o + 0x4b);
        d20 = cs;
        if (rel + diff > 3) {
            prev = (long double)(s32)diff;
        } else if ((s32)(cs + co) != 0) {
            prev = (long double)CM_GLOBAL(float, 0x968e70);
        } else if ((s16)H(s + 0x8ed0) <= 0x3c) {
            prev = (long double)CM_GLOBAL(float, 0x96d248);
        } else {
            prev = (long double)CM_GLOBAL(float, 0x96d320);
        }
    }

    if (flag28 == 0) {
        long double prod = (long double)(s32)(s16)H(s + 2u * oth + 0x1da) *
                           prev;
        u32 a = (u32)(long long)(prod *
                                 (long double)CM_GLOBAL(double, 0x96d318));
        s32 dv;

        r1 = rnd(a);
        r2 = rnd((u32)(s16)H(s + 2u * side + 0x1de));
        dv = (s32)r2 - (s32)r1;
        d = (u32)dv;
        if ((s16)(u16)d > 0) {
            cm_fn_006c06b0((u32)s, (u32)side, 0x1c);
            flagd = (u8)(side == 0);
            cm_fn_006c07a0((u32)s, (u32)flagd, 0xffffffec);
        } else {
            cm_fn_006c06b0((u32)s, (u32)side, 0xfffffffa);
            flagd = (u8)(side == 0);
            cm_fn_006c07a0((u32)s, (u32)flagd, 0x28);
        }
    } else {
        long double prod = (long double)(s32)(s16)H(s + 2u * oth + 0x1f2) *
                           prev;
        u32 a = (u32)(long long)(prod *
                                 (long double)CM_GLOBAL(double, 0x96d310));
        s32 dv;

        r1 = rnd(a);
        r2 = rnd((u32)(s16)H(s + 2u * side + 0x1de));
        dv = (s32)r2 - (s32)r1;
        d = (u32)dv;
        if ((s16)(u16)d > 0) {
            cm_fn_006c06b0((u32)s, (u32)side, 0x1c);
            flagd = (u8)(side == 0);
            cm_fn_006c0a70((u32)s, (u32)flagd, 0xffffffec);
        } else {
            cm_fn_006c06b0((u32)s, (u32)side, 0xfffffffa);
            flagd = (u8)(side == 0);
            cm_fn_006c0a70((u32)s, (u32)flagd, 0x28);
        }
    }

    /* 0x006bf912 */
    if ((s16)(u16)d <= 0)
        goto lbl_6bfd23;
    r1 = rnd((u32)B(s + 0xf5f3) + (u32)B(s + 0xf5bd));
    if ((s32)r1 > 3)
        goto lbl_6bfd23;

    if (flag20 != 0)
        B(s + ix * 54u + 0xf5c0) = (u8)(B(s + ix * 54u + 0xf5c0) + 1);
    {
        u32 a = 18u * (3u * ix + 0xda7u);

        B(s + a) = (u8)(B(s + a) + 1);
    }
    r23 = (u8)cm_fn_006c23f0((u32)s, (u32)side, (u32)flag28);
    {
        s32 arg;

        if (flag28 != 0)
            arg = (s32)(s16)H(s + 2u * side + 0x1ee);
        else
            arg = (s32)(s16)H(s + 2u * side + 0x1e2) *
                  ((s32)(s8)flag20 + 1);
        d20 = rnd((u32)arg);
    }
    w26 = (s16)d20;
    dres = (u32)(s32)w26;
    r1 = rnd(0x172);
    if ((s32)w26 + 0xa5 <= (s32)r1 + (0x168 - (s32)(s16)(u16)d))
        goto lbl_6bfd23;

    /* 0x006bf9ec */
    B(s + ix * 54u + 0xf5bf) = (u8)(B(s + ix * 54u + 0xf5bf) + 1);
    if (flag28 != 0)
        cm_fn_006c0890((u32)s, (u32)side, d20);
    else
        cm_fn_006c0980((u32)s, (u32)side, d20);
    {
        s32 lim = (7 - 3 * (s32)(s8)flag20) *
                  (s32)(s16)H(s + 2u * oth + 0x1e6);
        u32 r3 = rnd((u32)lim);

        if ((s32)w26 + 0x2d > (s32)r3) {
            u8 res = (u8)cm_fn_006c2050((u32)s, (u32)side, (u32)flag20);
            u32 k = (u32)(s32)(s8)res + ix * 20u;
            u8 *p;

            ent = s + 446u * k;
            dres = (u32)res;
            B(ent + 0x47a2) = (u8)(B(ent + 0x47a2) + 1);
            H(ent + 0x47cb) = (u16)(H(ent + 0x47cb) + 0x1db);
            p = s + oth * 8920u + 0x47a5;
            {
                u32 n = 0x14;

                while (n != 0) {
                    if ((s8)B(p + 0xa) >= 0) {
                        if (cm_fn_00717410((void *)(p - 0xf)) == 1)
                            B(p) = (u8)(B(p) + 1);
                    }
                    p += 446u;
                    n--;
                }
            }
            if (flag20 != 0) {
                cm_fn_006d6f70(W(s + 0x8f7a), 0x1fcf, (u32)s, dres,
                               (u32)side, -1, -1, 0);
                B(s + ix * 54u + 0xf5c1) =
                    (u8)(B(s + ix * 54u + 0xf5c1) + 1);
                B(ent + 0x47a3) = (u8)(B(ent + 0x47a3) + 1);
                B(ent + 0x47a4) = (u8)(B(ent + 0x47a4) + 1);
            } else {
                cm_fn_006d6f70(W(s + 0x8f7a), 0x1fcc, (u32)s, dres,
                               (u32)side, -1, -1, 0);
                if (flag28 != 0) {
                    s32 q = (s32)rnd(4);

                    if (q < 0) {
                        B(s + ix * 54u + 0xf5c4) =
                            (u8)(B(s + ix * 54u + 0xf5c4) + 1);
                    } else if (q <= 1) {
                        B(s + ix * 54u + 0xf5c3) =
                            (u8)(B(s + ix * 54u + 0xf5c3) + 1);
                    } else if (q == 2) {
                        B(s + ix * 54u + 0xf5c5) =
                            (u8)(B(s + ix * 54u + 0xf5c5) + 1);
                    } else {
                        B(s + ix * 54u + 0xf5c4) =
                            (u8)(B(s + ix * 54u + 0xf5c4) + 1);
                    }
                }
            }
            /* 0x006bfb5c */
            if ((s8)r23 > -1) {
                u32 k2 = (u32)(s32)(s8)r23 + ix * 20u;
                u8 *e2 = s + 446u * k2;

                B(e2 + 0x47a6) = (u8)(B(e2 + 0x47a6) + 1);
                H(e2 + 0x47cb) = (u16)(H(e2 + 0x47cb) + 0x15e);
            }
            B(s + oth * 54u + 0xf5c2) = (u8)(B(s + oth * 54u + 0xf5c2) + 1);
            B(s + ix * 54u + 0xf5bd) = (u8)(B(s + ix * 54u + 0xf5bd) + 1);
            {
                u8 *q = s + ix * 8920u + 0x47cb;
                u32 n = 0x14;

                while (n != 0) {
                    if ((s8)B(q - 0x1c) >= 0)
                        H(q) = (u16)(H(q) + 0x4b);
                    q += 446u;
                    n--;
                }
            }
            {
                u8 *q = s + (u32)(s32)(s8)flagd * 8920u + 0x47cb;
                u32 n = 0x14;

                while (n != 0) {
                    if ((s8)B(q - 0x1c) >= 0)
                        H(q) = (u16)(H(q) - 0x43);
                    q += 446u;
                    n--;
                }
            }
            if (flag15 != 0)
                goto lbl_6bfdf6;
            goto lbl_6bfd23;
        }
    }

    /* 0x006bfcaa */
    cm_fn_006c0b70((u32)s, (u32)flagd, 0xe1);
    if (flag20 != 0) {
        u8 res = (u8)cm_fn_006c2050((u32)s, (u32)side, (u32)flag20);
        u32 k;
        u8 *e2;

        dres = (u32)res;
        cm_fn_006d6f70(W(s + 0x8f7a), 0x1fce, (u32)s, dres, (u32)side,
                       -1, -1, 0);
        k = (u32)(s32)(s8)res + ix * 20u;
        e2 = s + 446u * k;
        B(e2 + 0x47a3) = (u8)(B(e2 + 0x47a3) + 1);
        H(e2 + 0x47cb) = (u16)(H(e2 + 0x47cb) - 500);
    }
    goto lbl_6bfd23;

lbl_6bfd3b:
    fail13 = 1;
    goto lbl_6bfdfd;

lbl_6bfd23:
    outer = (u8)(outer + 1);
    if ((s8)outer >= 2)
        goto lbl_6bfdfd;
    goto lbl_6bf088;

lbl_6bfd45:
    {
        u32 s0 = (u32)(side == 0);
        u8 cur;

        fail13 = 1;
        B(s + s0 * 54u + 0xf5bd) = 0;
        cur = B(s + ix * 54u + 0xf5bd);
        B(s + ix * 54u + 0xf5bd) = (u8)((cur > 3) ? cur : 3);
        if (side != 0) {
            B(W(s + 0x4792) + 0x43) = 3;
            B(W(s + 0x4792) + 0x44) = 0;
        } else {
            B(W(s + 0x4792) + 0x43) = 0;
            B(W(s + 0x4792) + 0x44) = 3;
        }
        o = W(s + 0x4792);
        if (B(o + 0x4b) != 0xff) {
            if ((s32)(s8)B(o + 0x43) + (s32)(s8)B(o + 0x4b) ==
                (s32)(s8)B(o + 0x4c) + (s32)(s8)B(o + 0x44)) {
                B(o + 0x43) = (u8)(B(o + 0x43) + 1);
                B(s + 0xf5bd) = (u8)(B(s + 0xf5bd) + 1);
            }
        }
    }
    goto lbl_6bfdfd;

lbl_6bfdf6:
    B(s + 0x1a8) = 1;

lbl_6bfdfd:
    H(s + 0x8ed0) = (u16)(H(s + 0x8ed0) + 1);
    ax = H(s + 0x8ed0);
    if (fail13 != 0)
        goto lbl_6bff20;
    if (B(s + 0x1a8) != 0)
        goto lbl_6bfff9;
    if (ax == 0x2d)
        goto lbl_6bfe2b;
    if (ax == 0x5a)
        goto lbl_6bfe61;
    if (ax == 0x69)
        goto lbl_6bff03;
    if (ax == 0x78)
        goto lbl_6c0079;
    goto lbl_beeda;

lbl_6bfe2b:
    c0 = B(s + 0xf5bd);
    H(s + 0x8ed4) = 0x3de;
    B(W(s + 0x4792) + 0x45) = c0;
    o = W(s + 0x4792);
    B(o + 0x46) = B(s + 0xf5f3);
    goto lbl_beeda;

lbl_6bfe61:
    B(W(s + 0x4792) + 0x43) = B(s + 0xf5bd);
    B(W(s + 0x4792) + 0x44) = B(s + 0xf5f3);
    o = W(s + 0x4792);
    H(s + 0x8ed4) = 0x483;
    if (B(o + 0x4b) != 0xff) {
        if ((s32)(s8)B(o + 0x43) + (s32)(s8)B(o + 0x4b) !=
            (s32)(s8)B(o + 0x4c) + (s32)(s8)B(o + 0x44))
            goto lbl_6c0192;
        if (B(o + 0x38) & 0x80) {
            flag15 = 1;
            goto lbl_beeda;
        }
        if (B(o + 0x38) & 2)
            goto lbl_beeda;
        goto lbl_6c001c;
    }
    if (B(o + 0x43) != B(o + 0x44))
        goto lbl_6c0192;
    if (B(o + 0x38) & 0x80) {
        flag15 = 1;
        goto lbl_beeda;
    }
    if (B(o + 0x38) & 2)
        goto lbl_beeda;
    goto lbl_6c0060;

lbl_6bff03:
    H(s + 0x8ed4) = 0x528;
    goto lbl_beeda;

lbl_6bff20:
    c0 = B(s + 0xf5bd);
    if ((s16)ax > 0x5a)
        goto lbl_6bff96;
    o = W(s + 0x4792);
    B(o + 0x43) = c0;
    o = W(s + 0x4792);
    B(o + 0x44) = B(s + 0xf5f3);
    o = W(s + 0x4792);
    {
        u8 dl4b = B(o + 0x4b);
        u8 cl43 = B(o + 0x43);

        if (dl4b == 0xff) {
            if (cl43 != B(o + 0x44))
                goto lbl_6c0192;
            if ((B(o + 0x38) & 0xa3) == 0)
                goto lbl_6c0192;
            B(o + 0x43) = (u8)(cl43 + 1);
            goto lbl_6c0192;
        }
        if ((s32)(s8)cl43 + (s32)(s8)dl4b !=
            (s32)(s8)B(o + 0x4c) + (s32)(s8)B(o + 0x44))
            goto lbl_6c0192;
        B(o + 0x43) = (u8)(cl43 + 1);
        goto lbl_6c0192;
    }

lbl_6bff96:
    o = W(s + 0x4792);
    B(o + 0x47) = c0;
    o = W(s + 0x4792);
    B(o + 0x48) = B(s + 0xf5f3);
    o = W(s + 0x4792);
    {
        u8 cl4b = B(o + 0x4b);

        if (cl4b == 0xff) {
            if (B(o + 0x43) != B(o + 0x44))
                goto lbl_6c0192;
            if ((B(o + 0x38) & 0xa3) == 0)
                goto lbl_6c0192;
            B(o + 0x47) = (u8)(B(o + 0x47) + 1);
            goto lbl_6c0192;
        }
        if ((s32)(s8)B(o + 0x43) + (s32)(s8)cl4b !=
            (s32)(s8)B(o + 0x4c) + (s32)(s8)B(o + 0x44))
            goto lbl_6c0192;
        B(o + 0x47) = (u8)(B(o + 0x47) + 1);
        goto lbl_6c0192;
    }

lbl_6bfff9:
    B(W(s + 0x4792) + 0x47) = B(s + 0xf5bd);
    B(W(s + 0x4792) + 0x48) = B(s + 0xf5f3);
    goto lbl_6c0192;

lbl_6c001c:
    o = W(s + 0x4792);
    {
        u16 fl = H(o + 0x38);

        if ((fl & 4) == 0 && B(o + 0x44) != B(o + 0x4b) &&
            (fl & 0x4000) == 0)
            goto lbl_6c0192;
        if (fl & 1)
            goto lbl_6c0116;
        if ((fl & 0x20) == 0)
            goto lbl_6c0192;
        goto lbl_6c0116;
    }

lbl_6c0060:
    {
        u16 fl = H(W(s + 0x4792) + 0x38);

        if (fl & 1)
            goto lbl_6c0116;
        if ((fl & 0x20) == 0)
            goto lbl_6c0192;
        goto lbl_6c0116;
    }

lbl_6c0079:
    B(W(s + 0x4792) + 0x47) = B(s + 0xf5bd);
    B(W(s + 0x4792) + 0x48) = B(s + 0xf5f3);
    o = W(s + 0x4792);
    if (B(o + 0x4b) != 0xff) {
        u16 bl = H(o + 0x38);

        if (bl & 0x4000) {
            u8 dl48 = B(o + 0x48);
            s32 e1 = (s32)(s8)B(o + 0x47) + (s32)(s8)B(o + 0x4b);
            s32 e2 = (s32)(s8)B(o + 0x4c) + (s32)(s8)dl48;

            if (e1 != e2)
                goto lbl_6c0192;
            if (dl48 != B(o + 0x4b))
                goto lbl_6c0192;
        } else {
            s32 e1 = (s32)(s8)B(o + 0x4c) + (s32)(s8)B(o + 0x48);
            s32 e2 = (s32)(s8)B(o + 0x47) + (s32)(s8)B(o + 0x4b);

            if (e2 != e1)
                goto lbl_6c0192;
        }
        if (bl & 1)
            goto lbl_6c0116;
        if ((bl & 0x20) == 0)
            goto lbl_6c0192;
        goto lbl_6c0116;
    }
    if (B(o + 0x47) != B(o + 0x48))
        goto lbl_6c0192;
    if ((B(o + 0x38) & 0x21) == 0)
        goto lbl_6c0192;
    goto lbl_6c0116;

lbl_6c0116:
    rnd(5);
    rnd(5);
    r1 = rnd(5);
    B(W(s + 0x4792) + 0x49) = (u8)r1;
    rnd(5);
    rnd(5);
    r2 = rnd(5);
    B(W(s + 0x4792) + 0x4a) = (u8)r2;
    o = W(s + 0x4792);
    if (B(o + 0x49) == B(o + 0x4a)) {
        r1 = rnd(2);
        o = W(s + 0x4792);
        if (r1 == 0)
            B(o + 0x4a) = (u8)(B(o + 0x4a) + 1);
        else
            B(o + 0x49) = (u8)(B(o + 0x49) + 1);
    }
    goto lbl_6c0192;

lbl_6c0192:
    o = W(s + 0x4792);
    {
        u8 dl4b = B(o + 0x4b);

        if (dl4b != 0xff) {
            u8 cl47 = B(o + 0x47);

            if (cl47 != 0xff) {
                B(o + 0x4b) = (u8)(cl47 + dl4b);
                B(W(s + 0x4792) + 0x4c) =
                    (u8)(B(W(s + 0x4792) + 0x4c) +
                         B(W(s + 0x4792) + 0x48));
            } else {
                B(o + 0x4b) = (u8)(B(o + 0x43) + dl4b);
                B(W(s + 0x4792) + 0x4c) =
                    (u8)(B(W(s + 0x4792) + 0x4c) +
                         B(W(s + 0x4792) + 0x44));
            }
        }
    }
    /* 0x006c01cc */
    o = W(s + 0x4792);
    {
        u32 sub = W(o + 0x1c);

        if (sub == 0)
            goto lbl_6c036c;
        if (W(o + 0x14) != W(sub + 0x57))
            goto lbl_6c036c;
        {
            u8 b44 = B(o + 0x44);
            u8 b43 = B(o + 0x43);

            if ((s32)(s8)b43 + (s32)(s8)b44 >= 5)
                goto lbl_6c036c;
            if (b43 != b44)
                goto lbl_6c036c;
        }
        if (rnd(4) != 0)
            goto lbl_6c036c;
    }
    o = W(s + 0x4792);
    {
        s16 v1 = (s16)H(W(o + 0x1c) + 0x80);
        s16 v2 = (s16)H(W(o + 0x20) + 0x80);

        if ((s32)v1 > (s32)v2) {
            u8 res;
            u32 k;
            u8 *e2;
            u32 n;

            B(s + 0xf5be) = (u8)(B(s + 0xf5be) + 1);
            res = (u8)cm_fn_006c2050((u32)s, 0, (u32)flag20);
            dres = (u32)res;
            k = (u32)(s32)(s8)res;
            e2 = s + 446u * k;
            B(e2 + 0x47a2) = (u8)(B(e2 + 0x47a2) + 1);
            H(e2 + 0x47cb) = (u16)(H(e2 + 0x47cb) + 0x1db);
            {
                u8 *p = s + 0x6a7d;

                n = 0x14;
                while (n != 0) {
                    if ((s8)B(p + 0xa) >= 0) {
                        if (cm_fn_00717410((void *)(p - 0xf)) == 1)
                            B(p) = (u8)(B(p) + 1);
                    }
                    p += 446u;
                    n--;
                }
            }
            cm_fn_006d6f70(W(s + 0x8f7a), 0x1fcc, (u32)s, dres, 0, -1, -1, 0);
        } else if ((s32)v1 + 0xfa < (s32)v2) {
            u8 res;
            u32 k;
            u8 *e2;
            u32 n;

            B(s + 0xf5f4) = (u8)(B(s + 0xf5f4) + 1);
            res = (u8)cm_fn_006c2050((u32)s, 1, (u32)flag20);
            dres = (u32)res;
            k = (u32)(s32)(s8)res;
            e2 = s + 446u * k;
            B(e2 + 0x6a7a) = (u8)(B(e2 + 0x6a7a) + 1);
            H(e2 + 0x6aa3) = (u16)(H(e2 + 0x6aa3) + 0x1db);
            {
                u8 *p = s + 0x47a5;

                n = 0x14;
                while (n != 0) {
                    if ((s8)B(p + 0xa) >= 0) {
                        if (cm_fn_00717410((void *)(p - 0xf)) == 1)
                            B(p) = (u8)(B(p) + 1);
                    }
                    p += 446u;
                    n--;
                }
            }
            cm_fn_006d6f70(W(s + 0x8f7a), 0x1fcc, (u32)s, dres, 1, -1, -1, 0);
        }
    }

lbl_6c036c:
    if (W(W(s + 0x4792)) == 0xfffffffeu)
        goto lbl_6c04c3;
    {
        s32 si = (s32)(s8)side;
        u32 outn = 2;

        do {
            u8 *ep = s + (u32)si * 8920u + 0x4803;
            u32 inn = 0x14;

            do {
                if ((s8)B(ep - 0x54) >= 0) {
                    u32 obj = W(ep);

                    if (obj != 0) {
                        float f = *(float *)(ep - 0x20);
                        u8 cost = (u8)((u8)((u8)B(obj + 0x2d) << 1) + 5);

                        if (!((long double)f >=
                              (long double)CM_GLOBAL(float, 0x96d30c))) {
                            cost = (u8)(cost + 0xf);
                        } else if (!((long double)f >=
                                     (long double)CM_GLOBAL(float,
                                                            0x96d308))) {
                            cost = (u8)(cost + 5);
                        }
                        r1 = rnd((u32)(s32)(s8)cost);
                        r2 = rnd(0x190);
                        if ((s32)r1 >= (s32)r2) {
                            u32 who = W(W(s + 0x4792) + 0x14);
                            u32 bound;
                            u32 pickv;
                            u32 sel;

                            if (who == 0)
                                bound = 0x5b;
                            else if (W(0xae2c74u) == 0)
                                bound = 0x66;
                            else if ((u32)cm_fn_005c6710(
                                         (void *)W(0xae2c74u), who) == 0)
                                bound = 0x66;
                            else
                                bound = 0x5b;
                            pickv = (u8)rnd(bound);
                            dres = pickv;
                            sel = rnd(0xa);
                            if (sel <= 9) {
                                u32 mi = (u32)B(0x6c069cu + sel);
                                u32 tgt = W(0x6c068cu + 4u * mi);
                                u32 item = (u32)ep - 0x6du;

                                if (tgt == 0x6c045eu)
                                    cm_fn_006d0980((u32)s, 1, pickv, item,
                                                   0);
                                else if (tgt == 0x6c046du)
                                    cm_fn_006d0980((u32)s, 3, pickv, item,
                                                   0);
                                else if (tgt == 0x6c047cu)
                                    cm_fn_006d0980((u32)s, 4, pickv, item,
                                                   0);
                                else if (tgt == 0x6c048bu)
                                    cm_fn_006d0980((u32)s, 5, pickv, item,
                                                   0);
                            }
                        }
                    }
                }
                ep += 446u;
                inn--;
            } while (inn != 0);
            outn--;
        } while (outn != 0);
    }

lbl_6c04c3:
    {
        u8 ri = 0;

        for (;;) {
            u8 *ep = s + (u32)(s32)(s8)ri * 8920u + 0x47cb;
            u8 b = 0;

            while ((s8)b < 0x14) {
                o = W(s + 0x4792);
                if (W(o) == 0xfffffffeu && (s8)b < 0xb) {
                    u32 sub = ((s8)ri != 0) ? W(o + 0x20) : W(o + 0x1c);
                    s16 v1 = (s16)H(ep + 6);
                    s16 v2 = (s16)H(sub + 0x80);
                    s16 v0 = (s16)H(ep);
                    long double t, u, r;
                    s16 res;

                    t = ((long double)v1 -
                         (long double)CM_GLOBAL(double, 0x968e30)) *
                        (long double)CM_GLOBAL(double, 0x968ff8);
                    u = (long double)v2 *
                            (long double)CM_GLOBAL(double, 0x969018) -
                        (long double)CM_GLOBAL(double, 0x96d300);
                    r = (t - u) * (long double)CM_GLOBAL(double, 0x96d2f8) +
                        (long double)CM_GLOBAL(double, 0x967890);
                    r *= (long double)v0;
                    res = (s16)(u16)(long long)r;
                    H(ep) = (u16)res;
                    if (res < (s16)0xdac) {
                        res = (s16)0xdac;
                        H(ep) = (u16)res;
                    } else if (res > (s16)0x2ee0) {
                        res = (s16)0x2ee0;
                        H(ep) = (u16)res;
                    }
                }
                {
                    long double acc =
                        (long double)((s32)(s16)H(ep) + 0x1770) *
                            (long double)CM_GLOBAL(double, 0x96a608) +
                        (long double)CM_GLOBAL(double, 0x96d2f0);

                    B(ep - 0x1a) = (u8)(long long)acc;
                    if ((s8)B(ep - 0x1a) > 0xa)
                        B(ep - 0x1a) = 0xa;
                    if ((s8)B(ep - 0x1a) < 1)
                        B(ep - 0x1a) = 1;
                }
                W(ep + 0x1c) = W(ep + 0x18);
                b = (u8)(b + 1);
                ep += 446u;
            }
            ri = (u8)(ri + 1);
            if ((s8)ri >= 2)
                break;
        }
    }

    /* 0x006c05e5 */
    ax = H(s + 0xf5e2);
    if ((s16)ax < 0xa) {
        H(s + 0xf5e2) = 0x14;
        H(s + 0xf5e4) = (u16)(rnd(6) + 0xc);
    } else if (ax == H(s + 0xf5e4)) {
        r1 = rnd(3);
        H(s + 0xf5e4) = (u16)(H(s + 0xf5e4) + (u16)(0xffffu - r1));
    }
    ax = H(s + 0xf618);
    if ((s16)ax < 0xa) {
        H(s + 0xf618) = 0x14;
        H(s + 0xf61a) = (u16)(rnd(6) + 0xc);
    } else if (ax == H(s + 0xf61a)) {
        r1 = rnd(3);
        H(s + 0xf61a) = (u16)(H(s + 0xf61a) + (u16)(0xffffu - r1));
    }
}
