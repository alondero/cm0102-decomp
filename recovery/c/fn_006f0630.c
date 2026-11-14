/* fn_006f0630 -- preferred-image VA 0x006f0630, image base 0x00400000.
 *
 * Convention: ECX is read before written, so ECX is the receiver (`this`).
 * The callers read EDX after the call (0x006c5023), so the result is the
 * 64-bit pair EDX:EAX; the routine returns that pair.
 *
 * EDX is an *input* on the path that leaves the body at 0x006f0960 (the
 * `[self+0x19e] == 0` early exit): the original never writes EDX there, so
 * the value the caller sees in EDX is the EDX it passed in.  Plain C cannot
 * read the incoming EDX through a thiscall receiver, so EDX is taken as the
 * second (fastcall) register argument and returned unchanged on that path.
 * Every other path writes EDX explicitly and its value is reproduced.
 */
#include "cmre.h"

/* Field access by byte offset from an object address. Every access is volatile
   so the listing's read and write order is kept. The object at self+0x19e is
   re-read wherever the listing re-reads it. */
#define OBJ_U8(base, off) (*(volatile u8 *)((u32)(base) + (off)))
#define OBJ_S8(base, off) (*(volatile s8 *)((u32)(base) + (off)))
#define OBJ_U16(base, off) (*(volatile u16 *)((u32)(base) + (off)))
#define OBJ_U32(base, off) (*(volatile u32 *)((u32)(base) + (off)))

/* Called with the object at self+0x19e as ECX and self as the one stack
   argument; the callee pops it. Only AX is compared (cmp ax, 1). */
extern u16 CM_THISCALL cm_fn_006c38c0(u32 recv, u32 arg);

unsigned long long CM_FASTCALL cm_fn_006f0630(u32 self, u32 edx_in)
{
    u32 p;
    u32 q;
    u32 a;
    u32 idx;
    u32 v;
    u32 eax_val;
    u32 edx_val;
    u16 w19a;
    u16 r;
    u8 al;
    u8 al1;
    u8 cl;
    u8 dl;
    s8 s102;
    s8 s103;
    s32 t;
    u8 ok;

    p = OBJ_U32(self, 0x19e);
    if (p == 0) {
        /* 0x006f0636 loads EAX with the null pointer and 0x006f063e jumps
           straight to the epilogue: EAX is 0 and EDX still holds the
           caller's EDX, which this routine never writes. */
        return ((unsigned long long)edx_in << 32);
    }
    q = OBJ_U32(p, 0xf582);
    if (q != 0) {
        OBJ_U32(OBJ_U32(q, 0x19e), 0x4786) = 0;
        OBJ_U8(OBJ_U32(q, 0x19e), 0x8eae) = 0xff;
        OBJ_U32(OBJ_U32(q, 0x19e), 0xf582) = 0;
        OBJ_U8(OBJ_U32(q, 0x19e), 0x1a7) = 0;
        OBJ_U16(q, 0x29) = 0;
        OBJ_U16(q, 0x19c) = 0;
    }

    OBJ_U8(OBJ_U32(self, 0x19e), 0x1a4) = 0;
    OBJ_U8(OBJ_U32(self, 0x19e), 0x1a7) = 0;
    OBJ_U8(OBJ_U32(self, 0x19e), 0x1cd) = 0;
    OBJ_U32(OBJ_U32(self, 0x19e), 0x4786) = 0;

    a = OBJ_U32(OBJ_U32(self, 0x19e), 0xf576);
    if (a != 0) {
        dl = OBJ_U8(self, 0x27);
        cl = OBJ_U8(a, 0x27);
        if (dl == cl) {
            cl = OBJ_U8(a, 0x02);
            cl = (u8)(cl + 1);
            OBJ_U8(a, 0x02) = cl;

            r = cm_fn_006c38c0(OBJ_U32(self, 0x19e), self);
            if (r == 1) {
                OBJ_U16(OBJ_U32(OBJ_U32(self, 0x19e), 0xf576), 0x35) += 0x6c;
            } else {
                al = OBJ_U8(self, 0x27);
                if ((al == 1 && OBJ_S8(self, 0x103) >= 6) ||
                    (al == 0 && OBJ_S8(self, 0x103) < 6)) {
                    OBJ_U16(OBJ_U32(OBJ_U32(self, 0x19e), 0xf576), 0x35) += 0x60;
                } else if ((al == 1 && OBJ_S8(self, 0x103) < 3) ||
                           (al == 0 && OBJ_S8(self, 0x103) > 8)) {
                    OBJ_U16(OBJ_U32(OBJ_U32(self, 0x19e), 0xf576), 0x35) += 0x96;
                } else {
                    OBJ_U16(OBJ_U32(OBJ_U32(self, 0x19e), 0xf576), 0x35) += 0x78;
                }
            }

            p = OBJ_U32(self, 0x19e);
            v = OBJ_U32(p, 0x4782);
            v += 0xf0;
            OBJ_U32(p, 0x4782) = v;

            p = OBJ_U32(self, 0x19e);
            v = OBJ_U32(p, 0xf576);
            OBJ_U32(p, 0xf5aa) = self;
            OBJ_U32(p, 0xf5a6) = v;
        }
    }

    OBJ_U32(OBJ_U32(self, 0x19e), 0xf576) = 0;
    OBJ_U32(OBJ_U32(self, 0x19e), 0xf57a) = 0;
    OBJ_U32(OBJ_U32(self, 0x19e), 0x477e) = 0;

    r = cm_fn_006c38c0(OBJ_U32(self, 0x19e), self);
    if (r == 1) {
        al = OBJ_U8(self, 0x27);
        s102 = OBJ_S8(self, 0x102);
        ok = 0;
        if (s102 >= 2 && s102 <= 6) {
            s103 = OBJ_S8(self, 0x103);
            if (al == 1) {
                ok = (s103 >= 10 && s103 <= 11);
            } else {
                ok = (s103 >= 0 && s103 <= 1);
            }
        }
        if (ok) {
            p = OBJ_U32(self, 0x19e);
            idx = (u32)(s32)(s8)al;
            if (OBJ_U8(p + idx, 0x1a5) == 0) {
                /* neg al / sbb eax,eax / and eax,0xb4 keeps 0xb4 for a
                   non-zero AL and 0 otherwise. */
                OBJ_U16(self, 0x19a) = (al != 0) ? 0xb4 : 0;
                OBJ_U8(p, 0x8ea9) = 0xff;
            }
        }
    }

    al = OBJ_U8(self, 0x27);
    if ((al == 1 && OBJ_S8(self, 0x103) >= 6) ||
        (al == 0 && OBJ_S8(self, 0x103) < 6)) {
        OBJ_U8(OBJ_U32(self, 0x19e), 0x8eb1) = 0;
    }

    t = (s32)OBJ_S8(self, 0x27);
    p = OBJ_U32(self, 0x19e);
    OBJ_U8(p + (u32)t, 0x1a5) = 1;

    al = OBJ_U8(self, 0x27);
    p = OBJ_U32(self, 0x19e);
    t = (al == 0) ? 1 : 0;
    OBJ_U8(p + (u32)t, 0x1a5) = 0;

    p = OBJ_U32(self, 0x19e);
    cl = OBJ_U8(self, 0x102);
    OBJ_U8(p, 0x8ea7) = cl;

    p = OBJ_U32(self, 0x19e);
    al = OBJ_U8(self, 0x103);
    OBJ_U8(p, 0x8ea8) = al;

    p = OBJ_U32(self, 0x19e);
    dl = OBJ_U8(self, 0x27);
    OBJ_U8(p, 0x8eae) = dl;

    OBJ_U32(OBJ_U32(self, 0x19e), 0xf582) = self;

    p = OBJ_U32(self, 0x19e);
    w19a = OBJ_U16(self, 0x19a);
    OBJ_U16(p, 0x8ed8) = w19a;

    OBJ_U8(OBJ_U32(self, 0x19e), 0x8eaa) = 0;

    p = OBJ_U32(self, 0x19e);
    dl = OBJ_U8(self, 0x27);
    cl = OBJ_U8(p, 0x8eab);
    if (cl != dl) {
        OBJ_U32(p, 0x4782) = 0;
    }

    al = OBJ_U8(self, 0x27);
    p = OBJ_U32(self, 0x19e);
    OBJ_U8(p, 0x8eab) = al;
    if (al != 0xff) {
        idx = (u32)(s32)(s8)al;
        OBJ_U8(p + idx, 0x8d46) += 1;
        t = (al == 0) ? 1 : 0;
        al1 = OBJ_U8(p + (u32)t, 0x8d46);
        if ((s8)al1 > 0) {
            al1 = (u8)(al1 - 10);
            OBJ_U8(p + (u32)t, 0x8d46) = al1;
        }
        /* Registers at the ret: EAX low byte is the last loaded/adjusted byte
           over the object pointer's upper bits; EDX is the 0/1 selector. */
        eax_val = (p & 0xffffff00u) | al1;
        edx_val = (u32)t;
    } else {
        /* Byte 0xff skips the block; EAX low byte is 0xff, and EDX keeps the
           pointer's high word, the word at +0x19a in bits 8..15, and 0xff. */
        eax_val = (p & 0xffffff00u) | 0xffu;
        edx_val = (p & 0xffff0000u) | ((u32)w19a & 0xff00u) | 0xffu;
    }

    OBJ_U16(self, 0x29) = 0;
    OBJ_U16(self, 0x19c) = 0;
    return ((unsigned long long)edx_val << 32) | eax_val;
}
