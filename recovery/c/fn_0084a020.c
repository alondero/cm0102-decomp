/* fn_0084a020 -- translated from the original instructions at VA 0x0084a020
   (preferred image base 0x00400000).

   Stack frame: after `and esp,~7`, the SEH registration occupies
   prev/handler/state at S-12/S-8/S-4; scratch[0x128] covers S-0x134 .. S-0xd,
   i.e. the same bytes as the original `[esp + 0x..]` locals measured from the
   post-push esp.  Objects handed to callees stay at their original offsets
   (0x2e, 0x34, 0x3c, 0x44, 0x88, 0xd8) inside that one allocation.

   Note: the listing's `call 0x945640` (__ftol) is written as a cast to
   long long of the x87 value, per docs/adr/0006. */

#include "cmre.h"

/* ---- callee boundaries (original code) ---- */

/* ecu=0x00ae2c90, 2 stack args */
extern void CM_THISCALL cm_fn_0054c5b0(u32 self, u32 obj, int count);

extern void CM_THISCALL cm_fn_0085e410(u32 self);
/* 7 stack arguments observed at the call site (ret 0x1c) */
extern u32 CM_THISCALL cm_fn_0085fc50(u32 self, u32 a1, u32 a2, u32 a3,
                                      u32 a4, u32 a5, u32 a6, u32 a7);
extern void CM_THISCALL cm_fn_008be300(u32 self, u32 p1, u32 p2);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_005968d0(u32 self, u32 person, u32 flags);
extern unsigned long long CM_THISCALL cm_fn_005fd210(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_005fea50(u32 self, s8 category, u32 person);
extern u32 CM_THISCALL cm_fn_00844540(u32 self, u32 club, u8 filter);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 person, u32 club);
extern u32 CM_THISCALL cm_fn_004cc9e0(u32 self, u32 first, u32 second);
/* param_1 (ECX) is the receiver; 7 stack arguments at the call site */
extern void CM_THISCALL cm_fn_004cc490(u32 param_1, int p2, int p3,
                                       u32 p4, u8 p5, u32 p6, int p7, int p8);
extern void CM_THISCALL cm_fn_004cda70(u32 self, u32 p1, u32 p2, u32 p3,
                                       s32 p4, double p5, u8 p6);
extern u32 CM_THISCALL cm_fn_0062ed70(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_008cca10(u32 self, u32 record, u32 output);
extern u32 CM_THISCALL cm_fn_008c5450(u32 self, u32 record, u32 out);
extern u32 CM_THISCALL cm_fn_008ce990(u32 self, u32 person, u32 club,
                                      u32 output, u32 mode);
extern u32 CM_THISCALL cm_fn_008cdc90(u32 self, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_008c5e40(u32 self, u32 record, u32 output);
extern u32 CM_THISCALL cm_fn_008ced40(u32 self, u32 club, u32 second,
                                      u32 person, u32 fourth, u32 retry,
                                      u32 reason);
extern u32 CM_THISCALL cm_fn_008cde70(u32 self, u32 key, u32 mode);
extern u32 CM_THISCALL cm_fn_00849ce0(u32 self, u32 a1, u32 a2, u32 a3, u32 a4);
extern u32 CM_THISCALL cm_fn_00843e10(u32 self, u32 person, u32 club,
                                      u32 kind, u32 selector);
/* 2 stack args, ECX receiver, no caller cleanup (ret 8) */
extern u32 CM_THISCALL cm_fn_00842740(u32 self, u32 a, u32 b);
extern u32 CM_THISCALL cm_fn_008d0040(u32 self, u32 a, u32 b, u32 c);
extern void CM_THISCALL cm_fn_008e5680(u32 self);
extern void CM_THISCALL cm_fn_008e5710(u32 self);
extern u32 CM_THISCALL cm_fn_008e5d60(u32 self, u32 player, u32 club,
                                      u32 offer, u32 value, u32 mode,
                                      u32 output);

extern u32 CM_CDECL cm_fn_0053b900(u32 input);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip, u32 flag, u32 person,
                                   u32 force);
extern u32 CM_CDECL cm_fn_0053ca90(u32 self);
extern u32 CM_CDECL cm_fn_0053c590(u32 club, u32 mode, u32 filter);
/* one stack argument per call site (two calls, one `add esp,8`) */
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 arg);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 arg);

/* ---- original globals ---- */
#define G_AE1050 CM_GLOBAL(u32, 0x00ae1050)
#define G_AE2A38 CM_GLOBAL(u32, 0x00ae2a38)
#define G_AE2378 CM_GLOBAL(s32, 0x00ae2378)
#define G_AE23B4 CM_GLOBAL(u32, 0x00ae23b4)
#define G_AE23BC CM_GLOBAL(u32, 0x00ae23bc)
#define G_AE2D0C CM_GLOBAL(u32, 0x00ae2d0c)
#define G_AE1080 CM_GLOBAL(u32, 0x00ae1080)
#define G_ADADFC CM_GLOBAL(u32, 0x00adadfc)
#define G_B63C98 CM_GLOBAL(u32, 0x00b63c98)
#define G_B63D28 CM_GLOBAL(u32, 0x00b63d28)
#define G_DE1F54 CM_GLOBAL(u32, 0x00de1f54)
#define G_9CF300 CM_GLOBAL(u32, 0x009cf300)

/* ---- original floating point constants ---- */
#define D_967880 CM_GLOBAL(double, 0x00967880)
#define D_967890 CM_GLOBAL(double, 0x00967890)
#define D_968968 CM_GLOBAL(double, 0x00968968)
#define D_968978 CM_GLOBAL(double, 0x00968978)
#define D_968910 CM_GLOBAL(double, 0x00968910)
#define D_968940 CM_GLOBAL(double, 0x00968940)
#define D_9689A0 CM_GLOBAL(double, 0x009689a0)
#define D_968DC0 CM_GLOBAL(double, 0x00968dc0)
#define D_968DE8 CM_GLOBAL(double, 0x00968de8)
#define D_969020 CM_GLOBAL(double, 0x00969020)

/* scratch views (volatile: callees receive these as integers, so the
   compiler must not assume the object survives the call unchanged) */
#define S8(o) (*(volatile u8 *)(S + (o)))
#define S32(o) (*(volatile u32 *)(S + (o)))
#define SS16(o) (*(volatile s16 *)(S + (o)))
#define SD(o) (*(volatile double *)(S + (o)))

#define LIMIT S8(0x0f)
#define VALUE S32(0x10)
#define COUNTER S8(0x17)
#define CONT S32(0x18)
#define IDXOFF S32(0x1c)
#define TMP32 S32(0x20)
#define DBL SD(0x24)
#define REASON SS16(0x2e)
#define THISV S32(0x30)
#define OUT S32(0x34)
#define ISV11 S32(0x38)

#define MEM8(a) (*(volatile u8 *)(unsigned long)(a))
#define MEM16S(a) (*(volatile s16 *)(unsigned long)(a))
#define MEM32(a) (*(volatile u32 *)(unsigned long)(a))
#define MEMS32(a) (*(volatile s32 *)(unsigned long)(a))

#define STATEBYTE(v) (*(volatile u8 *)(void volatile *)&seh.state = (u8)(v))

void CM_THISCALL cm_fn_0084a020(u32 self, u32 param)
{
    u8 buf[0x128];
    volatile u8 *const S = buf;
    volatile struct {
        void *prev;
        void *handler;
        u32 state;
    } seh;

    u32 base, rec, person, club, person2, q, bits, sres, offset;
    u32 tx, w, a1, a2, a3;
    s32 val, div, pr, c, scratch_value;
    u32 r1, r2, r3;
    long double t;
    double d;

    seh.state = 0xffffffffu;
    seh.handler = (void *)0x009625a9;
    seh.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)(void volatile *)&seh;

    THISV = self;                                      /* 0x0084a052 */
    cm_fn_0054c5b0(0x00ae2c90u, (u32)(unsigned long)(S + 0x3c), 7);
    seh.state = 0;
    cm_fn_0085e410((u32)(unsigned long)(S + 0xd8));
    STATEBYTE(1);

    if (cm_fn_0053b900(param) != 0) goto done;
    if (cm_fn_00600a50(param, 0, 1, 0, 0) != 0) goto done;
    if (cm_fn_00600a50(param, 1, 1, 0, 0) != 0) {
        a1 = cm_fn_0053ca90(param);
        if ((u32)cm_fn_005fd210(G_B63C98, a1) != 0) {
            a1 = cm_fn_0053ca90(param);
            if ((u8)cm_fn_005fea50(G_B63C98, (s8)4, a1) == 1) goto done;
        }
    }

    /* 0x0084a0ef */
    a1 = MEM32(param);
    a2 = MEM32(self + 0xdc);
    CONT = a1 * 9u + a2;
    if (MEM8(CONT + 4) != 1) goto done;
    if (MEM8(CONT + 8) >= 0x64) goto done;
    if (cm_fn_005a0590(G_AE2A38, param) == 0) goto done;

    /* 0x0084a128 */
    a1 = (u8)cm_fn_00844540(self, param, 0x0b);
    COUNTER = (u8)a1;
    a2 = (u8)cm_fn_0053c590(param, 0, 0);
    LIMIT = (u8)(COUNTER + a2);
    COUNTER = 0;

    for (;;) {
        /* 0x0084a155 */
        base = MEM32(CONT);
        IDXOFF = (u32)(37 * (s32)(s8)COUNTER);
        rec = base + IDXOFF;
        ISV11 = (MEM8(rec) == 0x0b) ? 1u : 0u;
        if ((s8)LIMIT >= 0x32 && ISV11 != 0) goto next;
        val = MEMS32(rec + 0x0f);
        if (val <= -1) goto next;
        if (MEMS32(rec + 0x17) != -1) goto next;

        person = G_AE23BC + 110u * (u32)val;
        q = cm_fn_004cf5a0(G_AE1050, person);
        G_AE1080 = q;
        club = (q != 0) ? (G_AE23B4 + 581u * MEM32(q + 4)) : 0u;
        if (club == param) goto next;

        if (ISV11 != 0) goto L28e;

        base = CONT;                             /* 0x0084a1ef */
        offset = IDXOFF;                         /* 0x0084a1f3 */
        rec = MEM32(base) + offset;              /* 0x0084a1f7 */
        c = (s32)MEM8(rec);                      /* 0x0084a1f9 */
        if (c == 8) {
            s32 k = 0;
            for (;;) {
                if (MEM32(param + 0x19f + (u32)k * 4) == 0) break;
                k++;
                if (!(k < 5)) break;
            }
            if (k == 5) goto next;
            goto L28e;
        } else if (c == 9) {
            s32 k = 0;
            for (;;) {
                if (MEM32(param + 0x1b3 + (u32)k * 4) == 0) break;
                k++;
                if (!(k < 7)) break;
            }
            if (k == 7) goto next;
            goto L28e;
        } else if (c == 0x0a) {
            s32 k = 0;
            for (;;) {
                if (MEM32(param + 0x1cf + (u32)k * 4) == 0) break;
                k++;
                if (!(k < 3)) break;
            }
            if (k == 3) goto next;
            goto L28e;
        } else if (c == 6 || c == 7) {
            if (MEM32(param + 0xd3) != 0) goto next;
            goto L28e;
        }

    L28e:
        /* 0x0084a28e */
        if ((MEM8(G_AE2D0C + 79u * MEM32(person) + 0xb) & 0x30) != 0 &&
            ISV11 != 0)
            goto next;
        if (cm_fn_008cca10(G_DE1F54, person, 0) != 0) goto next;
        if (cm_fn_008c5450(G_DE1F54, person, 0) != 0) goto next;
        if (MEM32(person + 0x24) != 0 && MEM8(person + 0x28) == 5) goto next;

        /* 0x0084a2e8 */
        base = MEM32(CONT);
        rec = base + IDXOFF;
        if ((MEM8(rec + 0x24) & 1) != 0) goto L_aa05;
        if (MEM16S(rec + 1) <= -1 && ISV11 != 0) goto next;

        /* 0x0084a310 */
        if (cm_fn_008ce990(G_DE1F54, person, param,
                           (u32)(unsigned long)(S + 0x2e), 0) != 0)
            goto L334;
        if (REASON != 9) goto next;

    L334:
        if (cm_fn_008cdc90(G_DE1F54, person, param) != 0) goto next;
        if (cm_fn_008c5e40(G_DE1F54, person, 0) != 0) goto next;

        /* 0x0084a35e */
        rec = MEM32(CONT) + IDXOFF;
        bits = MEM8(rec + 0x24);
        val = (s32)MEM32(rec + 0x0b);
        if ((bits & 0x20) != 0) {
            w = (u32)val << 1;
            if (!((s32)w > 0x4e20)) w = 0x4e20u;
            val = (s32)w;
        }
        q = MEM32(param + 0x57);
        VALUE = (u32)val;
        if (q != 0) {
            s32 id = MEMS32(q);
            if (id > -1 && id < G_AE2378 &&
                MEM32(G_ADADFC + (u32)id * 4) != 0) {
                if (MEM16S(param + 0x80) <= 0x128e) {
                    if ((s8)LIMIT < 0x19) VALUE = (u32)val * 2u;
                } else if ((s8)LIMIT < 0x19) {
                    VALUE = (u32)val * 3u;
                } else if ((s8)LIMIT < 0x23) {
                    VALUE = (u32)val * 2u;
                }
            }
        }

        /* 0x0084a3e1 */
        if (MEMS32(rec + 0x13) > -1 && MEM8(rec) == 0x0b) {
            q = cm_fn_004cf5a0(G_AE1050, person);
            G_AE1080 = q;
            club = (q != 0) ? (G_AE23B4 + 581u * MEM32(q + 4)) : 0u;
            if (cm_fn_008ced40(G_DE1F54, param,
                               (u32)(unsigned long)(S + 0x3c), person, club, 1,
                               (u32)(unsigned long)(S + 0x2e)) == 0 &&
                REASON != 9) {
                rec = MEM32(CONT) + IDXOFF;
                w = MEM32(rec + 0x0b);
                if (!((s32)w > 0x2710)) w = 0x2710u;
                VALUE = VALUE + w;
            }

            rec = MEM32(CONT) + IDXOFF;
            person2 = G_AE23BC + 110u * MEM32(rec + 0x13);
            r1 = cm_fn_0062ed70(G_B63D28, person2);
            if (r1 != 0) {
                s16 s1 = MEM16S(cm_fn_0062ed70(G_B63D28, person2) + 0x2c);
                s16 s2 = MEM16S(cm_fn_0062ed70(G_B63D28, person2) + 0x2c);
                TMP32 = (u32)((s32)s1 * (s32)s2);
                div = MEM16S(param + 0x80) / 500;
                /* VA 0x0084a4e0: IMUL keeps the low dword on overflow. */
                VALUE = VALUE + (u32)TMP32 * (u32)div;
            }
            q = MEM32(person2 + 0x61);
            if (q != 0) {
                s32 bv = (s32)*(volatile s8 *)(unsigned long)
                             (q + 0x45);
                TMP32 = (u32)bv;
                t = ((long double)D_968940 -
                     (long double)bv * (long double)D_969020) *
                    (long double)(s32)VALUE;                  /* 0x00945640 */
                VALUE = VALUE + (u32)(long long)t;
            }
            if (cm_fn_008cde70(G_DE1F54, person2, 0) != 0) {
                rec = MEM32(CONT) + IDXOFF;
                t = (long double)(s32)MEM32(rec + 0x0b) *
                    (long double)D_968940;
                if (!(t > (long double)D_968910)) t = (long double)D_968910;
                VALUE = VALUE + (u32)(long long)t;            /* 0x00945640 */
            }
        }

        /* 0x0084a559 */
        q = cm_fn_004cf5a0(G_AE1050, person);
        G_AE1080 = q;
        club = (q != 0) ? (G_AE23B4 + 581u * MEM32(q + 4)) : 0u;
        if (club == 0) goto L697;
        if (ISV11 == 0) goto L697;
        q = cm_fn_004cf5a0(G_AE1050, person);
        G_AE1080 = q;
        club = (q != 0) ? (G_AE23B4 + 581u * MEM32(q + 4)) : 0u;
        if (cm_fn_00600a50(club, 0, 1, 0, 0) == 0 &&
            MEM32(person + 0x39) != param)
            goto L697;

        /* 0x0084a5e9 */
        cm_fn_0085e410((u32)(unsigned long)(S + 0x88));
        base = MEM32(CONT);
        STATEBYTE(2);
        rec = base + IDXOFF;
        a3 = MEM8(rec);
        bits = ((u32)MEM8(rec + 0x24) >> 1) & 0x0fu;
        r1 = cm_fn_0085fc50((u32)(unsigned long)(S + 0x88), person, param,
                            0xffffffffu, bits, a3, 0, 0);
        if ((long double)(s32)r1 == (long double)D_9689A0) {
            STATEBYTE(1);
            goto next;
        }
        /* 0x0084a64a */
        rec = MEM32(CONT) + IDXOFF;
        bits = ((u32)MEM8(rec + 0x24) >> 1) & 0x0fu;
        sres = cm_fn_005a0590(G_AE2A38, param);
        r2 = cm_fn_005968d0(sres, person, bits);
        scratch_value = MEMS32(S + 0x88 + 0x0c); /* 0x0084a66d, before stores */
        TMP32 = r2;
        STATEBYTE(1);
        if ((long double)(s32)r2 * (long double)D_968DE8 <
            (long double)scratch_value)
            goto next;

    L697:
        /* 0x0084a697 */
        pr = MEM16S(param + 0x80);
        r1 = (u32)cm_fn_0090cfc0((u32)pr * 2u + 1000u);
        r2 = (u32)cm_fn_0090cfc0(VALUE);
        if (!((s32)r2 > (s32)r1)) goto next;

        /* 0x0084a6c2 */
        base = CONT;                            /* 0x0084a6c2 */
        offset = IDXOFF;                         /* saved EDI, 0x0084a6c6 */
        rec = MEM32(base) + offset;              /* 0x0084a6ca */
        MEM8(rec + 0x24) = (u8)(MEM8(rec + 0x24) & 0xdf);
        cm_fn_008e5680((u32)(unsigned long)(S + 0x44));
        base = CONT;                            /* 0x0084a6e2 */
        OUT = 0;
        STATEBYTE(3);
        rec = MEM32(base) + offset;              /* 0x0084a6fa/6ff, saved EDI */
        r1 = cm_fn_008e5d60((u32)(unsigned long)(S + 0x44), person, param, rec,
                            MEM32(rec + 7), 0, (u32)(unsigned long)(S + 0x34));
        if (r1 == 0) goto L9d4;

        /* 0x0084a71b */
        cm_fn_008be300(G_DE1F54, (u32)(unsigned long)(S + 0x44), 0);
        q = OUT;
        if (q == 0) goto L9b4;
        rec = MEM32(CONT) + offset;
        a3 = MEM8(rec);
        a2 = MEM32(S + 0x68);
        a1 = MEM32(S + 0x64);
        cm_fn_004cc490(G_AE1050, (int)q, (int)a1, a2, (u8)a3, 0, 1, 0);
        q = cm_fn_004cf5a0(G_AE1050, person);
        G_AE1080 = q;
        if (q == 0) goto L9a3;
        club = G_AE23B4 + 581u * MEM32(q + 4);
        if (club == 0) goto L9a3;
        if (cm_fn_00600a50(club, 0, 1, 0, 0) != 0) goto L9a3;
        a2 = MEM32(club);
        a1 = MEM32(person);
        if (cm_fn_004cc9e0(G_AE1050, a1, a2) != 0)
            goto L9a3;
        r1 = cm_fn_004cf5f0(G_AE1050, person, club);
        if (r1 == 0) goto L9a3;
        a3 = (u32)MEM8(r1 + 0x4f) >> 4;
        if (a3 - 1u > 4u) goto L9a3;
        tx = MEM32(0x0084abc4u + (a3 - 1u) * 4u);
        if (tx == 0x0084a7f9u) {
            /* table entries 0,1 */
            r2 = cm_fn_00849ce0(THISV, club, person, 0, 0);
            if (r2 != 0) {
                pr = MEM16S(param + 0x80);
                div = 1;
                if (!(pr < 1)) div = pr;
                VALUE = (u32)div;
                tx = (u32)MEM8(G_AE2D0C + 79u * MEM32(person) + 0xb) & 0x0fu;
                t = (long double)(s32)tx * (long double)D_967880 +
                    (long double)pr / (long double)div;
                d = (double)t;
                if ((long double)D_967890 > (long double)d)
                    d = 1.0;
                else if ((long double)D_968DC0 < (long double)d)
                    d = 1.4;
                cm_fn_004cda70(G_AE1050, person, 0, 0, -1, d, 2);
            }
        } else {
            /* table entries 2,3,4 */
            r2 = cm_fn_00849ce0(THISV, club, person, 0, 0);
            if (r2 != 0) {
                pr = MEM16S(param + 0x80);
                S32(0x24) = (u32)pr;
                a1 = (u32)pr * 2u;
                div = 1;
                if (!((s32)a1 < 1)) div = (s32)a1;
                VALUE = (u32)div;
                tx = (u32)MEM8(G_AE2D0C + 79u * MEM32(person) + 0xb) & 0x0fu;
                t = (long double)(s32)tx * (long double)D_967880 +
                    (long double)pr / (long double)div +
                    (long double)D_968968;
                d = (double)t;
                if ((long double)D_967890 > (long double)d)
                    d = 1.0;
                else if ((long double)D_968978 < (long double)d)
                    d = 1.2;
                cm_fn_004cda70(G_AE1050, person, 0, 0, -1, d, 2);
            }
        }

    L9a3:
        q = OUT;
        if (q != 0) {
            cm_fn_00944c94(q);
        }
    L9b4:
        base = CONT;
        w = MEM32(S + 0x64);
        rec = MEM32(base);
        offset = IDXOFF;
        MEM32(rec + offset + 0x17) = w;
        if (ISV11 != 0) LIMIT = (u8)(LIMIT + 1);
    L9ef:
        STATEBYTE(1);
        cm_fn_008e5710((u32)(unsigned long)(S + 0x44));
        goto next;

    L9d4:
        rec = MEM32(CONT) + offset;
        cm_fn_00843e10(THISV, person, param, 1,
                       (u32)(MEM8(rec) != 0x0b));
        goto L9ef;

    L_aa05:
        /* 0x0084aa05 */
        q = MEM32(param + 0x53);
        if (q != 0) {
            if (MEM32(q) == G_9CF300 &&
                MEM8(param + 0x64) == 3)
                goto next;
        }
        if (cm_fn_008d0040(G_DE1F54, param, person, 0) == 0) goto next;
        q = cm_fn_004cf5a0(G_AE1050, person);
        G_AE1080 = q;
        if (q != 0) {
            club = G_AE23B4 + 581u * MEM32(q + 4);
            if (club != 0) {
                q = cm_fn_004cf5a0(G_AE1050, person);
                G_AE1080 = q;
                club = (q != 0) ? (G_AE23B4 + 581u * MEM32(q + 4)) : 0u;
                if (cm_fn_00600a50(club, 0, 1, 0, 0) != 0) {
                    q = cm_fn_004cf5a0(G_AE1050, person);
                    if ((MEM8(q + 0x4e) & 2) == 0) goto next;
                }
            }
        }
        if (cm_fn_008cdc90(G_DE1F54, person, param) != 0) goto next;
        if (cm_fn_008c5e40(G_DE1F54, person, 0) != 0) goto next;
        {
            pr = MEM16S(param + 0x80);
            r1 = (u32)cm_fn_0090cfc0((u32)(pr / 10));
            r2 = (u32)cm_fn_0090cfc0(MEM32(MEM32(CONT) + IDXOFF + 0x0b));
            if ((s32)r2 > (s32)r1) goto L_ab52;
            pr = MEM16S(param + 0x80);            /* 0x0084ab26 fresh word */
            r3 = (u32)cm_fn_0090cfc0((u32)pr * 2u);
            a1 = (u32)cm_fn_0090cfc0(MEM32(MEM32(CONT) + IDXOFF + 0x0b));
            if (!((s32)a1 > (s32)r3)) goto next;
        }
    L_ab52:
        r1 = cm_fn_00842740(THISV, person, param);
        base = CONT;                            /* 0x0084ab5f */
        rec = MEM32(base);                      /* 0x0084ab63 */
        offset = IDXOFF;                        /* 0x0084ab65, saved ECX */
        MEM32(rec + offset + 0x17) = r1;         /* 0x0084ab69 */
        base = CONT;                            /* 0x0084ab6d */
        rec = MEM32(base) + offset;              /* 0x0084ab71/73 */
        r1 = MEM32(rec + 0x17);                  /* 0x0084ab76 */
        if ((s32)r1 > -1) {
            LIMIT = (u8)(LIMIT + 1);
            goto next;
        }
        cm_fn_00843e10(THISV, person, param, 1,
                       (u32)(MEM8(rec) != 0x0b));

    next:
        COUNTER = (u8)(COUNTER + 1);
        if (!((s8)COUNTER < 0x28)) break;
    }

done:
    CM_FS(void *, 0) = seh.prev;
    return;
}
