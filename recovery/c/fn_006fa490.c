/* fn_006fa490 — preferred-image VA 0x006fa490, image base 0x00400000, 3.9.68.
 *
 * Plain-C transliteration of the original listing (2222 bytes, `ret 8`,
 * ECX read before written -> CM_THISCALL with the object as the receiver).
 * Every block below is annotated with the VA it comes from; widths,
 * signedness, store order and call order follow the original instructions.
 */
#include "cmre.h"

typedef unsigned long long u64;
typedef signed long long s64;

extern void CM_THISCALL cm_fn_006bb9a0(u32 self, u16 a04, u32 a08, u32 a0c, u8 a10, u8 a14, u8 a18, u8 a1c, u8 a20, u8 a24);
extern u8 CM_THISCALL cm_fn_006cd490(u32 self, u8 param_1);
extern u32 CM_THISCALL cm_fn_006cfd70(u32 self, u32 a, u32 b, u32 c, u32 d);
extern void CM_THISCALL cm_fn_006d6f70(char *param_1,u16 param_2,char *param_3,char param_4,u8 param_5,char param_6, char param_7,u32 param_8);
extern float CM_THISCALL cm_fn_006f09b0(u32 self, u32 first, u32 second);
extern u8 CM_THISCALL cm_fn_006fad40(int param_1,u32 param_2,u32 param_3,char param_4,char *param_5, int param_6,u32 param_7,u32 param_8);
/* 0x0090cfc0 has one original stack argument (`push N` at every call site).
   The recovered boundary expects that pushed value in its first declared
   parameter; the trailing two slots are not read by the callee (witness
   traces match for every call site with them zeroed). */
extern u64 CM_CDECL cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);

/* `fld dword ptr [index*4 + 0x00a15e44]` (0x006fa558, 0x006fabd5,
   0x006fac01): a float table addressed by the signed byte coordinates. */
static float cm_006fa490_tbl(u32 index)
{
    return *(float *)(0x00a15e44u + index * 4u);
}

/* `sub eax,K; cdq; xor eax,edx; sub eax,edx; movsx eax,al`
   (0x006fa536..0x006fa53e and the identical copies at 0x006fabab,
   0x006fabc5, 0x006fabe1, 0x006fabf3): |v| truncated to a byte and
   sign extended again. */
static s32 cm_006fa490_abs8(s32 v)
{
    u32 a = (v < 0) ? ((u32)0 - (u32)v) : (u32)v;

    return (s32)(s8)(u8)a;
}

void CM_THISCALL cm_fn_006fa490(u32 self, char arg1, char arg2)
{
    /* The original frame is `sub esp,0x14` plus four saved registers, so the
       listing's [esp+disp] addresses map onto these byte offsets of one
       scratch block:
         L+2  L+3  L+4  L+5  flag bytes (0x006fa4a2..0x006fa4b8 stores)
         L+7                scratch byte whose address is argument 5 of
                            0x006fad40; set to 1 at 0x006fab54
         L+8                first coordinate, read as a DWORD at 0x006fab29
         L+0xc              second coordinate, read as a DWORD at 0x006fab2d
         L+0x10             action/message id, DWORD store/read
       L+9..L+0xb are never written by the original and stay uninitialized.
       The four flags live in locals: the only aliasing byte stores into L+5
       and L+6 (0x006fa6bb, 0x006fa79c, 0x006fa8b7, 0x006fa9ae) are register
       spills that the listing reloads immediately, and no path reads a flag
       after the spill that overwrites it. */
    union { u8 b[0x14]; u32 w[5]; } L;
    u8 f2, f3, f4, f5;
    u8 bl;
    s16 w19a;
    s8 m;
    u8 v;
    u32 found;
    u32 ca, cb, msgv;
    long double fx;

    bl = 0;
    f2 = 0;                                     /* [esp+0x12] */
    f3 = 0;                                     /* [esp+0x13] */
    f5 = 0;                                     /* [esp+0x15] */
    f4 = 0;                                     /* [esp+0x14] */

    w19a = *(s16 *)(self + 0x19a);              /* 0x006fa49b */

    /* 0x006fa4b8..0x006fa4e9 */
    if (w19a < 0x32 || w19a > 0x136) {
        if (*(s8 *)(self + 0x27) != 1) {
            f4 = 1;                             /* 0x006fa4d3 */
            goto L_4d8;
        }
        if (w19a > 0x136) {
            f2 = 1;                             /* 0x006fa4cc */
            goto L_4d8;
        }
        goto L_4e4;                             /* 0x006fa4ca */
    }
L_4d8:
    if (w19a > 0x28 && w19a < 0x8c)
        goto L_4e4;
    goto L_4e9;
L_4e4:
    f3 = 1;                                     /* 0x006fa4e4 */
L_4e9:
    /* 0x006fa4e9..0x006fa523 */
    if (w19a < 0x83 || w19a > 0xe5) {
L_50f:
        if (!f2 && w19a > 0xdc && w19a < 0x140)
            goto L_523;
        goto L_528;
    }
    if (*(s8 *)(self + 0x27) != 0) {            /* 0x006fa4f5 */
        bl = 1;
        f5 = 1;                                 /* 0x006fa50b */
        goto L_50f;
    }
    if (w19a <= 0xb4) {                         /* 0x006fa4fc */
        f3 = 1;                                 /* 0x006fa502 */
        goto L_50f;
    }
L_523:
    f2 = 1;                                     /* 0x006fa523 */
L_528:
    /* 0x006fa528..0x006fa56c: one float from the table, or 0x006f09b0(4,0) */
    if (*(s8 *)(self + 0x27) == 0) {
        s32 ix = cm_006fa490_abs8((s32)*(s8 *)(self + 0x103) - 0xb);
        s32 iy = cm_006fa490_abs8((s32)*(s8 *)(self + 0x102) - 4);

        fx = (long double)cm_006fa490_tbl((u32)(ix * 9 + iy));
    }
    else {
        fx = (long double)cm_fn_006f09b0(self, 4, 0);
    }
    /* 0x006fa56c: fcomp qword ptr [0x009688a0]; fnstsw ax; test ah,1 */
    /* 0x006fa56c..0x006fa57d: x87 C0 selects less-than or unordered. */
    if (!(fx >= (long double)CM_GLOBAL(double, 0x009688a0))) {
        s8 m102 = *(s8 *)(self + 0x102);        /* 0x006fa57f */

        if ((m102 < 2 || m102 > 6) && (u8)arg1 != 4) {
            /* 0x006fa592..0x006fa5d4: action 0x5e */
            L.w[4] = 0x5e;
            if (*(s8 *)(self + 0x27) == 0) {
                v = (u8)cm_fn_0090cfc0(2, 0, 0); /* 0x006fa5a3 */
                L.b[8] = (u8)(0xb - v);
            }
            else {
                v = (u8)cm_fn_0090cfc0(2, 0, 0); /* 0x006fa5b5 */
                L.b[8] = v;
            }
            v = (u8)cm_fn_0090cfc0(3, 0, 0);     /* 0x006fa5c3 */
            L.b[0xc] = (u8)(v + 3);
            goto L_aa15;
        }
    }

    /* 0x006fa5d9..0x006fa5ff */
    m = *(s8 *)(self + 0x27);
    if (m == 1 && *(s8 *)(self + 0x103) >= 6)
        goto L_7f6;
    if (m == 0 && *(s8 *)(self + 0x103) < 6)
        goto L_7f6;
    if ((u8)arg1 != 4)
        goto L_6eb;

    /* 0x006fa605..0x006fa676: action 0x5c, first coordinate */
    L.w[4] = 0x5c;
    if (bl != 0) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea8);
        v = (u8)(v - (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa61f */
        v = (u8)(v - 1);
        goto L_676;
    }
    if (f4) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea8);
        v = (u8)((u8)cm_fn_0090cfc0(2, 0, 0) + v); /* 0x006fa643 */
        v = (u8)(v + 1);
        L.b[8] = v;
        goto L_67a;
    }
    v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea8);
    v = (u8)(v - (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa663 */
    v = (u8)(v + (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa66c */
L_676:
    L.b[8] = v;                                 /* 0x006fa676 */
    /* 0x006fa67a..0x006fa6d5: action 0x5c, second coordinate */
L_67a:
    if (f2) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
        v = (u8)(v - (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa690 */
        L.b[0xc] = (u8)(v - 1);
        goto L_aa20;
    }
    if (f3) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
        v = (u8)((u8)cm_fn_0090cfc0(2, 0, 0) + v); /* 0x006fa6bf */
        L.b[0xc] = (u8)(v + 1);
        goto L_aa20;
    }
    v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7); /* 0x006fa6e0 */
    v = (u8)(v - (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa8e5 */
    v = (u8)(v + (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa8ee */
    L.b[0xc] = v;                               /* 0x006fa8f8 */
    goto L_aa20;

    /* 0x006fa6eb..0x006fa75d: action 0x5d, first coordinate */
L_6eb:
    L.w[4] = 0x5d;
    if (bl != 0) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea8);
        v = (u8)(v - (u8)cm_fn_0090cfc0(4, 0, 0)); /* 0x006fa705 */
        v = (u8)(v - 2);
        goto L_75d;
    }
    if (f4) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea8);
        v = (u8)((u8)cm_fn_0090cfc0(4, 0, 0) + v); /* 0x006fa72a */
        v = (u8)(v + 2);
        L.b[8] = v;
        goto L_761;
    }
    v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea8);
    v = (u8)(v - (u8)cm_fn_0090cfc0(3, 0, 0)); /* 0x006fa74a */
    v = (u8)(v + (u8)cm_fn_0090cfc0(3, 0, 0)); /* 0x006fa753 */
L_75d:
    L.b[8] = v;                                 /* 0x006fa75d */
    /* 0x006fa761..0x006fa7d4: action 0x5d, second coordinate */
L_761:
    if (f2) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
        v = (u8)(v - (u8)cm_fn_0090cfc0(4, 0, 0)); /* 0x006fa777 */
        v = (u8)(v - 2);
        goto L_7d6;
    }
    if (f3) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
        v = (u8)((u8)cm_fn_0090cfc0(4, 0, 0) + v); /* 0x006fa7a0 */
        v = (u8)(v + 2);
        goto L_7d6;
    }
    v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
    v = (u8)(v - (u8)cm_fn_0090cfc0(3, 0, 0)); /* 0x006fa7c3 */
    v = (u8)(v + (u8)cm_fn_0090cfc0(3, 0, 0)); /* 0x006fa7cc */
L_7d6:
    L.b[0xc] = v;                               /* 0x006fa7d9 */
    if (*(s8 *)(self + 0x27) == 0)
        goto L_9f3;
    if (f5)
        goto L_aa15;
    goto L_aa0b;

    /* 0x006fa7f6..0x006fa871: action 0x5b, first coordinate */
L_7f6:
    if ((u8)arg1 != 4)
        goto L_901;
    L.w[4] = 0x5b;
    if (bl != 0) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea8);
        v = (u8)(v - (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa819 */
        v = (u8)(v - 2);
        goto L_871;
    }
    if (f4) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea8);
        v = (u8)((u8)cm_fn_0090cfc0(2, 0, 0) + v); /* 0x006fa83e */
        v = (u8)(v + 2);
        L.b[8] = v;
        goto L_875;
    }
    v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea8);
    v = (u8)(v - (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa85e */
    v = (u8)(v + (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa867 */
L_871:
    L.b[8] = v;                                 /* 0x006fa871 */
    /* 0x006fa875..0x006fa8fc: action 0x5b, second coordinate */
L_875:
    if (f2) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
        v = (u8)(v - (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa88b */
        L.b[0xc] = (u8)(v - 2);
        goto L_aa20;
    }
    if (f3) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
        v = (u8)((u8)cm_fn_0090cfc0(2, 0, 0) + v); /* 0x006fa8bb */
        L.b[0xc] = (u8)(v + 2);
        goto L_aa20;
    }
    v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
    v = (u8)(v - (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa8e5 */
    v = (u8)(v + (u8)cm_fn_0090cfc0(2, 0, 0)); /* 0x006fa8ee */
    L.b[0xc] = v;                               /* 0x006fa8fc */
    goto L_aa20;

    /* 0x006fa901..0x006fa96f: computed action id, first coordinate */
L_901:
    {
        u32 wld = *(u32 *)(self + 0x19e);
        u8 a9 = *(u8 *)(wld + 0x8ea9);          /* 0x006fa907 */

        /* inc cl; neg cl; sbb ecx,ecx; and ecx,0xffffffcb; add ecx,0x3e */
        L.w[4] = ((u8)(a9 + 1) != 0) ? 9u : 0x3eu;
        if (bl != 0) {
            v = *(u8 *)(wld + 0x8ea8);          /* 0x006fa921 */
            v = (u8)(v - (u8)cm_fn_0090cfc0(5, 0, 0)); /* 0x006fa929 */
            v = (u8)(v - 4);
            goto L_96f;
        }
        v = *(u8 *)(wld + 0x8ea8);              /* 0x006fa93c */
        if (f4) {
            v = (u8)((u8)cm_fn_0090cfc0(5, 0, 0) + v); /* 0x006fa948 */
            v = (u8)(v + 4);
            L.b[8] = v;                         /* 0x006fa954 */
            goto L_973;
        }
        v = (u8)(v - (u8)cm_fn_0090cfc0(3, 0, 0)); /* 0x006fa95c */
        v = (u8)(v + (u8)cm_fn_0090cfc0(3, 0, 0)); /* 0x006fa965 */
    }
L_96f:
    L.b[8] = v;                                 /* 0x006fa96f */
    /* 0x006fa973..0x006fa9e6: computed action id, second coordinate */
L_973:
    if (f2) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
        v = (u8)(v - (u8)cm_fn_0090cfc0(5, 0, 0)); /* 0x006fa989 */
        v = (u8)(v - 3);
        goto L_9e8;
    }
    if (f3) {
        v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
        v = (u8)((u8)cm_fn_0090cfc0(5, 0, 0) + v); /* 0x006fa9b2 */
        v = (u8)(v + 3);
        goto L_9e8;
    }
    v = *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7);
    v = (u8)(v - (u8)cm_fn_0090cfc0(3, 0, 0)); /* 0x006fa9d5 */
    v = (u8)(v + (u8)cm_fn_0090cfc0(3, 0, 0)); /* 0x006fa9de */
L_9e8:
    L.b[0xc] = v;                               /* 0x006fa9eb */
    if (*(s8 *)(self + 0x27) != 0)
        goto L_aa03;
L_9f3:
    if (f4)
        goto L_aa15;
    L.b[8] = (u8)(L.b[8] + 2);                  /* 0x006fa9fb */
    goto L_aa15;
L_aa03:
    if (f5)
        goto L_aa15;
L_aa0b:
    L.b[8] = (u8)(L.b[8] + 0xfe);               /* 0x006faa0b */
    goto L_aa15;

L_aa15:
    /* 0x006faa15: cmp byte ptr [esp+0x28],0xff */
    if ((u8)arg1 == 0xff)
        goto L_ab26;

L_aa20:
    /* 0x006faa20..0x006faa63 */
    if ((u8)arg2 != 0)
        goto L_aa69;
    if (*(s8 *)(self + 0x10b) != 0)
        goto L_aa44;
    if ((u32)cm_fn_0090cfc0(3, 0, 0) == 0)     /* 0x006faa34 */
        goto L_ab26;
L_aa44:
    m = *(s8 *)(self + 0x27);                   /* 0x006faa44 */
    if (m == 0) {
        if (*(s8 *)(self + 0x103) < 2)
            goto L_ab26;
    }
    if (m != 1)
        goto L_aa69;
    if (*(s8 *)(self + 0x103) > 9)
        goto L_ab26;

L_aa69:
    /* 0x006faa69..0x006fab02: pick 1 or 7 and walk the loop variable */
    if (*(s8 *)(self + 0x27) != 0)
        goto L_aabb;
    if (f2) {
        v = 1;
    }
    else if (f3) {
        v = 7;
    }
    else {
        v = ((u32)cm_fn_0090cfc0(2, 0, 0) != 0) ? 1 : 7; /* 0x006faa86 */
    }
    {
        s32 c = 0xa;

        for (;;) {
            s32 d = c - (s32)*(s8 *)(self + 0x103);

            if (d <= 5) {
                if ((u8)arg1 != 4)
                    break;
                if (d <= 3)
                    break;
            }
            c = (s32)(s8)(u8)((u8)c - 1);
        }
        L.b[0xc] = (u8)(((s32)(s8)v + (s32)(s8)L.b[0xc]) / 2); /* 0x006fab0f */
        L.b[8] = (u8)((c + (s32)(s8)L.b[8]) / 2);              /* 0x006fab22 */
    }
    goto L_ab26;

L_aabb:
    if (f2) {
        v = 1;
    }
    else if (f3) {
        v = 7;
    }
    else {
        v = ((u32)cm_fn_0090cfc0(2, 0, 0) != 0) ? 1 : 7; /* 0x006faacd */
    }
    {
        s32 c = 1;

        for (;;) {
            s32 d = (s32)*(s8 *)(self + 0x103) - c;

            if (d <= 5) {
                if ((u8)arg1 != 4)
                    break;
                if (d <= 3)
                    break;
            }
            c = (s32)(s8)(u8)((u8)c + 1);
        }
        L.b[0xc] = (u8)(((s32)(s8)v + (s32)(s8)L.b[0xc]) / 2); /* 0x006fab0f */
        L.b[8] = (u8)((c + (s32)(s8)L.b[8]) / 2);              /* 0x006fab22 */
    }

L_ab26:
    /* 0x006fab26..0x006fab59 */
    ca = L.w[2];
    cb = L.w[3];
    msgv = L.w[4];
    *(u8 *)(self + 0x2b) = (u8)(*(u8 *)(self + 0x2b) + 1);
    L.b[7] = 1;
    if (cm_fn_006fad40((int)self,
                       (u32)(u8)*(u8 *)(self + 0x102),
                       (u32)(u8)*(u8 *)(self + 0x103),
                       (char)msgv, (char *)&L.b[7], 0, cb, ca) == 0)
        goto L_ac7c;

    /* 0x006fab66: cmp bx,0x36 */
    if ((u16)msgv == 0x36)
        goto L_ac46;
    found = cm_fn_006cfd70(*(u32 *)(self + 0x19e),
                           (u32)(u8)*(u8 *)(self + 0x27),
                           (u32)(u8)*(u8 *)(self + 0x103), ca, 0);
    if (found == 0)
        goto L_ac46;

    /* 0x006fab93..0x006fac1e */
    {
        u8 mobj = *(u8 *)(found + 0x27);
        s32 c103 = (s32)*(s8 *)(found + 0x103);
        s32 c102 = (s32)*(s8 *)(found + 0x102);
        s32 xbase = cm_006fa490_abs8(c102 - 4);
        s32 ia, ib;
        float fa, fb;
        long double t;
        s64 ll;

        if (mobj == 0) {
            ia = xbase + 9 * cm_006fa490_abs8(c103 - 0xb);
            ib = xbase + 9 * cm_006fa490_abs8(c103 - 0xb);
        }
        else {
            ia = xbase + 9 * cm_006fa490_abs8(c103);
            ib = xbase + 9 * cm_006fa490_abs8(c103);
        }
        fa = cm_006fa490_tbl((u32)ia);          /* 0x006fabd5 */
        fb = cm_006fa490_tbl((u32)ib);          /* 0x006fac01 */
        /* fmul st(1); fadd st(0),st(0) */
        t = (long double)fb * (long double)fa;
        t = t + t;
        /* call 0x00945640 (__ftol): MSVC's (long long) cast.  The EAX half
           of the 64-bit result is the `push eax` argument of 0x0090cfc0. */
        ll = (s64)t;
        if ((u32)cm_fn_0090cfc0((u32)ll, 0, 0) != 0) /* 0x006fac14 */
            goto L_ac86;
    }
    /* 0x006fac20..0x006fac41 */
    cm_fn_006bb9a0(*(u32 *)(self + 0x19e), (u16)0xffffu, found, 0, 0, 8,
                   *(u8 *)(found + 0x102), *(u8 *)(found + 0x103),
                   0xffu, 0xffu);

L_ac46:
    /* 0x006fac46..0x006fac77 */
    {
        u32 wld = *(u32 *)(self + 0x19e);
        s8 a, b;

        if (*(s8 *)(wld + 0x8eb2) != 0)       /* 0x006fac54 */
            goto L_ac7c;
        a = *(s8 *)(wld + 0x8ea7);            /* 0x006fac56 */
        if (a < 0)                            /* 0x006fac5e */
            goto L_ac73;
        b = *(s8 *)(wld + 0x8ea8);            /* 0x006fac60 */
        if (b < 0)                            /* 0x006fac68 */
            goto L_ac73;
        if (a >= 9)                           /* 0x006fac6c */
            goto L_ac73;
        if (b < 12)                           /* 0x006fac71 */
            goto L_ac7c;
    }
L_ac73:
    cm_fn_006cd490(*(u32 *)(self + 0x19e), (u8)*(u8 *)(self + 0x27));
L_ac7c:
    return;

L_ac86:
    /* 0x006fac86..0x006fad3b */
    cm_fn_006d6f70((char *)(u32)*(u32 *)(*(u32 *)(self + 0x19e) + 0x8f7a),
                   (u16)0x1f78u, (char *)(u32)*(u32 *)(self + 0x19e),
                   *(s8 *)(found + 0x2e), (u8)*(u8 *)(self + 0x27),
                   (char)0xff, (char)0xff, 0);
    {
        s32 k = (s32)*(s8 *)(self + 0x27);

        *(u8 *)(*(u32 *)(self + 0x19e) + 0xf5ca + k * 0x36) =
            (u8)(*(u8 *)(*(u32 *)(self + 0x19e) + 0xf5ca + k * 0x36) + 1);
    }
    *(u8 *)(found + 0x13) = (u8)(*(u8 *)(found + 0x13) + 1);
    *(u8 *)(self + 1) = (u8)(*(u8 *)(self + 1) - 1);
    *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea7) = *(u8 *)(found + 0x102);
    *(u8 *)(*(u32 *)(self + 0x19e) + 0x8ea8) = *(u8 *)(found + 0x103);
    *(u8 *)(*(u32 *)(self + 0x19e) + 0x8eb2) = 7;
    *(u8 *)(*(u32 *)(self + 0x19e) + 0x8eae) =
        (u8)(*(s8 *)(self + 0x27) == 0);
    *(u32 *)(*(u32 *)(self + 0x19e) + 0xf582) = 0;
    return;
}
