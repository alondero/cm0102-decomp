#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (self):
   0x00 and 0x04 are passed to 0x005e3fd0 and 0x005e2eb0; 0x08, 0x0c and 0x10
   are read as listed; 0x200 is a pointer passed to 0x005e3fd0; 0x208 is the
   base of 396-byte records (stride 0x18c); a signed 16-bit table at 0x212 + 2*i
   holds record indexes (entry -1 is at 0x210); 0xb72 is a 16-bit word; 0xb74 is
   a signed 16-bit count; 0xb76 is a signed 16-bit cursor (DI is its low word
   during the search); 0xbac is a byte limit.
   Record fields by byte offset from the record base: 0x10, 0x14, 0x18 and 0x1c
   are written; 0x20, 0x24, 0x28, 0x2c, 0x30 and 0x34 are read. */
#define SELF_U8(off) (*(volatile u8 *)(self + (u32)(off)))
#define SELF_U16(off) (*(volatile u16 *)(self + (u32)(off)))
#define SELF_S16(off) (*(volatile s16 *)(self + (u32)(off)))
#define SELF_U32(off) (*(volatile u32 *)(self + (u32)(off)))
#define ORD(i) (*(volatile s16 *)(self + 0x212u + (u32)(i) * 2u))
#define REC_ADDR(r) (recs + (u32)(r) * 396u)
#define REC_U32(r, off) (*(volatile u32 *)(REC_ADDR(r) + (u32)(off)))

extern void cm_fn_005e2eb0(u32 a0, u32 a1, u32 a2, u32 a3);
extern void cm_fn_005e3fd0(u32 a0, u32 a1, u32 a2);
extern void CM_THISCALL cm_fn_005ee590(u32 rec, u32 arg);
extern void CM_THISCALL cm_fn_00800330(u32 obj, u32 p2, u32 p3);

void CM_THISCALL cm_fn_00403ee0(u32 self, s32 arg)
{
    u32 recs;
    u16 di;
    s32 ai;
    s32 diff;
    u32 slot;

    if (SELF_U8(0xbac) == 0) {
        return;
    }
    if (SELF_S16(0xb74) <= 0) {
        return;
    }
    {
        u32 p = SELF_U32(0x200);
        if (p != 0) {
            u32 c0 = SELF_U32(0x00);
            u32 c4 = SELF_U32(0x04);
            cm_fn_005e3fd0(c0, c4, p);
        }
    }

    di = SELF_U16(0xb76);
    recs = SELF_U32(0x208);
    ai = ORD((s16)di);

    if (arg > 0) {
        s32 a2;
        u32 ka;
        u32 kb;

        a2 = ORD((s16)di);
        ka = REC_U32(a2, 0x24);
        kb = REC_U32(ai, 0x24);
        diff = (s32)(ka - kb);
        if (diff >= arg) {
            goto done;
        }
        for (;;) {
            s32 cidx;
            s32 bidx;
            u32 kbb;
            u32 kcc;

            di = (u16)(di + 1);
            if ((s16)di >= SELF_S16(0xb74)) {
                di = (u16)(di - 1);
                goto done;
            }
            cidx = ORD(SELF_S16(0xb76));
            bidx = ORD((s16)di);
            kbb = REC_U32(bidx, 0x24);
            kcc = REC_U32(cidx, 0x24);
            diff = (s32)(kbb - kcc);
            if (diff < arg) {
                continue;
            }
            goto done;
        }
    } else {
        s32 a2;
        u32 ka;
        u32 kb;

        a2 = ORD((s16)di);
        ka = REC_U32(a2, 0x24);
        kb = REC_U32(ai, 0x24);
        diff = (s32)(ka - kb);
        if (diff > arg) {
            for (;;) {
                s32 cidx;
                s32 bidx;
                u32 kbb;
                u32 kcc;

                di = (u16)(di - 1);
                if ((s16)di < 0) {
                    di = (u16)(di + 1);
                    break;
                }
                cidx = ORD(SELF_S16(0xb76));
                bidx = ORD((s16)di);
                kbb = REC_U32(bidx, 0x24);
                kcc = REC_U32(cidx, 0x24);
                diff = (s32)(kbb - kcc);
                if (diff > arg) {
                    continue;
                }
                break;
            }
        }
        /* 0x404033: stop unless the position is 0 */
        if ((s32)(s16)di - 1 != -1) {
            for (;;) {
                s32 bidx2;
                s32 yidx;
                u32 ky;
                u32 kb2;

                bidx2 = ORD((s16)di);
                yidx = ORD((s32)(s16)di - 1);
                ky = REC_U32(yidx, 0x24);
                kb2 = REC_U32(bidx2, 0x24);
                if (ky != kb2) {
                    break;
                }
                di = (u16)(di - 1);
                if ((s32)(s16)di - 1 == -1) {
                    break;
                }
            }
        }
    }

done:
    SELF_U16(0xb76) = di;
    if (di != 0xffffu) {
        for (;;) {
            s32 R;
            s32 cidx;
            u32 kR;
            u32 kC;
            s32 Q;

            if ((s16)di >= SELF_S16(0xb74)) {
                break;
            }
            recs = SELF_U32(0x208);
            slot = self + 0x212u + (u32)(s16)di * 2u;
            R = *(volatile s16 *)slot;
            cidx = ORD(SELF_S16(0xb76));
            kR = REC_U32(R, 0x24);
            kC = REC_U32(cidx, 0x24);
            Q = SELF_U8(0xbac);
            diff = (s32)(kR - kC);
            if (diff > (s32)Q - 1) {
                goto final;
            }
            {
                u32 f28 = REC_U32(R, 0x28);
                u32 b10;
                u32 ebp_v;
                u32 f30;
                u32 c;
                u32 d;

                if (f28 != 0) {
                    b10 = f28;
                    ebp_v = REC_U32(R, 0x2c);
                } else {
                    u32 f20 = REC_U32(R, 0x20);
                    u32 s0 = SELF_U32(0x00);

                    b10 = SELF_U32(0x1cu + f20 * 4u) + s0;
                    ebp_v = SELF_U32(0x10cu + f20 * 4u) + s0;
                }
                f30 = REC_U32(R, 0x30);
                if (f30 != 0) {
                    c = f30;
                    d = REC_U32(R, 0x34);
                } else {
                    u32 t = SELF_U32(0x10);
                    u32 ebp_s = SELF_U32(0x04);
                    u32 idx = t + (u32)diff;
                    u32 c1 = SELF_U32(0x94u + idx * 4u);
                    u32 d1 = SELF_U32(0x184u + idx * 4u);

                    c = c1 + ebp_s;
                    d = d1 + ebp_s;
                }
                REC_U32(R, 0x10) = b10;
                REC_U32(R, 0x18) = ebp_v;
                REC_U32(R, 0x14) = c;
                REC_U32(R, 0x1c) = d;
            }
            recs = SELF_U32(0x208);
            cm_fn_005ee590(REC_ADDR(*(volatile s16 *)slot), 0u);
            di = (u16)(di + 1);
            if (di == 0xffffu) {
                break;
            }
        }
    }

final:
    {
        u32 f8 = SELF_U32(0x08);
        u32 fc = SELF_U32(0x0c);
        u32 f4 = SELF_U32(0x04);
        u32 f0 = SELF_U32(0x00);

        cm_fn_005e2eb0(f0, f4, f8 - 0x16u, fc);
    }
    {
        s32 idx = ORD(SELF_S16(0xb76));
        u32 t = (u32)idx * 99u;
        u32 key;
        u32 comp;
        u16 w;

        recs = SELF_U32(0x208);
        key = REC_U32(idx, 0x24);
        w = SELF_U16(0xb72);
        comp = (t & 0xffff0000u) | (u32)w;
        cm_fn_00800330(0xb74450u, comp, key);
    }
}
