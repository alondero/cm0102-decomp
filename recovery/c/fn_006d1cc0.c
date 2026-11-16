/* fn_006d1cc0 - preferred-image VA 0x006d1cc0 (CM0102 3.9.68, base 0x00400000).
   Plain-C reconstruction from the original listing.  Callees stay explicit
   boundaries, globals keep their original VA and width, and every memory
   access / call follows the listing order.  Entry keeps the original ABI:
   ECX/EDX are incoming values the routine forwards (EDX is pushed at
   0x006d1dbe), the single stack argument is the routine's object base, and the
   original ends in a bare `ret`. */
#include "cmre.h"

#define G_2364 CM_GLOBAL(volatile u32, 0x00ae2364)
#define G_23B4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define G_23BC CM_GLOBAL(volatile u32, 0x00ae23bc)
#define G_1050 CM_GLOBAL(volatile u32, 0x00ae1050)
#define G_2C74 CM_GLOBAL(volatile u32, 0x00ae2c74)
#define G_7DC8 CM_GLOBAL(volatile u32, 0x00b67dc8)

#define K_967880 CM_GLOBAL(double, 0x00967880)
#define K_968918 CM_GLOBAL(double, 0x00968918)
#define K_968E40 CM_GLOBAL(double, 0x00968e40)
#define K_968E48 CM_GLOBAL(double, 0x00968e48)

#define RB(p) (*(volatile u8 *)(u32)(p))
#define RW(p) (*(volatile s16 *)(u32)(p))
#define RD(p) (*(volatile u32 *)(u32)(p))
#define RF(p) (*(volatile float *)(u32)(p))

extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern long long CM_CDECL cm_fn_00544410(volatile u32 record, volatile u32 adjustment);
extern u32 CM_CDECL cm_fn_00547790(u32 first, u32 second);
extern u32 CM_CDECL cm_fn_005477d0(u32 first, u32 second);
extern u32 CM_CDECL cm_fn_00547d30(u32 object, u32 target);
extern u32 CM_CDECL cm_fn_00547d60(u32 object, u32 target);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in,
                                                     const u8 *src, s16 count);
/* The caller observes this callee's EDX: 0x006d1dba keeps only the low byte
   (mov dl,[eax+0x17]) and 0x006d1dbd pushes the whole register, so the return
   is read as 64 bits (EAX:EDX). */
extern unsigned long long CM_THISCALL cm_fn_0068d1b0(u32 self);
extern void cm_fn_006d26b0(int param_1, int param_2, int param_3, int param_4,
                           int *param_5, float param_6, u32 param_7, char param_8);
extern void CM_CDECL cm_fn_006d3610(u32 output, u32 input, u32 other, u32 record,
                                    float amount, u8 flag, u8 adjust);
extern u32 CM_CDECL cm_fn_006d3de0(u32 object, u32 side, float input, u32 which,
                                   u32 optional);
/* Original call sites push one argument and carry the other two inputs in EAX
   and EDX (e.g. 0x006d1e15 loads EAX, 0x006d1e12 computes EDX, 0x006d1e22
   calls); the binding therefore passes them in those registers. */
extern unsigned long long __attribute__((regparm(2)))
    cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);

/* 0x006d2543: signed multiply by 0x2e8ba2e9, high half, sar 1, sign fix-up
   (truncating division by 11). */
static s32 cm_div11(s32 x)
{
    long long p = (long long)x * (long long)0x2e8ba2e9;
    s32 hi = (s32)((unsigned long long)p >> 32);
    s32 t = hi >> 1;
    t = t + (s32)((u32)t >> 31);
    return t;
}

/* 25-byte message record built at original frame +0x38 and handed to
   0x0068d0d0 by pointer. */
struct Msg {
    u32 f00;   /* +00 */
    u32 f04;   /* +04 */
    u32 f08;   /* +08 */
    u8 b0c;    /* +0c */
    u8 b0d;    /* +0d */
    u8 b0e;    /* +0e */
    u32 f0f;   /* +0f */
    u16 f13;   /* +13 */
    u8 tail[4];/* +15 */
} __attribute__((packed));

/* Original scratch frame (sub esp,0x44 plus four saved registers). */
struct Frame {
    u8 pad0[0x13];
    u8 inner;                    /* +13 inner counter (dl) */
    union { u32 w; u8 b; } outer;/* +14 outer counter (al) */
    u32 sum;                     /* +18 */
    u32 slot1c;                  /* +1c */
    u32 slot20;                  /* +20 */
    u32 slot24;                  /* +24 */
    float f28;                   /* +28 */
    u32 slot2c;                  /* +2c */
    double d30;                  /* +30 */
    struct Msg msg;              /* +38 */
    u8 pad2[3];                  /* +51 */
} __attribute__((packed));

void __attribute__((regparm(2))) cm_fn_006d1cc0(u32 eax_in, u32 edx_in, u32 obj)
{
    struct Frame F;
    u32 edx = edx_in;
    u32 ebx = 0xffffffffu;
    u32 A, B, rec, rp = 0, d3, R, res, px;
    u32 r1, r2, r3, r4, r5, r6, r7, ehi;
    u32 k;
    s32 found;
    u32 grp, entry, target;
    unsigned long long u64v;
    long long ll;
    s16 di;
    u8 al_v, cl_v;
    u32 ep = 0;

    (void)eax_in;

    /* ------------------------- phase 1 ------------------------- */
    A = RD(obj + 0x478e);
    if (A == 0) goto P2;
    if (G_7DC8 == 0) goto P2;
    B = RD(obj + 0x4792);
    if (B == 0) goto P2;
    edx = (edx & 0xffffff00u) | (u32)RB(B + 0x40);
    if (RB(B + 0x40) == 0) goto P2;
    {
        u32 v = RD(A + 2);
        F.outer.b = 0;
        RD(A + 0x0a) = v;
    }
    if (RW(A + 0) <= 0) goto P2;

L1LOOP:
    u64v = cm_fn_0068d1b0(A);
    rec = (u32)u64v;
    edx = (u32)(u64v >> 32);
    if (rec == 0) goto P2;
    di = RW(rec + 0x13);
    if (di == 0x82) goto L2032;
    if (di != 0x1e) goto L1F67;
    if (RB(rec + 0x0d) == 0) goto L1E73;

    /* ---- rec[0xd] != 0 : 0x006d1d57 ---- */
    {
        u8 b43 = RB(B + 0x43);
        u8 b44 = RB(B + 0x44);
        edx = (edx & 0xffffff00u) | (u32)b44;
        if ((s8)b44 < (s8)b43) goto L1D9E;
        if ((u8)edx != b43) goto L2045;
        {
            u32 p20 = RD(B + 0x20);
            s32 v20 = (s16)RW(p20 + 0x80);
            u32 p1c = RD(B + 0x1c);
            s32 v1c;
            edx = p1c;
            v1c = (s16)RW(p1c + 0x80);
            if (!((long double)v1c <
                  (long double)v20 * (long double)K_968E40))
                goto L2045;
        }
    }
L1D9E:
    {
        u32 p20 = RD(B + 0x20);
        d3 = RD(p20 + 0xd3);
        if (d3 == 0) goto L2045;
        R = RD(d3 + 0x69);
        if (R == 0) goto L2045;
        {
            u8 b17 = RB(R + 0x17);
            s32 sv;
            edx = (edx & 0xffffff00u) | (u32)b17;
            ll = cm_fn_00544410(R, edx);
            sv = (s32)(s8)(u8)ll;
            u64v = cm_fn_0090cfc0((u32)sv,
                                  (u32)((unsigned long long)ll >> 32),
                                  (u32)sv);
            r1 = (u32)u64v;
            ehi = (u32)(u64v >> 32);
            u64v = cm_fn_0090cfc0(r1, ehi, 0x14);
            r2 = (u32)u64v;
            if ((s32)r1 > (s32)r2) {
                R = RD(d3 + 0x69);
                if ((s8)RB(R + 0x14) > 5) {
                    u32 dv = RD(d3);
                    RB(rec + 0x0e) = 1;
                    RD(rec + 0x0f) = dv;
                    edx = dv;
                    cm_fn_0068d0d0((volatile u8 *)G_7DC8, edx,
                                   (const u8 *)rec, 0x19);
                    goto L2045;
                }
            }
        }
    }
    /* ---- 0x006d1e05 numeric path ---- */
    {
        u32 id = RD(rec + 0);
        u32 t1 = id * 5u;
        u32 t2 = id + t1 * 2u;
        u32 pr;
        s16 dw;
        t2 = t2 + t2 * 4u;
        pr = RD(G_23BC + t2 * 2u + 0x61);
        dw = RW(pr + 0x0b);
        r3 = (u32)cm_fn_0090cfc0(G_23BC, t2, 0x3e8u);
        {
            u32 d4 = RD(rec + 4);
            u32 off = d4 + d4 * 8u;
            s32 w;
            off = off * 16u;
            off = off + d4;
            w = (s16)RW(G_23B4 + d4 + off * 4u + 0x80);
            r4 = (u32)cm_fn_0090cfc0(d4, r3, 0x3e8u);
            w = w - (s32)r3 + (s32)r4;
            edx = (u32)(s32)dw;
            if (!((s32)dw > w)) {
                if (dw <= 0x1b58) goto L2045;   /* 0x006d2042 */
            }
            RB(rec + 0x0e) = 0;
            ebx = 0xffffffffu;
            RD(rec + 0x0f) = ebx;
            cm_fn_0068d0d0((volatile u8 *)G_7DC8, edx, (const u8 *)rec, 0x19);
            goto L2045;
        }
    }

L1E73:
    /* ---- rec[0xd] == 0 : 0x006d1e73 ---- */
    {
        u8 b43 = RB(B + 0x43);
        u8 b44 = RB(B + 0x44);
        edx = (edx & 0xffffff00u) | (u32)b43;
        if ((s8)b44 > (s8)(u8)edx) goto L1EBA;
        if ((u8)edx != b44) goto L2045;
        {
            u32 p1c = RD(B + 0x1c);
            s32 v1c = (s16)RW(p1c + 0x80);
            u32 p20 = RD(B + 0x20);
            s32 v20 = (s16)RW(p20 + 0x80);
            if (!((long double)v20 <
                  (long double)v1c * (long double)K_968E48))
                goto L2045;
        }
    }
L1EBA:
    {
        u32 p1c = RD(B + 0x1c);
        d3 = RD(p1c + 0xd3);
        if (d3 == 0) goto L2045;
        R = RD(d3 + 0x69);
        if (R == 0) goto L2045;
        {
            u8 b17 = RB(R + 0x17);
            u32 adj = (B & 0xffffff00u) | (u32)b17;  /* mov cl,[eax+0x17] */
            ll = cm_fn_00544410(R, adj);
            edx = (u32)(s32)(s8)(u8)ll;              /* movsx edx,al */
            u64v = cm_fn_0090cfc0(edx, edx, edx);
            r1 = (u32)u64v;
            ehi = (u32)(u64v >> 32);
            if ((s32)r1 > 5) {
                R = RD(d3 + 0x69);
                if ((s8)RB(R + 0x14) > 5) {
                    u32 dv = RD(d3);
                    RB(rec + 0x0e) = 1;
                    RD(rec + 0x0f) = dv;
                    edx = ehi;
                    cm_fn_0068d0d0((volatile u8 *)G_7DC8, edx,
                                   (const u8 *)rec, 0x19);
                    goto L2045;
                }
            }
        }
    }
    /* ---- 0x006d1f16 numeric path ---- */
    {
        u32 id = RD(rec + 0);
        u32 t1 = id * 5u;
        u32 t2 = id + t1 * 2u;
        u32 pr;
        u32 d4;
        u32 off;
        s16 dxv, wv;
        t2 = t2 + t2 * 4u;
        pr = RD(G_23BC + t2 * 2u + 0x61);
        d4 = RD(rec + 4);
        off = d4 + d4 * 8u;
        off = off * 16u;
        off = off + d4;
        dxv = RW(pr + 0x0b);                     /* mov dx,[edx+0xb] */
        edx = (pr & 0xffff0000u) | (u32)(u16)dxv;
        wv = RW(G_23B4 + d4 + off * 4u + 0x80);
        if (dxv <= wv) goto L2032;
        u64v = cm_fn_0090cfc0(d4, edx, 4);
        ehi = (u32)(u64v >> 32);
        edx = ehi;
        if ((u32)u64v != 0) goto L2032;
    }
    /* 0x006d202b */
    RB(rec + 0x0e) = 0;
    RD(rec + 0x0f) = ebx;
    goto L2032;

L1F67:
    /* ---- other record type : 0x006d1f67 ---- */
    {
        u32 id = RD(rec + 0);
        u32 t1 = id * 5u;
        u32 t2 = id + t1 * 2u;
        t2 = t2 + t2 * 4u;
        target = G_23BC + t2 * 2u;
        if (target == 0) goto L2045;
        found = -1;
        grp = 0;
        for (k = 0; k < 0x14u; k++) {
            u32 p = obj + 0x47afu + k * 0x1beu;
            if (RD(p + 0x50) == target && (s8)RB(p) >= 0) {
                found = (s32)k;
                break;
            }
        }
        if (found < 0) {
            for (k = 0; k < 0x14u; k++) {
                u32 p = obj + 0x6a87u + k * 0x1beu;
                if (RD(p + 0x50) == target && (s8)RB(p) >= 0) {
                    found = (s32)k;
                    grp = 1;
                    break;
                }
            }
            if (found < 0) goto L2045;
        }
        edx = grp * 5u;                          /* 0x006d1fdd */
        entry = obj + 446u * ((u32)found + 20u * grp) + 0x4796u;
        if (entry == 0) goto L2045;
        if (di == 6) {
            if (RB(entry + 0x24) == 2) goto L202B;
            if (RB(entry + 0x23) == 0) goto L2045;
            goto L202B;
        }
        if ((s8)RB(entry + 0x1b) >= 7) goto L2045;
        {
            s32 sv = (s8)RB(entry + 0x17);
            u64v = cm_fn_0090cfc0(entry, (u32)sv, (u32)sv);
            edx = (u32)(u64v >> 32);
            if ((u32)u64v != 0) goto L2045;
        }
    }

L202B:
    RB(rec + 0x0e) = 0;
    RD(rec + 0x0f) = ebx;

L2032:
    cm_fn_0068d0d0((volatile u8 *)G_7DC8, edx, (const u8 *)rec, 0x19);

L2045:
    F.outer.b = (u8)(F.outer.b + 1);
    A = RD(obj + 0x478e);
    if ((s32)(s8)F.outer.b < (s32)RW(A + 0)) goto L1LOOP;

    /* ------------------------- phase 2 ------------------------- */
P2:
    ebx = obj;
    F.outer.b = 0;

L206B:
    F.sum = 0;
    F.slot1c = 0;
    edx = edx & 0xffffff00u;
    F.inner = 0;
    {
        u32 os = (u32)(s32)(s8)F.outer.b;
        F.slot2c = os;
        F.slot20 = (os * 5u) * 4u;
    }

L2094:
    {
        u32 idx = (u32)(s32)(s8)F.inner + F.slot20;
        ep = obj + idx * 446u + 0x4796u;
    }
    if ((s8)F.inner > 10) {
        if (RB(ep + 0x1f) == 0xff) goto L25B0;
    }
    rp = RD(ep + 0x69);
    if (rp == 0) goto L251B;
    if (RD(ep + 0x6d) == 0) goto L251B;
    if ((s32)RD(rp) >= (s32)G_2364) goto L251B;

    /* 0x006d20e4 */
    {
        u32 ecx_v, edi_v;
        B = RD(obj + 0x4792);
        if (F.outer.b == 0) {
            edi_v = RD(B + 0x20);
            ecx_v = RD(B + 0x1c);
        } else {
            edi_v = RD(B + 0x1c);
            ecx_v = RD(B + 0x20);
        }
        edx = (edx & 0xffffff00u) | (u32)RB(obj);
        {
            s32 sb = (s32)(s8)RB(ep + 0x1b);
            float fv = (float)sb;
            cm_fn_006d26b0((int)ep, (int)rp, (int)ecx_v, (int)edi_v, (int *)B,
                           fv, edx, 1);
        }
    }
    if (G_7DC8 == 0) goto L2517;
    B = RD(obj + 0x4792);
    if (F.outer.b == 0) px = RD(B + 0x1c); else px = RD(B + 0x20);
    if (cm_fn_0053b900((u32 *)px) != 0) goto L2517;
    B = RD(obj + 0x4792);
    if (RB(B + 0x40) == 0) goto L2517;
    if (F.outer.b == 0) px = RD(B + 0x1c); else px = RD(B + 0x20);
    res = cm_fn_004cf5f0(G_1050, rp, px);
    if (res != 0) {
        B = RD(obj + 0x4792);
        if (F.outer.b == 0) px = RD(B + 0x1c); else px = RD(B + 0x20);
        if (px == 0) goto L233F;
        if (rp == RD(px + 0xcf)) goto L233F;
        if (RD(B + 0x14) == 0) goto L233F;
        if (cm_fn_005c6710((void *)G_2C74, RD(B + 0x14)) != 0) goto L233F;
        if (RB(ep + 0x20) == 0xff) goto L233F;
        if (RB(ep + 0x22) != 0xff) goto L233F;
        if ((s8)RB(res + 0x3d) <= 0) goto L233F;
        if ((s16)RW(RD(ep + 0x6d) + 0x0b) <= 0x109a) goto L233F;
        B = RD(obj + 0x4792);
        if (B == 0) goto L233F;
        if (RB(B + 0x40) == 0) goto L233F;
        /* 0x006d2238 */
        ll = (long long)(long double)RF(ep + 0x1a2);   /* 0x00945640 __ftol */
        u64v = cm_fn_0090cfc0((u32)ll, (u32)((unsigned long long)ll >> 32),
                              (u32)ll);
        r1 = (u32)u64v;
        ehi = (u32)(u64v >> 32);
        u64v = cm_fn_0090cfc0(r1, ehi, 0x64);
        r2 = (u32)u64v;
        ehi = (u32)(u64v >> 32);
        if ((s32)r1 <= (s32)r2) goto L233F;
        {
            s32 sv = (s8)RB(rp + 0x5d);
            u64v = cm_fn_0090cfc0(r2, (u32)sv, (u32)sv);
            r3 = (u32)u64v;
            ehi = (u32)(u64v >> 32);
            u64v = cm_fn_0090cfc0(r3, ehi, 0xa);
            r4 = (u32)u64v;
            ehi = (u32)(u64v >> 32);
            if ((s32)r3 >= (s32)r4) goto L233F;
        }
        /* 0x006d227e */
        F.f28 = RF(ep + 0x4d);
        u64v = cm_fn_0090cfc0(r4, ehi, 0x5dc);
        r5 = (u32)u64v;
        ehi = (u32)(u64v >> 32);
        r5 = r5 + 0x1d4cu;
        F.slot24 = r5;
        if (!((long double)(s32)F.slot24 < (long double)F.f28)) goto L233F;
        /* 0x006d22ae */
        {
            u8 b5b = RB(rp + 0x5b);
            s32 sb;
            if ((s8)b5b >= 6) goto L233F;
            if ((s8)RB(rp + 0x57) <= 10) goto L233F;
            sb = (s32)(s8)b5b;
            u64v = cm_fn_0090cfc0(rp, ehi,
                                  (u32)(s32)(s8)RB(rp + 0x57));
            r6 = (u32)u64v;
            ehi = (u32)(u64v >> 32);
            u64v = cm_fn_0090cfc0(r6, (u32)sb, (u32)sb);
            r7 = (u32)u64v;
            if ((s32)r7 >= (s32)r6) goto L233F;
        }
        /* 0x006d22df: first message */
        {
            u32 v = RD(obj + 0x4792);
            u32 px2;
            edx = v;
            F.msg.b0e = 0;
            F.msg.f0f = 0xffffffffu;
            if (F.outer.b == 0) px2 = RD(v + 0x1c); else px2 = RD(v + 0x20);
            F.msg.f04 = RD(px2);
            F.msg.b0c = 0;
            F.msg.b0d = F.outer.b;
            F.msg.f00 = RD(rp);
            F.msg.f13 = 0x000a;
            F.msg.f08 = 0x4000000u;
            edx = RD(ep + 0x69);
            cm_fn_0068d0d0((volatile u8 *)G_7DC8, edx, (const u8 *)&F.msg,
                           0x19);
        }
    }

L233F:
    {
        s32 sc = -(s32)(s16)RW(ep + 0x31);
        u32 other, first;
        B = RD(obj + 0x4792);
        if (F.outer.b != 0) {
            first = RD(B + 0x20);
            other = RD(B + 0x1c);
        } else {
            first = RD(B + 0x1c);
            other = RD(B + 0x20);
        }
        if (RD(first + 0xb3) == other) sc += 10;
        else if (RD(first + 0xb7) == other) sc += 10;
        else if (RD(first + 0xbb) == other) sc += 10;
        if (cm_fn_005477d0(rp, other) != 0) {
            sc += 10;
        } else if (cm_fn_00547790(rp, other) != 0) {
            sc -= 10;
        }
        if (cm_fn_00547d30(rp, other) != 0) {
            sc += 10;
        } else if (cm_fn_00547d60(rp, other) != 0) {
            sc -= 20;
        }
        {
            u8 b5b = RB(rp + 0x5b);
            u8 b5d = RB(rp + 0x5d);
            if ((s8)b5b >= 6) goto L2517;
            if ((s8)b5d >= 10) goto L2517;
            {
                s32 a = (s32)(s8)RB(rp + 0x5a) * (s32)(s8)b5b *
                        (s32)(s8)b5d + 0x14;
                u32 bound = (u32)(a * 100);
                u64v = cm_fn_0090cfc0((u32)a, bound, bound);
                r5 = (u32)u64v;
                ehi = (u32)(u64v >> 32);
                sc += 50;
                u64v = cm_fn_0090cfc0(r5, ehi, (u32)sc);
                r6 = (u32)u64v;
                if ((s32)r5 >= (s32)r6) goto L2517;
            }
            /* 0x006d24b5: second message */
            {
                u32 v = RD(obj + 0x4792);
                u32 px2;
                edx = v;
                F.msg.b0e = 0;
                F.msg.f0f = 0xffffffffu;
                if (F.outer.b == 0) px2 = RD(v + 0x1c); else px2 = RD(v + 0x20);
                F.msg.f04 = RD(px2);
                F.msg.b0c = 0;
                F.msg.b0d = F.outer.b;
                F.msg.f00 = RD(rp);
                F.msg.f13 = 0x000b;
                F.msg.f08 = 0;
                edx = RD(ep + 0x69);
                cm_fn_0068d0d0((volatile u8 *)G_7DC8, edx,
                               (const u8 *)&F.msg, 0x19);
            }
        }
    }

L2517:
L251B:
    if (RB(obj) != 0) {
        if ((s8)F.inner >= 0xb) goto L2557;
        al_v = 0x5a;
        goto L2598;
    }
    if ((s8)F.inner >= 0xb) goto L2557;
    al_v = RB(ep + 0x21);
    if ((s8)al_v > -1) goto L2594;
    al_v = RB(ep + 0x20);
    if ((s8)al_v > -1) goto L2594;
    al_v = (u8)cm_div11((s32)RW(obj + 0x8ed0));
    goto L2590;

L2557:
    cl_v = RB(ep + 0x1f);
    if (cl_v == 0xff) goto L25B0;
    al_v = RB(ep + 0x21);
    if ((s8)al_v > -1) { al_v = (u8)(al_v - cl_v); goto L2594; }
    al_v = RB(ep + 0x20);
    if ((s8)al_v > -1) { al_v = (u8)(al_v - cl_v); goto L2594; }
    al_v = (u8)((u8)cm_div11((s32)RW(obj + 0x8ed0)) - cl_v);

L2590:
    edx = (edx & 0xffffff00u) | (u32)F.inner;

L2594:
    if (al_v == 0) goto L25B0;

L2598:
    F.sum = F.sum + (u32)(s32)(s8)al_v;
    F.slot1c = F.slot1c +
               (u32)((s32)(s8)RB(ep + 0x1b) * (s32)(s8)al_v);

L25B0:
    F.inner = (u8)(F.inner + 1);
    if ((s8)F.inner < 0x14) goto L2094;

    /* 0x006d25bf */
    if (F.sum != 0) {
        u32 t0, t1;
        long double v;
        B = RD(obj + 0x4792);
        if (F.outer.b == 0) px = RD(B + 0x1c); else px = RD(B + 0x20);
        t0 = RD(B + 0x18);
        t1 = RD(px + 0x69);
        v = (long double)(s32)F.slot1c / (long double)(s32)F.sum;
        if (t0 == t1) v = v - (long double)K_967880;
        else v = v + (long double)K_968918;
        F.d30 = (double)v;
    }
    /* 0x006d2601 */
    {
        u32 other, ec;
        float amt;
        B = RD(obj + 0x4792);
        if (F.outer.b == 0) {
            other = RD(B + 0x20);
            ec = RD(B + 0x1c);
        } else {
            other = RD(B + 0x1c);
            ec = RD(B + 0x20);
        }
        amt = (float)F.d30;
        cm_fn_006d3610(obj + F.slot2c * 0xd0u + 0x9032u, ec, other, B, amt,
                       (u8)RB(obj), 1);
    }
    /* 0x006d266b */
    B = RD(obj + 0x4792);
    if (F.outer.b == 0) px = RD(B + 0x1c); else px = RD(B + 0x20);
    if (cm_fn_0053b900((u32 *)px) == 0) {
        cm_fn_006d3de0(obj, F.outer.w, (float)F.d30, (u32)RB(obj), 1);
    }
    F.outer.b = (u8)(F.outer.b + 1);
    if ((s8)F.outer.b < 2) goto L206B;
}
