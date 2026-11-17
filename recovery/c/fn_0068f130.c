/* cm_fn_0068f130: VA 0x0068f130, preferred image base 0x00400000, build 3.9.68.
   Plain C reconstruction from the original listing; callees and globals stay
   explicit boundaries.

   The five calls to 0x00945640 (MSVC __ftol) are NOT called here: each is the
   compiler lowering of a floating point to integer cast.  Every site is
   written as a (long long) conversion of a long double expression that follows
   the listing's x87 sequence (FILD/FADD/FMUL/FSUBR/FDIV) on the original
   constants, and the 64-bit result is narrowed the way the listing uses AX:
      0x0068f440  (long long)d18                    [FLD qword]
      0x0068f573  (long long)(t18 - (long double)r2) [FILD/FSUBR]
      0x0068f5fb  ditto
      0x0068f671  ditto
      0x0068f6e7  ditto
    Constant doubles/floats are read through CM_GLOBAL at their original VAs. */

#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_0053ca90(volatile u8 *self);
extern u32 CM_CDECL cm_fn_005409b0(u32 *input);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u8 CM_CDECL cm_fn_00544820(u32 *object);
extern short *CM_THISCALL cm_fn_00549f70(short *param_1, short param_2, char param_3,
                                         u16 param_4, u32 param_5);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person,
                                   u32 force);
extern void CM_THISCALL cm_fn_0069ba50(int *param_1, int *param_2, int *param_3);
extern u32 CM_THISCALL cm_fn_006a3770(u32 param_1, u32 param_2, u32 param_3);
/* 0x0090cfc0: each call site pushes exactly one dword and cleans four bytes;
   callers use EAX only. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

#define DAT_00adadfc CM_GLOBAL(u32, 0x00adadfc)
#define DAT_00ae235c CM_GLOBAL(s32, 0x00ae235c)
#define DAT_00ae2364 CM_GLOBAL(s32, 0x00ae2364)
#define DAT_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(u32, 0x00ae23bc)
#define DAT_00ae2c90 CM_GLOBAL(u32, 0x00ae2c90)

/* x87 constants, original image bytes. */
#define K_968878 CM_GLOBAL(double, 0x00968878)
#define K_968940 CM_GLOBAL(double, 0x00968940)
#define K_968968 CM_GLOBAL(double, 0x00968968)
#define K_968970 CM_GLOBAL(float, 0x00968970)
#define K_9689a0 CM_GLOBAL(double, 0x009689a0)
#define K_9689e0 CM_GLOBAL(double, 0x009689e0)
#define K_969040 CM_GLOBAL(double, 0x00969040)
#define K_9690b0 CM_GLOBAL(double, 0x009690b0)
#define K_96d1a4 CM_GLOBAL(float, 0x0096d1a4)
#define K_96d1a8 CM_GLOBAL(float, 0x0096d1a8)

/* The pointer field is re-read with a volatile load in every expansion: the
   original issues a fresh `mov reg,[esi]` before each narrow store, and the
   u16/u8 stores may overlap this u32 field, so a nonvolatile u32 read could be
   cached under strict aliasing (0x0068f2d2/2d7 and 0x0068f840..f855). */
#define WORD_SET(o, val) \
    do { base = *(volatile u32 *)param_1; *(u16 *)(base + off + (o)) = (u16)(val); } while (0)
#define DWORD_SET(o, val) \
    do { base = *(volatile u32 *)param_1; *(u32 *)(base + off + (o)) = (u32)(val); } while (0)
#define BYTE_SET(o, val) \
    do { base = *(volatile u32 *)param_1; *(u8 *)(base + off + (o)) = (u8)(val); } while (0)
#define BYTE_AND(o, m) \
    do { base = *(volatile u32 *)param_1; \
         *(u8 *)(base + off + (o)) = (u8)(*(u8 *)(base + off + (o)) & (m)); } while (0)

void CM_FASTCALL cm_fn_0068f130(int *param_1)
{
    u8 *p;
    u8 *q;
    u32 *vp;
    short *s;
    u32 base;
    u32 b;
    u32 club;
    u32 r;
    u32 u10;
    s16 v;
    s32 i;
    s32 j;
    s32 off;
    float f04;
    float f08;
    double d18;
    u8 buf[0x28]; /* L+0x20..L+0x47: 0x0068f2ac/0x0068f333/417c10 temporaries */

    u10 = 0; /* L+0x10 = 0 at 0x0068f150 */

    /* ---------------- pass 1: 0x0068f13a..0x0068f1b9 ---------------- */
    {
        u32 e;
        s32 k;

        e = DAT_00ae23bc;
        k = 0;
        if (DAT_00ae2364 > 0) {
            do {
                u32 m;
                u8 md;

                m = *(u32 *)(e + 0x24);
                if (m != 0) {
                    md = *(u8 *)(e + 0x28);
                    if ((s32)md - 5 == 0) goto L1a7;
                    if ((s32)md - 11 == 0) goto L1next;
                    if ((s32)md - 12 == 0) goto L1a7;
                    if (md == 0x0f || md == 0x0d) {
                        if (*(u32 *)(e + 0x61) == 0) {
                            *(u8 *)(e + 0x28) = 8;
                            goto L1clear;
                        }
                        *(u8 *)(e + 0x28) = 0x0b;
                    } else {
L1clear:
                        *(u32 *)(e + 0x24) = 0;
                    }
                    if (*(u32 *)(e + 0x39) == 0 && *(u8 *)(e + 0x3d) == 0)
                        *(u8 *)(e + 0x3d) = *(u8 *)(e + 0x28);
                    if (*(u8 *)(e + 0x28) != 0x0b)
                        *(u8 *)(e + 0x28) = 0;
                    goto L1next;
L1a7:
                    *(u32 *)(m + 0xcf) = e;
                }
L1next:
                k = k + 1;
                e = e + 0x6e;
            } while (k < DAT_00ae2364);
        }
    }

    /* ---------------- pass 2: 0x0068f1bb..0x0068f9b2 ---------------- */
    j = 0;
    if (DAT_00ae235c > 0) {
        p = (u8 *)(u32)DAT_00ae23b4;
        off = 0;
        do {
            r = cm_fn_0053b900((u32 *)p);
            if (r != 0) {
                if (*(u32 *)(p + 0x53) == 0) goto L994;
                if (*(u32 *)(*(u32 *)(p + 0x53) + 0x71) == 0) goto L994;
            }

            /* 0x0068f1f9 */
            if (cm_fn_005409b0((u32 *)p) != 0) {
                club = *(u32 *)(p + 0x53);
                if (club != 0) {
                    if ((s32)*(s16 *)(p + 0x80) >
                        (s32)*(s16 *)(club + 0x8e) * 200) {
                        *(s16 *)(p + 0x80) =
                            (s16)((s32)*(s16 *)(*(u32 *)(p + 0x53) + 0x8e) * 200);
                    }
                }
                *(u32 *)(p + 0xcf) = 0;
                *(u32 *)(p + 0xbf) = 0;
                *(u32 *)(p + 0xd3) = 0;
                *(u32 *)(p + 0xc3) = 0;
                *(u32 *)(p + 0xc7) = 0;
                *(u32 *)(p + 0xcb) = 0;
                *(u32 *)(p + 0x19f) = 0;
                *(u32 *)(p + 0x1a3) = 0;
                *(u32 *)(p + 0x1a7) = 0;
                *(u32 *)(p + 0x1ab) = 0;
                *(u32 *)(p + 0x1af) = 0;
                vp = (u32 *)(p + 0x1b3);
                for (i = 7; i != 0; i = i - 1) {
                    *vp = 0;
                    vp = vp + 1;
                }
                *(u32 *)(p + 0x1cf) = 0;
                *(u32 *)(p + 0x1d3) = 0;
                *(u32 *)(p + 0x1d7) = 0;
            }

            /* 0x0068f2a1: 549f70(ECX = L+0x20, 0x17, 6, 0x7b3, -1) */
            s = cm_fn_00549f70((short *)(void *)(buf + 0x00), (short)0x17, (char)6,
                               (u16)0x7b3, (u32)0xffffffff);
            base = *(volatile u32 *)param_1;
            cm_fn_00417c10((u32 *)(base + off + 0x41), (u32 *)(void *)(buf + 0x08),
                           (u16 *)s);
            WORD_SET(0x0e, 0);
            DWORD_SET(0x2c, 0);
            base = *(volatile u32 *)param_1;
            cm_fn_00417c10((u32 *)(base + off + 0x18), (u32 *)(void *)(buf + 0x10),
                           (u16 *)(void *)&DAT_00ae2c90);
            BYTE_SET(0, 0);
            base = *(volatile u32 *)param_1;
            *(double *)(base + off + 0x30) =
                (double)((long double)((s32)*(s16 *)(p + 0x80) *
                                       (s32)*(s16 *)(p + 0x80)) * K_9689e0);
            /* 0x0068f30e: one pointer-field load feeds both stores, no reload. */
            base = *(volatile u32 *)param_1;
            *(u32 *)(base + off + 0x38) = 0;
            *(u32 *)(base + off + 0x3c) = 0;

            /* 0x0068f318 */
            q = (u8 *)(u32)cm_fn_0053ca90(p);
            if (q == 0) {
                /* ---------- 0x0068f81b ---------- */
                r = cm_fn_00540a50((u32)p, (u32)(void *)&u10, 1);
                if (r == 0 || u10 == 0) {
                    WORD_SET(0x06, 0x1388);
                    WORD_SET(0x08, 0x1388);
                    WORD_SET(0x0a, 0x1388);
                    WORD_SET(0x0c, 0x1388);
                    WORD_SET(0x0e, 0x0010);
                    WORD_SET(0x10, 0);
                    WORD_SET(0x12, 0);
                    WORD_SET(0x14, 0);
                    WORD_SET(0x16, 0);
                    BYTE_AND(0x02, 0xfe);
                    BYTE_AND(0x02, 0xfd);
                    BYTE_AND(0x02, 0xfb);
                    BYTE_AND(0x02, 0xf7);
                    BYTE_AND(0x02, 0xef);
                    BYTE_SET(0x04, 0);
                    BYTE_AND(0x03, 0xe1);
                    BYTE_AND(0x02, 0xdf);
                    BYTE_AND(0x02, 0xbf);
                    BYTE_AND(0x02, 0x7f);
                    DWORD_SET(0x28, 0);
                    BYTE_SET(0x05, 0);
                    BYTE_AND(0x03, 0xfe);
                    BYTE_AND(0x40, 0x07);
                    BYTE_AND(0x40, 0xf8);
                    if (cm_fn_00544820((u32 *)p) != 0 ||
                        (cm_fn_0053b900((u32 *)p) == 0 && DAT_00adadfc != 0 &&
                         *(u32 *)(p + 0x57) != 0 &&
                         *(u32 *)(p + 0x53) != 0 &&
                         *(u32 *)(DAT_00adadfc +
                                 4 * *(u32 *)(*(u32 *)(p + 0x57))) != 0 &&
                         (*(u8 *)(*(u32 *)(p + 0x53) + 0x11c) & 3) != 0)) {
                        cm_fn_006a3770((u32)param_1, (u32)p, 1);
                        cm_fn_0069ba50(param_1, (int *)p, 0);
                    }
                }
            } else {
                /* ---------- 0x0068f328 ---------- */
                s = cm_fn_00549f70((short *)(void *)(buf + 0x18), (short)0x17,
                                   (char)7, (u16)0x7b3, (u32)0xffffffff);
                b = *(u32 *)p;
                base = *(volatile u32 *)param_1;
                cm_fn_00417c10((u32 *)(base + b * 73 + 0x20),
                               (u32 *)(void *)(buf + 0x20), (u16 *)s);

                q = (u8 *)(u32)cm_fn_0053ca90(p);
                if (*(u32 *)(q + 0x69) != 0) goto L38f;
                if (cm_fn_00600a50((u32)p, 0, 1, 0, 0) != 0) goto L38f;
                f08 = (float)(s32)*(s16 *)(p + 0x80);
                goto L466;

L38f:
                if (cm_fn_00600a50((u32)p, 0, 1, 0, 0) == 0) goto L3e7;
                if (*(u32 *)(p + 0xcf) == 0) goto L3da;
                q = (u8 *)(u32)cm_fn_0053ca90(p);
                if (*(u32 *)(q + 0x69) == 0) goto L3da;
                q = (u8 *)(u32)cm_fn_0053ca90(p);
                f08 = (float)(s32)*(s16 *)(*(u32 *)(q + 0x69) + 0x0a);
                goto L466;

L3da:
                f08 = 4500.0f;
                goto L4cf;

L3e7:
                q = (u8 *)(u32)cm_fn_0053ca90(p);
                b = *(u32 *)(q + 0x69);
                f08 = (float)(s32)*(s16 *)(b + 0x0a);
                d18 = (double)((long double)(s32)*(s16 *)(p + 0x80) * K_969040);
                q = (u8 *)(u32)cm_fn_0053ca90(p);
                b = *(u32 *)(q + 0x69);
                if ((long double)(s32)*(s16 *)(b + 0x0a) < (long double)d18) {
                    v = (s16)(long long)d18; /* 0x0068f440 (__ftol) */
                    q = (u8 *)(u32)cm_fn_0053ca90(p);
                    *(s16 *)(*(u32 *)(q + 0x69) + 0x0a) = (s16)v;
                }

L466:
                if (f08 == K_968970) {
                    if (*(u16 *)(p + 0x80) == 0) {
                        f08 = 5000.0f;
                    } else {
                        f08 = (float)((long double)(s32)*(s16 *)(p + 0x80) * K_969040);
                    }
                } else if (f08 < K_96d1a8) {
                    r = cm_fn_0090cfc0(1000);
                    f08 = (float)(s32)(r + 0x1f4);
                }

L4cf:
                {
                    long double D;
                    long double T;

                    T = (long double)(s32)*(s16 *)(p + 0x80) * K_968968 + K_9690b0;
                    if (T == (long double)K_9689a0) {
                        D = (long double)f08 * K_968940;
                    } else if (T < (long double)K_968878) {
                        r = cm_fn_0090cfc0(0x2ee);
                        D = (long double)(s32)(r + 0xfa);
                    } else {
                        D = T;
                    }
                    f04 = (float)(((long double)f08 / D) * (long double)K_96d1a4);
                }

                r = cm_fn_0090cfc0(0x1f4);
                {
                    float t18;

                    t18 = (float)((long double)(s32)r + (long double)f04);
                    r = cm_fn_0090cfc0(0x1f4);
                    /* 0x0068f573 (__ftol): FILD/FSUBR then ftol */
                    base = *(volatile u32 *)param_1;
                    *(s16 *)(base + off + 6) =
                        (s16)(long long)((long double)t18 - (long double)(s32)r);
                }
                /* 0x0068f57f: reload after the possibly overlapping word store.
                   CMP AX / JGE and JLE interpret the reloaded word as signed. */
                base = *(volatile u32 *)param_1;
                v = *(s16 *)(base + off + 6);
                if (v < 0x1676) {
                    r = cm_fn_0090cfc0(0x2ee);
                    WORD_SET(0x06, (s16)(s32)(r + 0x1676));
                } else if (v > 0x251c) {
                    r = cm_fn_0090cfc0(0x1f4);
                    WORD_SET(0x06, (s16)(u32)(10000 - r));
                }

                r = cm_fn_0090cfc0(0x1f4);
                {
                    float t18;

                    t18 = (float)((long double)(s32)r + (long double)f04);
                    r = cm_fn_0090cfc0(0x1f4);
                    base = *(volatile u32 *)param_1;
                    *(s16 *)(base + off + 8) =
                        (s16)(long long)((long double)t18 - (long double)(s32)r);
                }
                base = *(volatile u32 *)param_1; /* 0x0068f607 */
                v = *(s16 *)(base + off + 8);
                if (v < 0x1676) {
                    r = cm_fn_0090cfc0(0x1f4);
                    WORD_SET(0x08, (s16)(s32)(r + 0x1676));
                } else if (v > 0x2710) {
                    WORD_SET(0x08, 0x2710);
                }

                r = cm_fn_0090cfc0(0x6d6);
                {
                    float t18;

                    t18 = (float)((long double)(s32)r + (long double)f04);
                    r = cm_fn_0090cfc0(0x1f4);
                    base = *(volatile u32 *)param_1;
                    *(s16 *)(base + off + 0x0a) =
                        (s16)(long long)((long double)t18 - (long double)(s32)r);
                }
                base = *(volatile u32 *)param_1; /* 0x0068f67d */
                v = *(s16 *)(base + off + 0x0a);
                if (v < 0x157c) {
                    r = cm_fn_0090cfc0(0x0fa);
                    WORD_SET(0x0a, (s16)(s32)(r + 0x157c));
                } else if (v > 0x2710) {
                    WORD_SET(0x0a, 0x2710);
                }

                r = cm_fn_0090cfc0(0x9c4);
                {
                    float t18;

                    t18 = (float)((long double)(s32)r + (long double)f04);
                    r = cm_fn_0090cfc0(0x1f4);
                    base = *(volatile u32 *)param_1;
                    *(s16 *)(base + off + 0x0c) =
                        (s16)(long long)((long double)t18 - (long double)(s32)r);
                }
                base = *(volatile u32 *)param_1; /* 0x0068f6f3 */
                v = *(s16 *)(base + off + 0x0c);
                if (v < 0x157c) {
                    r = cm_fn_0090cfc0(0x0fa);
                    WORD_SET(0x0c, (s16)(s32)(r + 0x157c));
                } else if (v > 0x2710) {
                    WORD_SET(0x0c, 0x2710);
                }

                WORD_SET(0x10, 0);
                WORD_SET(0x12, 0);
                WORD_SET(0x14, 0);
                WORD_SET(0x16, 0);
                BYTE_AND(0x02, 0xfe);
                BYTE_AND(0x02, 0xfd);
                BYTE_AND(0x02, 0xfb);
                BYTE_AND(0x02, 0xf7);
                BYTE_AND(0x02, 0xef);
                BYTE_SET(0x04, 0);
                BYTE_AND(0x03, 0xe1);
                BYTE_AND(0x02, 0xdf);
                BYTE_AND(0x02, 0xbf);
                BYTE_AND(0x02, 0x7f);
                DWORD_SET(0x28, 0);
                BYTE_SET(0x05, 0);
                BYTE_AND(0x03, 0xfe);
                BYTE_AND(0x40, 0x07);
                BYTE_AND(0x40, 0xf8);
                cm_fn_006a3770((u32)param_1, (u32)p, 1);
            }

L994:
            j = j + 1;
            off = off + 0x49;
            p = p + 0x245;
        } while (j < DAT_00ae235c);
    }
    return;
}
