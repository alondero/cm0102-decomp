/* Tier-2 C recovery for the 3.9.68 primary executable, VA 0x005c4cb0
   (901 bytes, image base 0x00400000).

   Convention: ECX is the receiver (thiscall). Five stack arguments follow the
   return address and the callee pops 0x14. Entry slots: p2 = [esp+4],
   p3 = [esp+8], p4 = [esp+0xc], p5 = [esp+0x10] (one byte read),
   p6 = [esp+0x14].

   The 22-byte key record (0x16 bytes, same layout as the elements scanned by
   0x00946464 with stride 0x16) is one scratch allocation with explicit offsets.
   Table entries of 0x245 bytes are indexed by the dword at the element start.
   Fields are named by offset only. */
#include "cmre.h"

#define MEM_U8(a, off)  (*(volatile u8 *)((a) + (off)))
#define MEM_S8(a, off)  (*(volatile s8 *)((a) + (off)))
#define MEM_U16(a, off) (*(volatile u16 *)((a) + (off)))
#define MEM_S16(a, off) (*(volatile s16 *)((a) + (off)))
#define MEM_U32(a, off) (*(volatile u32 *)((a) + (off)))

extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern int cm_fn_0090cfc0(int param_1);
extern u32 cm_fn_00946464(u32 param_1, u32 param_2, u32 param_3, int param_4, void *param_5);

u32 CM_THISCALL cm_fn_005c4cb0(u32 self, u32 p2, u32 p3, u32 p4, s8 p5, u32 p6)
{
    u8 key[0x16];
    u32 keyAddr;
    s8 level;
    u32 base;
    u32 idx;
    u32 obj;
    u32 cand;
    u32 end;
    u32 candIdx;
    u32 iVar7;
    u32 obj2;
    u32 ptrC;
    u32 ptr9;
    u32 t;
    u32 r;
    u32 k;
    u32 w1;
    u32 w2;
    u8 b1;
    u8 b2;
    s16 sv;
    s16 g;
    u16 f;
    u16 cx;
    u16 ax;
    s8 cl;
    s8 al;
    s32 a;
    s32 cr;

    /* 005c4cb7..005c4cc9: clamp the byte argument (signed) to 1..20. */
    level = p5;
    if (level < 1) {
        level = 1;
    } else if (level > 0x14) {
        level = 0x14;
    }

    /* 005c4ccb..005c4d0e: read the head index and table base, then fill the
       key record. The original writes the record bytes in this order. */
    keyAddr = (u32)key;
    idx = MEM_U32(p2, 0);
    base = CM_GLOBAL(volatile u32, 0x00ae23b4);
    obj = base + idx * 0x245u;
    MEM_U32(keyAddr, 0x0) = 0;
    MEM_U8(keyAddr, 0x4) = 0;
    MEM_U32(keyAddr, 0x6) = 0;
    MEM_U32(keyAddr, 0xa) = 0;
    MEM_U32(keyAddr, 0xe) = 0;
    MEM_S8(keyAddr, 0x5) = level;
    MEM_U32(keyAddr, 0x12) = 0;

    /* 005c4d12..005c4d40: call 0x00946464 until it returns a non-null element.
       When the byte at +5 is 20 or more, the element is the end-of-range
       argument p3 instead. The byte at +5 then advances (signed 8-bit wrap). */
    do {
        cand = cm_fn_00946464(keyAddr, p3, p4, 0x16u, (void *)0x005c5270u);
        if (MEM_S8(keyAddr, 0x5) >= 0x14) {
            cand = p3;
        }
        MEM_S8(keyAddr, 0x5) = (s8)(MEM_S8(keyAddr, 0x5) + 1);
    } while (cand == 0);

    /* 005c4d42..005c4d52: end = p3 + 0x16 * p4 - 0x16. The original parks this
       value in the p4 slot, which is not observable; an empty range returns 0. */
    end = p3 + p4 * 0x16u - 0x16u;
    if (cand >= end) {
        return 0;
    }

    for (;;) {
        /* 005c4d58..005c4d6f: skip a candidate that is the same table entry. */
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        candIdx = MEM_U32(cand, 0);
        iVar7 = base + candIdx * 0x245u;
        if (obj == iVar7) {
            goto next;
        }

        /* 005c4d77..005c4d9c: p6 holds four element pointers; an equal index
       in any non-null slot skips this candidate. */
        for (k = 0; k < 4; k++) {
            if (p6 != 0) {
                t = MEM_U32(p6, k * 4);
                if (t != 0 && candIdx == MEM_U32(t, 0)) {
                    goto next;
                }
            }
        }

        /* 005c4d9e..005c4dd1: move the link at +0xa through the helper. */
        t = MEM_U32(cand, 0xa);
        if (t != 0) {
            MEM_U32(t, 0xa) = MEM_U32(t, 0x2);
            r = cm_fn_0068d1b0(MEM_U32(cand, 0xa));
            if (r != 0) {
                do {
                    w1 = MEM_U32(r, 0);
                    w2 = MEM_U32(p2, 0);
                    if (w1 == w2) {
                        goto next;
                    }
                    r = cm_fn_0068d1b0(MEM_U32(cand, 0xa));
                } while (r != 0);
            }
        }

        /* 005c4dd7..005c4df7: a receiver word at 0x34e8 that matches the global
       word, together with the entry word at 0x1db matching its global, skips. */
        g = CM_GLOBAL(volatile s16, 0x00ae2c90);
        f = MEM_U16(self, 0x34e8);
        if ((s32)g == (s32)f) {
            w1 = MEM_U32(iVar7, 0x1db);
            if (w1 == CM_GLOBAL(volatile u32, 0x009cf6f8)) {
                goto next;
            }
        }

        /* 005c4e02..005c4e28: words at 0x0e (cand and p2) and 0x10. 0xffff
       in either word bypasses the window test. */
        cx = MEM_U16(cand, 0xe);
        if (cx != 0xffffu) {
            ax = MEM_U16(p2, 0xe);
            if (ax != 0xffffu) {
                if ((s16)cx > MEM_S16(p2, 0x10)) {
                    goto next;
                }
                if ((s16)ax > MEM_S16(cand, 0x10)) {
                    goto next;
                }
            }
        }

        /* 005c4e2e..005c4e70: six back-link comparisons between the two entries. */
        if (obj == MEM_U32(iVar7, 0xb3)) goto next;
        if (iVar7 == MEM_U32(obj, 0xb3)) goto next;
        if (obj == MEM_U32(iVar7, 0xb7)) goto next;
        if (iVar7 == MEM_U32(obj, 0xb7)) goto next;
        if (obj == MEM_U32(iVar7, 0xbb)) goto next;
        if (iVar7 == MEM_U32(obj, 0xbb)) goto next;

        /* 005c4e76..005c4eaa: compare the 0x53 pointers through their 0x110,
       0x114 and 0x118 fields. A match requires one extra helper check. */
        ptrC = MEM_U32(iVar7, 0x53);
        ptr9 = MEM_U32(obj, 0x53);
        if (ptr9 == MEM_U32(ptrC, 0x110)) goto L4eac;
        if (ptrC == MEM_U32(ptr9, 0x110)) goto L4eac;
        if (ptr9 == MEM_U32(ptrC, 0x114)) goto L4eac;
        if (ptrC == MEM_U32(ptr9, 0x114)) goto L4eac;
        if (ptr9 == MEM_U32(ptrC, 0x118)) goto L4eac;
        if (ptrC != MEM_U32(ptr9, 0x118)) goto L4ec4;
L4eac:
        cr = cm_fn_0090cfc0(2);
        if (cr == 0) {
            goto next;
        }
L4ec4:
        /* 005c4ec4..005c4ee1: field 0x57 must differ; helper value 3 bounds byte +4. */
        w1 = MEM_U32(obj, 0x57);
        w2 = MEM_U32(iVar7, 0x57);
        if (w1 == w2) {
            goto next;
        }
        cr = cm_fn_0090cfc0(3);
        cl = MEM_S8(cand, 0x4);
        al = (s8)(u8)(cr + 3);
        if (cl > al) {
            goto next;
        }

        /* 005c4ee9..005c4f20: re-read the table base and the head index; word at
       0x80 of the head entry selects the rule. */
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        idx = MEM_U32(p2, 0);
        obj2 = base + idx * 0x245u;
        sv = MEM_S16(obj2, 0x80);
        if (sv < 8000 && sv < 6000 && sv < 4000 && sv < 2000) {
            candIdx = MEM_U32(cand, 0);
            iVar7 = base + candIdx * 0x245u;
            ptrC = MEM_U32(iVar7, 0x53);
            ptr9 = MEM_U32(obj2, 0x53);
            if (ptrC != ptr9) {
                goto next;
            }
            goto L4fb5;
        }

        /* 005c4f45..005c4f66: word in [2000, 4000) and below 6000 and 8000. */
        sv = MEM_S16(obj2, 0x80);
        if (sv < 8000 && sv < 6000 && sv < 4000 && sv >= 2000) {
            candIdx = MEM_U32(cand, 0);
            ptr9 = MEM_U32(obj2, 0x53);
            iVar7 = base + candIdx * 0x245u;
            ptrC = MEM_U32(iVar7, 0x53);
            b1 = MEM_U8(ptrC, 0x75);
            b2 = MEM_U8(ptr9, 0x75);
            if (b1 != b2) {
                goto next;
            }
            goto L4fb5;
        }

        /* 005c4f8c..005c4fb3: accept at 8000 or more; otherwise compare dword 0x71. */
        if (MEM_S16(obj2, 0x80) >= 8000) {
            goto L4fb5;
        }
        candIdx = MEM_U32(cand, 0);
        ptr9 = MEM_U32(obj2, 0x53);
        iVar7 = base + candIdx * 0x245u;
        ptrC = MEM_U32(iVar7, 0x53);
        w1 = MEM_U32(ptrC, 0x71);
        w2 = MEM_U32(ptr9, 0x71);
        if (w1 != w2) {
            goto next;
        }

L4fb5:
        /* 005c4fb5..005c4fc5: helper value 2 against the signed byte at +4 of p2. */
        cr = cm_fn_0090cfc0(2);
        a = MEM_S8(p2, 0x4);
        if (a <= (s32)cr) {
            goto L4ff3;
        }
        /* 005c4fc7..005c4fef: the 0x53 pointers of the head and candidate must match. */
        idx = MEM_U32(p2, 0);
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        candIdx = MEM_U32(cand, 0);
        iVar7 = base + candIdx * 0x245u;
        ptrC = MEM_U32(iVar7, 0x53);
        obj2 = base + idx * 0x245u;
        ptr9 = MEM_U32(obj2, 0x53);
        if (ptrC != ptr9) {
            goto next;
        }

L4ff3:
        /* 005c4ff3..005c5000: helper value 5 must be exactly 1 to return the candidate. */
        cr = cm_fn_0090cfc0(5);
        if (cr == 1) {
            return cand;
        }

next:
        cand = cand + 0x16u;
        if (cand >= end) {
            return 0;
        }
    }
}
