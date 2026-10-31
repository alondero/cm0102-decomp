#include "cmre.h"

/* Object fields by byte offset from `self` (the ECX object):
   0x18 + 4*idx: pointer to a 12-byte record array (record offset `off`).
   0x54: pointer used for the idx==0x0f store.
   0x58 + 4*idx: pointer to a second record array (record offset `off`).
   0x98 + 4*idx: element count for index `idx` (signed).
   `off` is the byte offset of the current record (12 bytes per step).
   Record fields used (by offset from the record pointer): +0x00 value,
   +0x08 store target. Result object fields: +0x04, +0x3d, +0x4e.
   Pointer-target fields: +0x39, +0x52, +0x57, +0x61, +0x80. */

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 p1);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 p1, u32 p2);
extern u8 CM_FASTCALL cm_fn_00867d30(u32 p1);
extern u32 CM_CDECL cm_fn_00600a50(u32 p1, u32 p2, u8 p3, u32 p4, u32 p5);
extern u32 CM_CDECL cm_fn_005444a0(u32 p1, u32 p2);
extern void CM_CDECL cm_fn_00945333(u32 p1, u32 p2, u32 p3, u32 p4);

#define RD32(a) (*(volatile u32 *)(a))
#define RD16(a) (*(volatile u16 *)(a))
#define RD8(a) (*(volatile u8 *)(a))
#define RDS8(a) (*(volatile s8 *)(a))

void CM_THISCALL cm_fn_0083a780(u32 self)
{
    u32 idx;
    s32 count;
    s32 lim;
    s32 counter;
    u32 off;
    u32 p18;
    u32 v;
    u32 g1;
    u32 g2;
    u32 arg;
    u32 r;
    u32 x;
    u32 f4;
    u32 a;
    u32 r2;
    u32 t;
    u32 c;
    u32 w;
    u32 p18c;
    u32 addr;
    u32 cur;
    u32 p18d;
    u32 objv;
    u32 addr2;
    u32 hi;
    u32 q;
    u32 cur2;
    u32 q2;
    u32 g1b;
    u32 r3;
    u32 x2;
    u32 r4;
    u32 k;
    u32 obj2;
    u32 m;
    u32 key;
    u32 g4;
    u32 mem;
    u16 w80;
    u8 cl;
    s32 ecx_s;
    u32 cnt2;
    u32 cnt3;
    u32 p58;
    u32 p18e;
    long long prod;

    idx = 0u;
    do {
        count = (s32)RD32(self + idx * 4u + 0x98u);
        off = 0u;
        counter = 0;
        if (count > 0) {
            do {
                p18 = RD32(self + idx * 4u + 0x18u);
                v = RD32(p18 + off);
                g1 = CM_GLOBAL(volatile u32, 0x00ae1050);
                g2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
                arg = g2 + 110u * v;
                r = cm_fn_004cf5a0(g1, arg);
                CM_GLOBAL(volatile u32, 0x00ae1080) = r;
                if (r != 0u) {
                    f4 = RD32(r + 4u);
                    a = CM_GLOBAL(volatile u32, 0x00ae23b4);
                    x = a + 581u * f4;
                } else {
                    x = 0u;
                }
                g1b = CM_GLOBAL(volatile u32, 0x00ae1050);
                r2 = cm_fn_004cf5f0(g1b, arg, x);

                if (idx == 0x0fu) {
                    t = RD32(self + 0x54u);
                    c = RD32(arg + 0x52u);
                    RD32(t + off + 8u) = c;
                    goto loop_tail;   /* listing: jmp 0x83a965 */
                } else if (r2 == 0u || cm_fn_00867d30(r2) != 0u) {
                    /* 83a882: zero the +8 field of the record */
                    ecx_s = (s32)RD32(self + idx * 4u + 0x18u);
                    RD32((u32)ecx_s + off + 8u) = 0u;
                } else {
                    p18 = RD32(self + idx * 4u + 0x18u);
                    c = RD32(arg + 0x52u);
                    RD32(p18 + off + 8u) = c;
                    w = RD32(arg + 0x39u);
                    if (w != 0u &&
                        cm_fn_00600a50(w, 0u, 1u, 0u, 0u) != 0u &&
                        cm_fn_005444a0(arg, 0u) != 0u) {
                        p18c = RD32(self + idx * 4u + 0x18u);
                        addr = p18c + off + 8u;
                        cur = RD32(addr);
                        RD32(addr) = cur + 0xffe91ca0u;
                        p18d = RD32(self + idx * 4u + 0x18u);
                        objv = RD32(arg + 0x52u);
                        addr2 = p18d + off + 8u;
                        prod = (long long)(s32)0x55555556u * (long long)(s32)objv;
                        hi = (u32)((unsigned long long)prod >> 32);
                        q = hi + (hi >> 31);
                        cur2 = RD32(addr2);
                        if ((s32)cur2 < (s32)q) {
                            RD32(addr2) = q;
                        }
                    }
                }

                /* 83a88e: second record update for this index and step */
                q2 = RD32(self + idx * 4u + 0x58u);
                v = RD32(off + q2);
                g1 = CM_GLOBAL(volatile u32, 0x00ae1050);
                g2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
                arg = g2 + 110u * v;
                r3 = cm_fn_004cf5a0(g1, arg);
                CM_GLOBAL(volatile u32, 0x00ae1080) = r3;
                if (r3 != 0u) {
                    f4 = RD32(r3 + 4u);
                    a = CM_GLOBAL(volatile u32, 0x00ae23b4);
                    x2 = a + 581u * f4;
                } else {
                    x2 = 0u;
                }
                g1b = CM_GLOBAL(volatile u32, 0x00ae1050);
                r4 = cm_fn_004cf5f0(g1b, arg, x2);
                k = RD32(arg + 0x61u);
                if (k != 0u) {
                    obj2 = RD32(arg + 0x39u);
                    if (obj2 != 0u) {
                        m = RD32(obj2 + 0x57u);
                        if (m != 0u) {
                            key = RD32(m);
                            g4 = CM_GLOBAL(volatile u32, 0x00adadfc);
                            mem = RD32(g4 + key * 4u);
                            if (mem == 0u) {
                                w80 = RD16(obj2 + 0x80u);
                            } else {
                                w80 = 0x1770u;
                            }
                            if (mem == 0u && (s16)w80 < 0x1770) {
                                /* 83a959 */
                                q2 = RD32(self + idx * 4u + 0x58u);
                                RD32(off + q2 + 8u) = 0x7d0u;
                            } else {
                                /* 83a910 */
                                if (r4 == 0u) {
                                    /* 83a94b */
                                    t = RD32(self + idx * 4u + 0x58u);
                                    RD32(off + t + 8u) = 0x7d0u;
                                } else {
                                    cl = RD8(r4 + 0x4eu);
                                    if ((cl & 2u) != 0u) {
                                        t = RD32(self + idx * 4u + 0x58u);
                                        RD32(off + t + 8u) = 0xfffffc18u;
                                    } else if ((cl & 9u) == 0u) {
                                        ecx_s = (s32)RDS8(r4 + 0x3du);
                                        q2 = RD32(self + idx * 4u + 0x58u);
                                        RD32(off + q2 + 8u) = (u32)ecx_s;
                                    } else {
                                        t = RD32(self + idx * 4u + 0x58u);
                                        RD32(off + t + 8u) = 0x7d0u;
                                    }
                                }
                            }
                        } else {
                            /* 83a959 via je at 83a8f5 */
                            q2 = RD32(self + idx * 4u + 0x58u);
                            RD32(off + q2 + 8u) = 0x7d0u;
                        }
                    } else {
                        /* 83a959 via je at 83a8ee */
                        q2 = RD32(self + idx * 4u + 0x58u);
                        RD32(off + q2 + 8u) = 0x7d0u;
                    }
                } else {
                    /* 83a959 via je at 83a8e7 */
                    q2 = RD32(self + idx * 4u + 0x58u);
                    RD32(off + q2 + 8u) = 0x7d0u;
                }

loop_tail:
                /* 83a965: loop tail */
                lim = (s32)RD32(self + idx * 4u + 0x98u);
                counter = counter + 1;
                off = off + 0xcu;
            } while (counter < lim);
        }

        /* 83a980: two calls to 0x00945333 for this index */
        cnt2 = RD32(self + idx * 4u + 0x98u);
        p18e = RD32(self + idx * 4u + 0x18u);
        cm_fn_00945333(p18e, cnt2, 0x0cu, 0x0083b670u);
        cnt3 = RD32(self + idx * 4u + 0x98u);
        p58 = RD32(self + idx * 4u + 0x58u);
        cm_fn_00945333(p58, cnt3, 0x0cu, 0x0083b670u);

        idx = idx + 1u;
    } while (idx < 0x10u);
}
