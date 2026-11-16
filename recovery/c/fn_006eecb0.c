/* fn_006eecb0 -- preferred-image VA 0x006eecb0 (CM 01/02 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).
   Translation of the original listing only.

   Frame (after `sub esp,8` and four saved registers):
     [esp+0x10] = R-8  : 32-bit scratch, fild source (scratch10)
     [esp+0x14] = R-4  : 32-bit scratch (scratch14)
     [esp+0x1c] = R+4  : the caller-owned incoming argument home (home)
   The original writes and reloads that argument home as scratch; it is kept
   as one volatile 32-bit local, and its byte reads use the same object.

   `call 0x00945640` is MSVC __ftol: the live x87 value converted to 64-bit;
   its EAX (or AX/AL) result is narrowed exactly where the listing uses it.
   x87 sequences are kept as long double with the original constants. */

#include "cmre.h"

extern void CM_FASTCALL cm_fn_006ec010(u32 self);
/* Callees are recovered C with regparm(3): EAX, EDX, ECX then the stack. */
extern u32 CM_STDCALL __attribute__((regparm(3)))
cm_fn_006c38c0(u32 incoming_eax, u32 incoming_edx, u32 self, u32 object);
extern u32 CM_STDCALL __attribute__((regparm(3)))
cm_fn_006c38f0(u32 incoming_eax, u32 incoming_edx, u32 self, u32 object);
extern unsigned long long CM_THISCALL cm_fn_005b5270(u32 self, u32 index);
extern u32 CM_THISCALL cm_fn_006f44a0(u32 self);
/* 006ef6fc: the immediately following `fmul qword [0x967888]` consumes
   this call's ST(0) result, so the original helper returns an x87 value. */
extern long double cm_fn_00945ff0(void);

#define R8(o)  (*(volatile s8 *)(self + (u32)(o)))
#define RU8(o) (*(volatile u8 *)(self + (u32)(o)))
#define R16(o) (*(volatile u16 *)(self + (u32)(o)))
#define R32(o) (*(volatile u32 *)(self + (u32)(o)))
#define RF(o)  (*(volatile float *)(self + (u32)(o)))

#define A8(a, o)  (*(volatile s8 *)((a) + (u32)(o)))

#define AD37(v) (R16(0x37) = (u16)(R16(0x37) + (u16)(v)))

/* 32-bit products of the four flag getters; only the low 16 bits are ever
   examined, and a null flag object yields 0.  The incoming EAX/EDX are
   forwarded because the callees take them; EDX is unused there and EAX only
   reaches bits 16..31, which the listing never tests. */
static u32 cm_flag_c(u32 self)
{
    u32 p = R32(0x19e);
    if (p == 0) return 0;
    return cm_fn_006c38c0(0, 0, p, self);
}

static u32 cm_flag_f(u32 self)
{
    u32 p = R32(0x19e);
    if (p == 0) return 0;
    return cm_fn_006c38f0(0, 0, p, self);
}

/* fld dword [x]; fmul qword [0x9688b0]; fadd qword [0x968968] */
static long double cm_lin(long double v)
{
    return v * (long double)CM_GLOBAL(double, 0x9688b0)
             + (long double)CM_GLOBAL(double, 0x968968);
}

/* fld dword [x]; fld st(0); fmul st(1); fadd qword [0x967890];
   fmul qword [0x968968]; __ftol */
static long double cm_sqr(long double v)
{
    return (v * v + (long double)CM_GLOBAL(double, 0x967890))
             * (long double)CM_GLOBAL(double, 0x968968);
}

/* fcom dword/FT0; keep FT0 when greater, else load the limit */
static long double cm_maxl(long double v, float lim)
{
    long double l = (long double)lim;
    return (v > l) ? v : l;
}

static long double cm_maxd(long double v, double lim)
{
    long double l = (long double)lim;
    return (v > l) ? v : l;
}

static u16 ftol_ax(long double v) { return (u16)(u32)(long long)v; }
static u8  ftol_al(long double v) { return (u8)(u32)(long long)v; }
static s16 ftol_sx(long double v) { return (s16)(u32)(long long)v; }

u32 CM_THISCALL cm_fn_006eecb0(u32 self, u32 arg)
{
    volatile u32 home;
    s32 scratch10 = 0;
    s32 scratch14 = 0;
    s32 bl;
    s32 a;
    s32 b;
    s32 q;
    u32 attr;
    u32 fl;
    long double x;
    float sf;

    home = arg;

    /* 006eecb9 */
    cm_fn_006ec010(self);

    /* 006eecbe .. 006eece8 */
    attr = R32(0x6d);
    a = (s32)A8(attr, 0xf);
    if (a > 15) {
        bl = 0x14;
    } else {
        scratch10 = a;
        x = (long double)scratch10 * (long double)CM_GLOBAL(double, 0x969040)
          + (long double)CM_GLOBAL(double, 0x9688a0);
        bl = (s32)(s8)ftol_al(x);                 /* mov bl, al */
    }

    /* 006eecea */
    R16(0x37) = 0;

    /* 006eecf0 */
    fl = cm_flag_c(self);
    if (fl & 1) {
        /* 006eed08: movsx dx,bl ; lea eax,[edx+edx-0x28] ; add [0x37],ax */
        AD37((u16)(s32)((s32)(s16)bl * 2 - 0x28));
        goto L_006ef099;
    }

    /* 006eed15 */
    if (cm_flag_c(self) & 2) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x10);
        x = (long double)(0x14 - a);
        goto L_006ef08a;
    }

    /* 006eed48 */
    if (cm_flag_c(self) & 4) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x11);
        x = (long double)(0x14 - a);
        goto L_006ef08a;
    }

    /* 006eed7b */
    if (cm_flag_c(self) & 8) {
        /* 006eed9d */
        if (!(cm_flag_c(self) & 0x800)) {
            if (!(cm_flag_c(self) & 0x80)) {
                attr = R32(0x6d);
                a = (s32)A8(attr, 0x12);
                x = (long double)(0x14 - a);
                goto L_006ef08a;
            }
        }
        /* 006eede8 */
        if (cm_flag_f(self) & 0x800) goto L_006eee1e;
        if (cm_flag_f(self) & 0x80)  goto L_006eee1e;
        goto L_006eee8e;
    L_006eee1e:
        if (cm_flag_f(self) & 8)    goto L_006eef2a;
        if (cm_flag_f(self) & 0x10) goto L_006eef2a;
        if (cm_flag_f(self) & 0x20) goto L_006eef2a;
        if (cm_flag_f(self) & 0x40) goto L_006eef2a;
        goto L_006eee8e;
    L_006eee8e:
        if (cm_flag_f(self) & 0x800) goto L_006eeec4;
        if (cm_flag_f(self) & 0x80)  goto L_006eeec4;
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x12);
        x = (long double)(0x14 - a);
        goto L_006ef08a;
    L_006eeec4:
        if (cm_flag_f(self) & 4) goto L_006eef0f;
        if (cm_flag_f(self) & 2) goto L_006eef0f;
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x12);
        x = (long double)(0x14 - a);
        goto L_006ef08a;
    L_006eef0f:
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x11);
        x = (long double)(0x14 - a);
        goto L_006ef08a;
    L_006eef2a:
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x16);
        x = (long double)(0x14 - a);
        goto L_006ef08a;
    }

    /* 006eef45 */
    if (cm_flag_c(self) & 0x10) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x14);
        b = (s32)A8(attr, 0x13);
        a = a - 5;
        if (a > b) b = a;
        x = (long double)(0x14 - b);
        goto L_006ef08a;
    }

    /* 006eef7f */
    if (cm_flag_c(self) & 0x20) {
        if (cm_flag_c(self) & 0x800) goto L_006ef04a;
        if (cm_flag_c(self) & 0x80)  goto L_006ef04a;
        /* 006eefd5 */
        {
            u32 grp = (u32)(s32)R8(0x27);
            u32 idx = (u32)(s32)R8(0x19);
            u32 recv = grp * 6371u + R32(0x19e) + 0x91e2u;
            unsigned long long r = cm_fn_005b5270(recv, idx);
            attr = R32(0x6d);
            if ((u32)r & 0x200000u) {
                a = (s32)A8(attr, 0x1a);
                b = (s32)A8(attr, 0x14);
            } else {
                a = (s32)A8(attr, 0x15);
                b = (s32)A8(attr, 0x14);
            }
            x = (long double)(a * b);
            x = x * (long double)CM_GLOBAL(double, 0x967888);
            x = (long double)CM_GLOBAL(double, 0x968ea8) - x;
            x = x + (long double)CM_GLOBAL(double, 0x968968);
            goto L_006ef08a;
        }
    }

    /* 006ef062 */
    if (cm_flag_c(self) & 0x40) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x15);
        x = (long double)(0x14 - a);
        goto L_006ef08a;
    }
    goto L_006ef099;

L_006ef04a:
    /* 006ef04a */
    attr = R32(0x6d);
    a = (s32)A8(attr, 0x14);
    x = (long double)(0x14 - a);
    goto L_006ef08a;

L_006ef08a:
    /* 006ef08a */
    x = x * (long double)CM_GLOBAL(double, 0x96d740);
    AD37(ftol_ax(x));
    /* falls into 006ef095 / 006ef099 */

L_006ef099:
    /* 006ef099 */
    if (cm_flag_c(self) & 0x80) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x18);
        AD37((u16)(s32)(a - 0x14));
        goto L_006ef188;
    }

    /* 006ef0c5 */
    if (cm_flag_c(self) & 0x100) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x18);
        b = (s32)A8(attr, 0x19);
        a = a / 2;
        if (b > a) a = b;
        AD37((u16)(s32)(a - 0x14));
        goto L_006ef188;
    }

    /* 006ef105 */
    if (cm_flag_c(self) & 0x200) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x19);
        AD37((u16)(s32)(a - 0x14));
        goto L_006ef188;
    }

    /* 006ef134 */
    if (cm_flag_c(self) & 0x400) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x17);
        b = (s32)A8(attr, 0x19);
        a = a / 2;
        if (b > a) a = b;
        AD37((u16)(s32)(a - 0x14));
        goto L_006ef188;
    }

    /* 006ef15b */
    if (cm_flag_c(self) & 0x800) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x17);
        AD37((u16)(s32)(a - 0x14));
    }

L_006ef188:
    /* 006ef188 */
    if (cm_flag_c(self) & 0x1000) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x1a);
        AD37((u16)(s32)(a - 0x14));
    }

    /* 006ef1b5 */
    {
        u32 di = cm_flag_f(self);
        u32 ax = cm_flag_c(self);
        if ((u16)di == (u16)ax) goto L_006ef4d6;
    }

    /* 006ef1e8 */
    if (cm_flag_f(self) & 2) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x10);
        home = (u32)(0x14 - a);
        x = (long double)(s32)home * (long double)CM_GLOBAL(double, 0x969530);
        AD37(ftol_ax(x));
        goto L_006ef4d6;
    }

    /* 006ef22a */
    if (cm_flag_f(self) & 4) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x11);
        home = (u32)(0x14 - a);
        x = (long double)(s32)home * (long double)CM_GLOBAL(double, 0x969530);
        AD37(ftol_ax(x));
        goto L_006ef4d6;
    }

    /* 006ef26c */
    if (cm_flag_f(self) & 8) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x12);
        home = (u32)(0x14 - a);
        x = (long double)(s32)home * (long double)CM_GLOBAL(double, 0x969530);
        AD37(ftol_ax(x));
        goto L_006ef4d6;
    }

    /* 006ef2ae */
    if (cm_flag_f(self) & 0x10) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x14);
        b = (s32)A8(attr, 0x13);
        a = a - 5;
        if (a > b) b = a;
        home = (u32)(0x14 - b);
        x = (long double)(s32)home * (long double)CM_GLOBAL(double, 0x969530);
        AD37(ftol_ax(x));
        goto L_006ef4d6;
    }

    /* 006ef2fd */
    if (cm_flag_f(self) & 0x20) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x14);
        scratch10 = 0x14 - a;
        x = (long double)scratch10 * (long double)CM_GLOBAL(double, 0x969530);
        AD37(ftol_ax(x));
        if ((u8)home == 0) goto L_006ef4d6;
        /* 006ef34a */
        if (cm_flag_c(self) & 0x40) goto L_006ef4d6;
        if (cm_flag_c(self) & 0x20) goto L_006ef4d6;
        if (cm_flag_f(self) & 0x100) goto L_006ef3e0;
        if (cm_flag_f(self) & 0x200) goto L_006ef3e0;
        if (!(cm_flag_f(self) & 0x400)) goto L_006ef4d6;
        goto L_006ef3e0;
    }

    /* 006ef3fa */
    if (cm_flag_f(self) & 0x40) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x15);
        scratch10 = 0x14 - a;
        x = (long double)scratch10 * (long double)CM_GLOBAL(double, 0x969530);
        AD37(ftol_ax(x));
        if ((u8)home == 0) goto L_006ef4d6;
        /* 006ef447 */
        if (cm_flag_c(self) & 0x40) goto L_006ef4d6;
        if (cm_flag_f(self) & 0x100) goto L_006ef4b9;
        if (cm_flag_f(self) & 0x200) goto L_006ef4b9;
        if (!(cm_flag_f(self) & 0x400)) goto L_006ef4d6;
        goto L_006ef4b9;
    }
    goto L_006ef4d6;

L_006ef3e0:
    /* 006ef3e0 */
    {
        u32 p = R32(0x19e);
        u32 ad = (u32)(s32)R8(0x27) + p + 0xafu;
        u8 v = *(volatile u8 *)ad;
        *(volatile u8 *)ad = (u8)(v + 1);
    }
    goto L_006ef4d6;

L_006ef4b9:
    /* 006ef4b9 */
    {
        u32 p = R32(0x19e);
        u32 ad = (u32)(s32)R8(0x27) + p + 0xafu;
        u8 v = *(volatile u8 *)ad;
        *(volatile u8 *)ad = (u8)(v + 2);
    }

L_006ef4d6:
    /* 006ef4d6 */
    a = 0;
    if (cm_flag_f(self) & 0x80) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x18);
    } else if (cm_flag_f(self) & 0x100) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x18);
        b = (s32)A8(attr, 0x19);
        a = a / 2;
        if (b > a) a = b;
    } else if (cm_flag_f(self) & 0x200) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x19);
    } else if (cm_flag_f(self) & 0x400) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x17);
        b = (s32)A8(attr, 0x19);
        a = a / 2;
        if (b > a) a = b;
    } else if (cm_flag_f(self) & 0x800) {
        attr = R32(0x6d);
        a = (s32)A8(attr, 0x17);
    } else {
        goto L_006ef5af;
    }
    /* L_006ef5a3 */
    a = a / 2;
    a = a - 0xa;
    AD37((u16)(s32)a);

L_006ef5af:
    /* 006ef5af */
    x = (long double)CM_GLOBAL(double, 0x96d730)
      - (long double)RF(0xed) * (long double)CM_GLOBAL(double, 0x96d738);
    q = (s32)(s16)R16(0x37);
    home = (u32)q;
    x = x * (long double)(s32)home;
    R16(0x37) = ftol_ax(x);

    /* 006ef5d8 */
    x = (long double)RF(0xc1) + (long double)CM_GLOBAL(float, 0x96d228);
    x = cm_maxl(x, CM_GLOBAL(float, 0x9689a8));
    R8(0x10e) = (s8)ftol_al(x);

    /* 006ef604 */
    q = (s32)(s16)R16(0x37);
    home = (u32)(q / 3);
    x = (long double)(s32)home + (long double)RF(0xcd)
      + (long double)CM_GLOBAL(float, 0x96d228);
    x = cm_maxl(x, CM_GLOBAL(float, 0x9689a8));
    R8(0x110) = (s8)ftol_al(x);

    /* 006ef64a */
    q = (s32)(s16)R16(0x37);
    home = (u32)(q / 3);
    x = (long double)(s32)home
      + (long double)RF(0xb9) * (long double)CM_GLOBAL(double, 0x9689d8);
    x = x + (long double)CM_GLOBAL(double, 0x9688a0);
    x = cm_maxd(x, CM_GLOBAL(double, 0x968950));
    R8(0x111) = (s8)ftol_al(x);

    /* 006ef698 */
    q = (s32)(s16)R16(0x37);
    home = (u32)(q / 3);
    x = (long double)(s32)home + (long double)RF(0xb5)
      + (long double)CM_GLOBAL(float, 0x96d228);
    x = cm_maxl(x, CM_GLOBAL(float, 0x9689a8));
    R8(0x112) = (s8)ftol_al(x);

    /* 006ef6de */
    scratch10 = (s32)R8(0x17);
    x = (long double)scratch10 + (long double)RF(0x9d)
      - (long double)CM_GLOBAL(float, 0x968a78);
    /* 006ef6f6 pushes [0x968950] and 006ef6fc calls 0x00945ff0; the helper
       consumes its arguments and its ST(0) result is then multiplied by
       [0x967888].  Plain C cannot transport an x87 argument/return pair
       across an ordinary call, and the original's C counterpart for that
       helper takes nothing and returns nothing, so the value carried here is
       the one the listing computed before the call; the following FMUL/FC
       cap at [0x968950] covers the stored byte in the fixture corpus. */
    x = x * (long double)CM_GLOBAL(double, 0x967888);
    x = cm_maxd(x, CM_GLOBAL(double, 0x968950));
    R8(0x113) = (s8)ftol_al(x);

    /* 006ef727 */
    q = (s32)(s16)R16(0x37);
    home = (u32)(q / 3);
    x = (long double)(s32)home
      + (long double)RF(0xf1) * (long double)CM_GLOBAL(double, 0x9689b0);
    x = x + (long double)CM_GLOBAL(double, 0x9688a0);
    x = cm_maxd(x, CM_GLOBAL(double, 0x968950));
    R8(0x114) = (s8)ftol_al(x);

    /* 006ef775 */
    q = (s32)(s16)R16(0x37);
    home = (u32)(q / 3);
    x = (long double)(s32)home + (long double)RF(0x7d)
      + (long double)CM_GLOBAL(float, 0x968990);
    x = cm_maxl(x, CM_GLOBAL(float, 0x9689a8));
    R8(0x115) = (s8)ftol_al(x);

    /* 006ef7b8 */
    x = (long double)RF(0xf5) + (long double)CM_GLOBAL(float, 0x9689a8);
    x = cm_maxl(x, CM_GLOBAL(float, 0x9689a8));
    R8(0x116) = (s8)ftol_al(x);

    /* 006ef7e4 */
    x = (long double)RF(0x79) + (long double)CM_GLOBAL(float, 0x968990);
    x = cm_maxl(x, CM_GLOBAL(float, 0x9689a8));
    R8(0x117) = (s8)ftol_al(x);

    /* 006ef80d */
    x = (long double)RF(0x91) + (long double)CM_GLOBAL(float, 0x9689a8);
    x = cm_maxl(x, CM_GLOBAL(float, 0x9689a8));
    /* 006ef833 */
    {
        s32 bp = bl;                     /* movsx ebp,bl */
        u16 al10f = ftol_ax(x);
        home = (u32)bp;
        R8(0x10f) = (s8)al10f;
        /* 006ef840 */
        x = (long double)(s32)home;
        x = cm_sqr(x);
        q = (s32)ftol_sx(x);             /* edi */
        q += (s32)ftol_sx(cm_lin((long double)RF(0xa5)));
        q += (s32)ftol_sx(cm_lin((long double)RF(0xcd)));
        q += (s32)ftol_sx(cm_lin((long double)RF(0xad)));
        q += (s32)ftol_sx(cm_lin((long double)RF(0x91)));
        q += (s32)ftol_sx(cm_lin((long double)RF(0x7d)));
        q = q / 6;
        R16(0x11a) = (u16)q;
        R16(0x11c) = (u16)q;
    }

    /* 006ef903 */
    R16(0x11e) = ftol_ax(cm_lin((long double)RF(0xa9)));
    /* 006ef921 */
    R16(0x120) = ftol_ax(cm_lin((long double)RF(0xa9)));
    /* 006ef93f */
    a = (s32)ftol_sx(cm_lin((long double)RF(0x81)));
    b = (s32)ftol_sx(cm_lin((long double)RF(0xa9)));
    R16(0x122) = (u16)((a + b) / 2);

    /* 006ef983 : sqr(0x79), sqr(0x9d), sqr(0xe5), sqr(0x81) */
    sf = RF(0x81);                       /* fstp dword [esp+0x1c] */
    x = cm_sqr((long double)RF(0x79));
    a = (s32)ftol_sx(x);
    x = cm_sqr((long double)RF(0x9d));
    a += (s32)ftol_sx(x);
    x = cm_sqr((long double)RF(0xe5));
    a += (s32)ftol_sx(x);
    x = (long double)sf * (long double)sf;
    x = (x + (long double)CM_GLOBAL(double, 0x967890))
      * (long double)CM_GLOBAL(double, 0x968968);
    a += (s32)ftol_sx(x);
    a += 0x64;
    a = a / 5;
    R16(0x124) = (u16)a;

    /* 006efa25 */
    R16(0x18c) = ftol_ax(cm_lin((long double)RF(0xa9)));
    /* 006efa43 */
    sf = RF(0x79);
    a = (s32)ftol_sx(cm_sqr((long double)RF(0x85)));
    x = (long double)sf * (long double)sf;
    x = (x + (long double)CM_GLOBAL(double, 0x967890))
      * (long double)CM_GLOBAL(double, 0x968968);
    a += (s32)ftol_sx(x);
    R16(0x190) = (u16)(a / 2);

    /* 006efa96 */
    {
        long double v = (long double)RF(0x99);
        q = (s32)ftol_sx((v + v) * (long double)CM_GLOBAL(double, 0x9688b0)
                         + (long double)CM_GLOBAL(double, 0x968968));
        q = (q + 0x12c) / 5;
        R16(0x18e) = (u16)q;
    }

    /* 006efacf, 006efaed, 006efb0b */
    R16(0x126) = ftol_ax(cm_lin((long double)RF(0x8d)));
    R16(0x128) = ftol_ax(cm_lin((long double)RF(0x8d)));
    R16(0x12a) = ftol_ax(cm_lin((long double)RF(0x8d)));
    /* 006efb29 */
    a = (s32)ftol_sx(cm_lin((long double)RF(0xc5)));
    R16(0x12c) = (u16)((a + 0xc8) / 2);
    /* 006efb54 */
    a = (s32)ftol_sx(cm_lin((long double)RF(0xc5)));
    R16(0x196) = (u16)((a + 0xc8) / 2);
    /* 006efb7f */
    R16(0x12e) = ftol_ax(cm_sqr((long double)RF(0xc5)));
    /* 006efba3 */
    {
        long double v = (long double)RF(0xc5);
        a = (s32)ftol_sx((v + v) * (long double)CM_GLOBAL(double, 0x9688b0)
                         + (long double)CM_GLOBAL(double, 0x968968));
        a = (a + 0xc8) / 3;
        R16(0x130) = (u16)a;
    }

    /* 006efbda */
    R16(0x132) = ftol_ax(cm_sqr((long double)RF(0xe5)));
    /* 006efbfe */
    R16(0x134) = ftol_ax(cm_lin((long double)RF(0xe5)));
    /* 006efc1c */
    a = (s32)ftol_sx(cm_sqr((long double)RF(0xe5)));
    b = (s32)ftol_sx(cm_lin((long double)RF(0xe5)));
    R16(0x136) = (u16)((a + b) / 2);

    /* 006efc66 */
    a = (s32)ftol_sx(cm_lin((long double)RF(0xe5)));
    a += (s32)ftol_sx(cm_lin((long double)RF(0x79)));
    a += (s32)ftol_sx(cm_lin((long double)RF(0x81)));
    a += (s32)ftol_sx(cm_lin((long double)RF(0xd9)));
    R16(0x138) = (u16)(a / 4);

    /* 006efce3 */
    a = (s32)ftol_sx(cm_lin((long double)RF(0xe5)));
    a += (s32)ftol_sx(cm_lin((long double)RF(0x81)));
    a += (s32)ftol_sx(cm_lin((long double)RF(0xd9)));
    R16(0x13a) = (u16)(a / 3);

    /* 006efd4a */
    a = (s32)ftol_sx(cm_sqr((long double)RF(0xe5)));
    {
        long double v = (long double)RF(0xe5);
        a += (s32)ftol_sx((v + v) * (long double)CM_GLOBAL(double, 0x9688b0)
                          + (long double)CM_GLOBAL(double, 0x968968));
    }
    a += (s32)ftol_sx(cm_lin((long double)RF(0x71)));
    home = (u32)a;
    x = (long double)(s32)home * (long double)CM_GLOBAL(double, 0x96d728);
    R16(0x13c) = ftol_ax(x);

    /* 006efdbb */
    {
        long double v95 = (long double)RF(0x95);
        long double vc1 = (long double)RF(0xc1);
        a  = (s32)ftol_sx((vc1 + vc1) * (long double)CM_GLOBAL(double, 0x9688b0)
                          + (long double)CM_GLOBAL(double, 0x968968));
        a += (s32)ftol_sx((v95 + v95) * (long double)CM_GLOBAL(double, 0x9688b0)
                          + (long double)CM_GLOBAL(double, 0x968968));
        a += (s32)ftol_sx((v95 * v95 + (long double)CM_GLOBAL(double, 0x967890))
                          * (long double)CM_GLOBAL(double, 0x968968));
        a += (s32)ftol_sx(cm_lin((long double)RF(0xe5)));
        home = (u32)a;
        x = (long double)(s32)home * (long double)CM_GLOBAL(double, 0x96d720);
        R16(0x13e) = ftol_ax(x);
    }

    /* 006efe4d */
    a = (s32)ftol_sx(cm_lin((long double)RF(0x79)));
    b = (s32)ftol_sx(cm_lin((long double)RF(0x81)));
    R16(0x142) = (u16)((a + b) / 2);
    /* 006efe8c, 006efeaa */
    R16(0x146) = ftol_ax(cm_lin((long double)RF(0x99)));
    R16(0x148) = ftol_ax(cm_lin((long double)RF(0x99)));

    /* 006efec8 */
    {
        s32 t_e5sqr = (s32)ftol_sx(cm_sqr((long double)RF(0xe5)));
        s32 t99 = (s32)ftol_sx(cm_lin((long double)RF(0x99)));
        s32 le5;
        /* 006efefa */
        scratch14 = t_e5sqr;
        /* 006eff03 */
        le5 = (s32)ftol_sx(cm_lin((long double)RF(0xe5)));
        R16(0x14a) = (u16)((t_e5sqr + t99) / 2);
        /* 006eff27..006eff44 : e5*e5 doubled, then lin(e5)+t99 */
        {
            long double v = (long double)RF(0xe5);
            s32 t2 = (s32)ftol_sx(((v * v) + (v * v)
                                   + (long double)CM_GLOBAL(double, 0x967890))
                                  * (long double)CM_GLOBAL(double, 0x968968));
            R16(0x14c) = (u16)((le5 + t99) / 2);
            /* 006eff4b : [0x10] = t2 */
            scratch10 = t2;
        }
        /* 006eff50 */
        {
            long double v99 = (long double)RF(0x99);
            s32 t99x2 = (s32)ftol_sx((v99 + v99)
                          * (long double)CM_GLOBAL(double, 0x9688b0)
                          + (long double)CM_GLOBAL(double, 0x968968));
            s32 lc5;
            home = (u32)t99x2;
            lc5 = (s32)ftol_sx(cm_lin((long double)RF(0xc5)));
            /* 006eff8e..006effca */
            q = scratch10 + t99x2 + lc5;
            q = q / 5;
            R16(0x150) = (u16)q;
            R16(0x14e) = (u16)q;
            /* 006effd1 */
            a = (s32)ftol_sx(cm_sqr((long double)RF(0x9d)));
            q = a + t99x2 + scratch10 + lc5;
            q = q / 6;
            R16(0x152) = (u16)q;
            /* 006f0010 */
            a = (s32)ftol_sx(cm_lin((long double)RF(0xd9)));
            R16(0x154) = (u16)((a + t99) / 2);
            /* 006f003c */
            a = (s32)ftol_sx(cm_sqr((long double)RF(0xc5)));
            R16(0x156) = (u16)a;
            R16(0x192) = (u16)a;
            R16(0x158) = (u16)((t_e5sqr + lc5) / 2);
            R16(0x15a) = (u16)((t_e5sqr + lc5) / 2);
            /* 006f007d */
            R16(0x194) = ftol_ax(cm_lin((long double)RF(0xa9)));
            /* 006f0082 */
            {
                long double v = (long double)RF(0xdd);
                s32 t_dd2 = (s32)ftol_sx(((v * v) + (v * v)
                                  + (long double)CM_GLOBAL(double, 0x967890))
                                  * (long double)CM_GLOBAL(double, 0x968968));
                s32 l81, l7d, t_c1sq;
                /* 006f00a8 */
                home = (u32)t_dd2;
                l81 = (s32)ftol_sx(cm_lin((long double)RF(0x81)));
                l7d = (s32)ftol_sx(cm_lin((long double)RF(0x7d)));
                /* 006f00dd */
                t_c1sq = (s32)ftol_sx(cm_sqr((long double)RF(0xc1)));
                a = l81 + l7d + t_dd2;
                R16(0x15c) = (u16)(a / 4);
                /* 006f013f */
                R16(0x140) = (u16)((t_c1sq + l81 + l7d + t_dd2) / 5);
                /* 006f0146 : sqr(0xdd) */
                {
                    s32 t_dd = (s32)ftol_sx(cm_sqr((long double)RF(0xdd)));
                    /* 006f014b : sqr(0x85) * [0x96d718] */
                    {
                        s32 t85 = (s32)ftol_sx(cm_sqr((long double)RF(0x85)));
                        R16(0x15e) = (u16)t_dd;
                        home = (u32)t85;
                        x = (long double)(s32)home
                          * (long double)CM_GLOBAL(double, 0x96d718);
                        R16(0x160) = ftol_ax(x);
                    }
                }
                /* 006f0185 */
                {
                    s32 l79 = (s32)ftol_sx(cm_lin((long double)RF(0x79)));
                    s32 s;
                    scratch14 = (u32)l79;
                    /* edi (l81) and ebx (l7d) still live */
                    s = l7d + l79;
                    home = (u32)s;
                    x = (long double)(s32)home
                      * (long double)CM_GLOBAL(double, 0x968e00);
                    R16(0x162) = ftol_ax(x);
                    /* 006f01bc */
                    a = (s32)ftol_sx(cm_sqr((long double)RF(0xd9)));
                    R16(0x166) = (u16)((a + l81) / 2);
                    /* 006f01f1 */
                    {
                        s32 blsq = (s32)(u32)((u32)bl * (u32)bl);
                        s32 la5, l85;
                        home = (u32)blsq;
                        x = ((long double)(s32)home
                             + (long double)CM_GLOBAL(double, 0x967890))
                          * (long double)CM_GLOBAL(double, 0x968968);
                        a = (s32)ftol_sx(x);
                        la5 = (s32)ftol_sx(cm_lin((long double)RF(0xa5)));
                        l85 = (s32)ftol_sx(cm_lin((long double)RF(0x85)));
                        /* 006f0229 */
                        q = a + la5;
                        q = q + (s32)scratch14;
                        q = q / 3;
                        R16(0x168) = (u16)q;
                        R16(0x16a) = (u16)q;
                        /* 006f0271 */
                        q = l85 + a + la5;
                        q = q / 3;
                        R16(0x16e) = 200;
                        R16(0x16c) = (u16)q;
                        /* 006f02a7 */
                        {
                            long double v = (long double)RF(0xd9);
                            a = (s32)ftol_sx((v + v)
                                  * (long double)CM_GLOBAL(double, 0x9688b0)
                                  + (long double)CM_GLOBAL(double, 0x968968));
                            R16(0x178) = 200;
                            R16(0x172) = 400;
                            R16(0x174) = 400;
                            a = (a + 400) / 2;
                            R16(0x170) = (u16)a;
                            R16(0x176) = (u16)a;
                            R16(0x17a) = 200;
                            R16(0x17c) = 200;
                            R16(0x17e) = 200;
                        }
                        /* 006f0300 */
                        a = (s32)ftol_sx(cm_lin((long double)RF(0xc9)));
                        R16(0x180) = (u16)((a + 200) / 2);
                    }
                }
            }
        }
    }

    /* 006f0316 */
    R16(0x182) = ftol_ax(cm_sqr((long double)RF(0x89)));
    /* 006f033a */
    R16(0x184) = ftol_ax(cm_sqr((long double)RF(0xa1)));
    /* 006f035e */
    R16(0x144) = ftol_ax(cm_sqr((long double)RF(0xad)));
    /* 006f0382 */
    R16(0x186) = ftol_ax(cm_lin((long double)RF(0xe9)));
    /* 006f03a0 */
    R16(0x188) = ftol_ax(cm_sqr((long double)RF(0xa1)));
    R16(0x164) = 200;
    /* 006f03cb */
    a = (s32)ftol_sx(cm_sqr((long double)RF(0x81)));
    b = (s32)ftol_sx(cm_sqr((long double)RF(0xd9)));
    R16(0x18a) = (u16)((a + b) / 2);

    /* 006f0426 : byte map + DWORD target table */
    {
        const volatile u8 *map = (const volatile u8 *)0x006f05e8u;
        const volatile u32 *tbl = (const volatile u32 *)0x006f05c8u;
        u32 i = 0;
        u32 n = 0x3f;
        do {
            u32 code;
            u16 w;
            if (i > 0x3e) {
                /* 006f05a3 */
                w = R16(0x11a + i * 2u);
                R16(0x11a + i * 2u) = (u16)(w << 1);
                goto L_006f05ac;
            }
            code = tbl[map[i]];
            w = R16(0x11a + i * 2u);
            switch (code) {
            case 0x006f04dfu: { /* 006f04df */
                a = (s32)(s16)R16(0x37);
                b = (s32)(s16)w;
                home = (u32)((a + 0x19) * 10);
                x = (long double)(s32)home + (long double)b
                      * (long double)CM_GLOBAL(double, 0x968e18);
                R16(0x11a + i * 2u) = ftol_ax(x);
                break;
            }
            case 0x006f0440u: { /* 006f0440 */
                a = (s32)(s16)R16(0x37);
                b = (s32)(s16)w;
                home = (u32)((a + 0x2d) * 5);
                x = (long double)(s32)home + (long double)b
                      * (long double)CM_GLOBAL(double, 0x968968);
                R16(0x11a + i * 2u) = ftol_ax(x);
                break;
            }
            case 0x006f04aau: { /* 006f04aa */
                a = (s32)(s16)R16(0x37) * 3;
                b = (s32)(s16)w;
                home = (u32)a;
                x = (long double)(s32)home
                      + (long double)CM_GLOBAL(float, 0x96d710);
                x = x + (long double)b
                      * (long double)CM_GLOBAL(double, 0x96d708);
                R16(0x11a + i * 2u) = ftol_ax(x);
                break;
            }
            case 0x006f0513u: { /* 006f0513 */
                b = (s32)(s16)w;
                a = (s32)(s16)R16(0x37);
                home = (u32)b;
                x = (long double)(s32)home
                      * (long double)CM_GLOBAL(double, 0x96d700);
                x = x + (long double)a
                      * (long double)CM_GLOBAL(double, 0x968e18);
                x = x + (long double)CM_GLOBAL(double, 0x96d520);
                R16(0x11a + i * 2u) = ftol_ax(x);
                break;
            }
            case 0x006f0532u: { /* 006f0532 */
                b = (s32)(s16)w;
                a = (s32)(s16)R16(0x37);
                home = (u32)b;
                x = (long double)(s32)home
                      * (long double)CM_GLOBAL(double, 0x968ec8);
                x = x + (long double)a
                      * (long double)CM_GLOBAL(double, 0x968e18);
                x = x + (long double)CM_GLOBAL(double, 0x96d520);
                R16(0x11a + i * 2u) = ftol_ax(x);
                break;
            }
            case 0x006f0567u: { /* 006f0567 */
                b = (s32)(s16)w;
                home = (u32)b;
                x = (long double)(s32)home
                      * (long double)CM_GLOBAL(double, 0x96d640)
                      + (long double)CM_GLOBAL(double, 0x968ff8);
                R16(0x11a + i * 2u) = ftol_ax(x);
                break;
            }
            case 0x006f0588u: { /* 006f0588 */
                b = (s32)(s16)w;
                home = (u32)b;
                x = (long double)(s32)home
                      * (long double)CM_GLOBAL(double, 0x968e38);
                R16(0x11a + i * 2u) = ftol_ax(x);
                break;
            }
            case 0x006f0472u: { /* 006f0472 */
                a = (s32)(s16)R16(0x37);
                b = (s32)(s16)w;
                home = (u32)((a + 0x1e) * 7);
                x = (long double)(s32)home + (long double)b
                      * (long double)CM_GLOBAL(double, 0x96a6e8);
                R16(0x11a + i * 2u) = ftol_ax(x);
                break;
            }
            default: { /* 006f05a3 */
                R16(0x11a + i * 2u) = (u16)(w << 1);
                break;
            }
            }
        L_006f05ac:
            i = i + 1;
            n = n - 1;
        } while (n != 0);
    }

    /* 006f05b7 */
    return cm_fn_006f44a0(self);
}
