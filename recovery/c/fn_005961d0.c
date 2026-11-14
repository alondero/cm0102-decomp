/* fn_005961d0 -- finance.cpp, preferred-image VA 0x005961d0, image base 0x00400000.
   thiscall (ECX = this), one stack argument, `ret 4`.

   Translation of the original listing. Callees stay explicit boundaries.
   x87 evaluation follows the listing: arithmetic is done in 80-bit long
   double, and the original `fstp qword [esp+0x10]` slots are `double`. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u8 CM_THISCALL cm_fn_005986b0(u32 self);
extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* the original callee reads its first two inputs from EAX and EDX and its
   single bound argument from the stack (0x0090cfc2 reads [esp+4]);
   regparm(2) reproduces that register assignment */
extern unsigned long long cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound)
    __attribute__((regparm(2)));
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void cm_fn_00945934(u32 param_1);
extern s32 CM_CDECL cm_fn_00946fa0(u32 first, u32 second, u32 count);

/* original image constants / globals */
#define KD(va) CM_GLOBAL(double, va)
#define KU(va) CM_GLOBAL(u32, va)
#define ADDR(v) ((const u8 *)(unsigned long)(v))

/* fnstsw ax at 0x005963fb / 0x005964e4 leaves the x87 status word in EAX's low
   half.  Both compares feeding cm_fn_0090cfc0 at 0x005964ed are ordered and
   strictly less, so C0 (0x0100) is the only status bit this routine's own
   compare sets; the sticky exception flags of earlier x87 operations are not
   representable in C. */
#define SW_LT 0x0100u

static u8 r8(const u8 *p) { return *p; }
static u16 r16(const u8 *p) { u16 v; __builtin_memcpy(&v, p, 2); return v; }
static u32 r32(const u8 *p) { u32 v; __builtin_memcpy(&v, p, 4); return v; }
static void w8(u8 *p, u8 v) { *p = v; }
static void w32(u8 *p, u32 v) { __builtin_memcpy(p, &v, 4); }
static double rd_q(u32 va) { double v; __builtin_memcpy(&v, ADDR(va), 8); return v; }

/* one-operand `imul` of a magic constant: 64-bit signed product, high in EDX */
static s32 mul_hi(s32 a, s32 b, s32 *lo)
{
    long long p = (long long)a * (long long)b;
    *lo = (s32)(u32)((unsigned long long)p & 0xffffffffu);
    return (s32)(u32)((unsigned long long)p >> 32);
}

/* sar edx,2 ; mov eax,edx ; shr eax,0x1f ; add edx,eax */
static s32 q10_of_hi(s32 hi)
{
    s32 t = hi >> 2;
    return t + (s32)((u32)t >> 31);
}

void CM_THISCALL cm_fn_005961d0(unsigned long long *param_1, u32 *param_2)
{
    u8 *self = (u8 *)param_1;
    u8 *arg = (u8 *)param_2;
    /* original frame slots, kept adjacent: [esp+0xc]=L+0, [esp+0x10]=L+4,
       [esp+0x1c]=L+0x10, [esp+0x20]=L+0x14, [esp+0x120]=L+0x114 */
    u8 scratch[0x200];      /* L+0x14 .. L+0x213 */
    u32 t0;                 /* [esp+0xc] */
    double qd;              /* [esp+0x10] qword */
    u32 t1c;                /* [esp+0x1c] */
    long long fv = 0;
    u8 al;
    u32 eax5 = 0;           /* EAX seen by cm_fn_0090cfc0 at 0x005964ed */
    u32 edx5 = 0;           /* EDX seen by cm_fn_0090cfc0 at 0x005964ed */

    if (param_2 == (u32 *)0) {
        /* 0x005961ea: report the missing argument and stop */
        /* 0x00596206: b114/b14 and 0x8e are pushed before the (void) call to
           0x90d130 and belong to the following 0x944c9f call */
        char *b14 = (char *)(void *)&scratch[0x00];
        char *b114 = (char *)(void *)&scratch[0x100];
        char *msg;

        cm_fn_00944cff(0x009c8784u, 0u, 0u, (void *)b114, (void *)b14);
        msg = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)(unsigned long)msg,
                       (u32)(unsigned long)b114, (u32)(unsigned long)b14, 0x8eu);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        KU(0x00b67a34) = 0;
        cm_fn_005cc7d0();
        cm_fn_005a8f60();
        cm_fn_00945934(0xffffffffu);
        return;
    }

    /* 0x00596257 */
    w32(self + 8, r32(arg));
    if (cm_fn_0053b900(param_2) != 0) {
        w32(self + 0x00, 0);
        w32(self + 0x04, 0);
        w32(self + 0x10, 0);
        w32(self + 0x0c, 0);
        w32(self + 0x1c, 0);
        return;
    }

    /* 0x00596280 */
    if (r8(arg + 0x64) == 3) {
        u32 tbl = KU(0x00adadfc);
        u32 a0 = r32(arg + 0x57);
        int set2 = 0;

        if (a0 != 0 && tbl != 0 && r32(ADDR(tbl + r32(ADDR(a0)) * 4u)) != 0) {
            set2 = 1;
        } else {
            u32 b0 = r32(arg + 0x60);
            if (b0 != 0 && tbl != 0 && r32(ADDR(tbl + r32(ADDR(b0)) * 4u)) != 0)
                set2 = 1;
        }
        if (set2)
            w8(arg + 0x64, 2);
    }

    /* 0x005962b4 */
    w32(self + 0x160, 0x3dcccccdu);

    {
        s32 v65 = (s32)r32(arg + 0x65);
        t1c = (u32)v65;

        if (v65 != 0) {
            s16 w80 = (s16)r16(arg + 0x80);
            long double lw = (long double)(s32)w80;
            long double lv = (long double)v65;

            /* fild v65; fild w80; fld st0; fmul st1; fmul [0x968940];
               fld st2; fcompp  ->  v65 < w80*w80*C */
            if (lv < lw * lw * (long double)KD(0x00968940)) {
                /* 0x005962f3 */
                s32 lo1, hi1, lo2, hi2;
                u32 r1, r2;

                hi1 = mul_hi(0x66666667, v65, &lo1);
                qd = (double)v65;                       /* fstp qword [esp+0x10] */
                r1 = (u32)cm_fn_0090cfc0((u32)lo1, (u32)q10_of_hi(hi1),
                                         (u32)q10_of_hi(hi1));
                hi2 = mul_hi(0x66666667, (s32)t1c, &lo2);
                r2 = (u32)cm_fn_0090cfc0((u32)lo2, (u32)q10_of_hi(hi2),
                                         (u32)q10_of_hi(hi2));
                t0 = r2 - r1;                           /* mov [esp+0xc],eax */
                fv = (long long)((long double)(s32)t0 + (long double)qd);
                goto have_final;
            }
        }
    }

    /* 0x00596347 */
    if (cm_fn_00946fa0((u32)(unsigned long)(arg + 4), 0x009c877cu, 6u) == 0) {
        /* 0x0059635e: the "<Rushden" style name matched */
        u32 v65 = r32(arg + 0x65);
        s32 lo1, hi1, lo2, hi2;
        u32 r1, r2;

        t1c = v65;
        hi1 = mul_hi(0x66666667, (s32)v65, &lo1);
        qd = (double)(s32)v65;                          /* fstp qword [esp+0x10] */
        r1 = (u32)cm_fn_0090cfc0((u32)lo1, (u32)q10_of_hi(hi1), (u32)q10_of_hi(hi1));
        hi2 = mul_hi(0x66666667, (s32)t1c, &lo2);
        r2 = (u32)cm_fn_0090cfc0((u32)lo2, (u32)q10_of_hi(hi2), (u32)q10_of_hi(hi2));
        t0 = r2 - r1;
        fv = (long long)((long double)(s32)t0 + (long double)qd);
        goto have_final;
    }

    /* 0x005963ba */
    {
        s16 w80 = (s16)r16(arg + 0x80);
        s32 lo, hi;
        s32 idx;
        u8 dl;
        u32 club;
        u32 eax_low;

        hi = mul_hi(0x10624dd3, (s32)w80, &lo);
        eax_low = (u32)lo;
        {
            s32 t = hi >> 5;
            s32 q = t + (s32)((u32)t >> 31);
            dl = (u8)(q & 0xff);
        }
        dl = (u8)(dl - 1u);                             /* dec dl */
        if ((s8)dl < 0)                                 /* jns / xor dl,dl */
            dl = 0;
        idx = (s32)(s8)dl;                              /* movsx edx,dl */

        club = r32(arg + 0x53);
        qd = rd_q(0x009c8290u + (u32)idx * 8u);         /* fld [edx*8+0x9c8290] */
        if (club == 0) {
            /* 0x005965ba */
            qd = 0.0;
            goto L_596509;
        }

        /* 0x005963f1 */
        if ((long double)qd > (long double)KD(0x009689a0)) {
            const u8 *cl = ADDR(club);
            long double cv = (long double)(s32)(s8)cl[0x7e];
            s32 v85;
            u32 sq;
            u32 val, base;
            s16 wv;
            s32 wvsq;
            long long ft1;
            u32 ft1low, ft1high;
            s32 c64;

            if (cv < (long double)KD(0x00967890))
                cv = (long double)KD(0x00967890);
            else if (cv > (long double)KD(0x00968930))
                cv = (long double)KD(0x00968930);

            v85 = (s32)(s8)cl[0x85];
            sq = (u32)v85 * (u32)v85;
            t0 = sq;                                    /* mov [esp+0xc],ecx */
            val = r32(self + 8);
            base = KU(0x00ae23b4);
            {
                long double prod = (long double)(s32)t0 *
                                   (long double)KD(0x0096a578);
                long double p2 = (long double)qd * (long double)KD(0x00967888);
                u32 edxpart = (((val * 9u) << 4) + val) & 0xffffffffu;
                u32 ecxpart = (base + val) & 0xffffffffu;

                prod = prod * p2;                       /* fmulp st(1) */
                wv = (s16)r16(ADDR((ecxpart + edxpart * 4u + 0x80u) & 0xffffffffu));
                prod = prod / cv;                       /* fdiv st(1) */
                wvsq = (s32)((u32)(s32)wv * (u32)(s32)wv);
                qd = (double)prod;                      /* fstp qword [esp+0x10] */
                t0 = (u32)wvsq;
                ft1 = (long long)((long double)(s32)t0 *
                                  (long double)KD(0x00968968));
                ft1low = (u32)(unsigned long long)ft1;
                ft1high = (u32)((unsigned long long)ft1 >> 32);

                /* fcomp st(1): keep the smaller value */
                if ((long double)qd > (long double)(s32)ft1low)
                    qd = (double)(s32)ft1low;

                c64 = (s32)(s8)r8(arg + 0x64);
                if (c64 == 2) {
                    qd = (double)((long double)qd * (long double)KD(0x00968a80));
                    eax5 = 0;
                } else if (c64 == 3) {
                    qd = (double)((long double)qd * (long double)KD(0x00968918));
                    eax5 = 0;
                } else {
                    eax5 = (u32)(c64 - 3);
                }
                edx5 = ft1high;
            }
        } else {
            eax5 = (eax_low & 0xffff0000u) | SW_LT;
            edx5 = (u32)idx;
        }

        /* 0x005964da */
        if ((long double)qd < (long double)KD(0x009689a0)) {
            if ((u32)cm_fn_0090cfc0(eax5, edx5, 2u) != 0)
                qd = 50000.0;
        }
    }

L_596509:
    {
        long long q;
        u32 lo, hi;
        u32 r1, r2;
        s32 diff;
        u32 club;
        long double sum;

        /* 0x00596509 */
        q = (long long)((long double)qd * (long double)KD(0x00968918));
        lo = (u32)(unsigned long long)q;
        hi = (u32)((unsigned long long)q >> 32);

        r1 = (u32)cm_fn_0090cfc0(lo, hi, lo);
        t0 = r1;                        /* mov [esp+0x14],edx  (== [esp+0xc]) */
        r2 = (u32)cm_fn_0090cfc0(r1, r1, lo);
        diff = (s32)(r1 - r2);
        club = r32(arg + 0x53);
        t0 = (u32)diff;                 /* mov [esp+0xc],ecx */
        sum = (long double)(s32)t0 + (long double)qd;

        if (club != 0 && r32(ADDR(club)) == KU(0x009cf484) &&
            sum > (long double)KD(0x009688e0)) {
            /* 0x00596563 */
            long double a = sum - (long double)KD(0x009688e0);
            a = a * (long double)KD(0x00968968) + (long double)KD(0x009688e0);
            t0 = (u32)(unsigned long long)(long long)a;
            sum = (long double)(s32)t0;
        }
        fv = (long long)sum;            /* 0x00596582 */
    }

have_final:
    /* 0x00596587 */
    w32(self + 0x00, (u32)(unsigned long long)fv);
    w32(self + 0x04, (u32)((unsigned long long)fv >> 32));
    al = cm_fn_005986b0((u32)(unsigned long)self);

    /* 0x00596593 */
    if (r8(arg + 0x6d) == 0 || al == 0) {
        w8(self + 0x166, 10);
    } else if ((s8)al < 0 || (s8)al > 2) {
        w8(self + 0x166, 0x0f);
    } else {
        w8(self + 0x166, 5);
    }

    /* 0x005965df */
    w8(self + 0x165, 0);
    w32(self + 0x70, 0);
    w32(self + 0x74, 0);
    w32(self + 0x80, 0);
    w32(self + 0x84, 0);
    w32(self + 0x88, 0);
    w32(self + 0x8c, 0);
    w32(self + 0x90, 0);
    w32(self + 0x94, 0);
    w32(self + 0x98, 0);
    w32(self + 0x9c, 0);
    w32(self + 0xa0, 0);
    w32(self + 0x78, 0);
    w32(self + 0x7c, 0);
    w32(self + 0xa4, 0);
    w32(self + 0xa8, 0);
    w32(self + 0xac, 0);
    w32(self + 0xb0, 0);
    w32(self + 0xb4, 0);
    w32(self + 0xb8, 0);
    w32(self + 0xbc, 0);
    w32(self + 0x20, 0);
    w32(self + 0x24, 0);
    w32(self + 0x30, 0);
    w32(self + 0x34, 0);
    w32(self + 0x38, 0);
    w32(self + 0x3c, 0);
    w32(self + 0x40, 0);
    w32(self + 0x44, 0);
    w32(self + 0x48, 0);
    w32(self + 0x4c, 0);
    w32(self + 0x50, 0);
    w32(self + 0x28, 0);
    w32(self + 0x2c, 0);
    w32(self + 0x54, 0);
    w32(self + 0x58, 0);
    w32(self + 0x5c, 0);
    w32(self + 0x60, 0);
    w32(self + 0x64, 0);
    w32(self + 0x68, 0);
    w32(self + 0x6c, 0);
    w32(self + 0x110, 0);
    w32(self + 0x114, 0);
    w32(self + 0x120, 0);
    w32(self + 0x124, 0);
    w32(self + 0x128, 0);
    w32(self + 0x12c, 0);
    w32(self + 0x130, 0);
    w32(self + 0x134, 0);
    w32(self + 0x118, 0);
    w32(self + 0xc0, 0);
    w32(self + 0xc8, 0);
    w32(self + 0x138, 0);
    w32(self + 0x13c, 0);
    w32(self + 0x140, 0);
    w32(self + 0x11c, 0);
    w32(self + 0x144, 0);
    w32(self + 0x148, 0);
    w32(self + 0x14c, 0);
    w32(self + 0x150, 0);
    w32(self + 0x154, 0);
    w32(self + 0x158, 0);
    w32(self + 0x15c, 0);
    w32(self + 0xc4, 0);
    w32(self + 0xd0, 0);
    w32(self + 0xd4, 0);
    w32(self + 0xd8, 0);
    w32(self + 0xdc, 0);
    w32(self + 0xe0, 0);
    w32(self + 0xe4, 0);
    w32(self + 0xe8, 0);
    w32(self + 0xec, 0);
    w32(self + 0xf0, 0);
    w32(self + 0xcc, 0);
    w32(self + 0xf4, 0);
    w32(self + 0xf8, 0);
    w32(self + 0xfc, 0);
    w32(self + 0x100, 0);
    w32(self + 0x104, 0);
    w32(self + 0x108, 0);
    w32(self + 0x10c, 0);
    w32(self + 0x18, 0);
    w8(self + 0x164, 3);
    w32(self + 0x1c, 0);

    /* 0x0059678a */
    {
        u32 club = r32(arg + 0x53);
        u32 vid;
        u16 di;
        u32 edi_hy;
        u16 cx;
        s16 cx16;
        s32 v;

        if (club == 0) {
            w32(self + 0x14, 0);
            return;
        }
        vid = r32(ADDR(club));
        if (!(vid == KU(0x009cf2e4) || vid == KU(0x009cf360) ||
              vid == KU(0x009cf488))) {
            w32(self + 0x14, 0);
            return;
        }

        /* 0x005967b5: `mov di, word ptr [edi+0x80]` keeps EDI's high half */
        di = r16(arg + 0x80);
        if ((s16)di <= 0x128e) {
            w32(self + 0x14, 0);
            return;
        }
        if (!(al == 2 || al == 1)) {
            w32(self + 0x14, 0);
            return;
        }

        edi_hy = (((u32)(unsigned long)arg) & 0xffff0000u) | (u32)di;
        if (al == 2) {
            cx = (u16)(edi_hy + edi_hy - 0x128eu);      /* 0x005967e2 */
        } else if ((s16)di > 0x1482) {
            cx = (u16)(edi_hy + edi_hy - 0x1482u);      /* 0x005967f2 */
        } else {
            cx = (u16)(edi_hy + 0xfau);                 /* 0x005967fb */
        }

        /* 0x00596811 */
        cx16 = (s16)cx;
        if (r8(arg + 0x82) == 0) {
            if (cx16 < 0x1676)
                v = ((s32)cx16 - 0x2ee) * 6;            /* 0x00596876 */
            else if (al != 1)
                v = ((s32)cx16 - 0x2ee) * 10;           /* 0x005968a9 */
            else
                v = (s32)cx16 * 8 - 0x1770;             /* 0x00596893 */
        } else {
            if (cx16 < 0x1676)
                v = ((s32)cx16 - 0x4e2) * 6;            /* 0x00596823 */
            else if (al != 1)
                v = ((s32)cx16 - 0x4e2) * 10;           /* 0x00596856 */
            else
                v = (s32)cx16 * 8 - 0x2710;             /* 0x00596840 */
        }
        w32(self + 0x14, (u32)v);
    }
}
