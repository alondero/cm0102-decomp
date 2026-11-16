/* fn_007ec6c0 - preferred-image VA 0x007ec6c0, image base 0x00400000,
   CM0102/Game/cm0102.exe 3.9.68 (SHA-256 0c78e7da...f86).  169 bytes.

   Plain C translation of the listed body.  The original is a thiscall routine:
   ECX (read before written) is the object, the 16 caller stack words are never
   read, and `ret 0` leaves the caller's stack untouched.

   Every memory access and call is kept in listing order.

   The fourth argument of 0x007671c0 (the RETRY word at [P + 0x0d]):
   `mov di, word ptr [ecx + 0xd]` (0x007ec704, 0x007ec750) writes only the low
   half of EDI, so the `push edi` (0x007ec70b, 0x007ec757) carries the
   caller's entry EDI in the upper 16 bits.  C cannot name an incoming
   callee-saved register, so the word is passed with the upper half cleared.
   That is exactly equivalent, because the forwarded dword is masked before
   any use:
     - 0x007671e0  mov dx, word ptr [esp + 0x20]   (0x007671c0 compares the
       low word of the argument only),
     - 0x00767285  mov edx, dword ptr [esp + 0x24]   (the full dword is
       forwarded: `push edx` at 0x0076728d, `call 0x86c8c0` at 0x007672cb,
       where it becomes the fifth stack argument),
     - 0x0086c96b  and edx, 0xffff                   (0x0086c8c0 keeps only
       the low 16 bits of that argument).
   The caller's entry EDI upper half is therefore discarded before it can
   affect anything, and the plain-C zero extension reproduces the original
   exactly. */

#include "cmre.h"

/* 0x007671c0: ECX = receiver; four stack words pushed right to left, so
   a1 = eax, a2 = edx, a3 = ecx, a4 = edi. */
extern void CM_THISCALL cm_fn_007671c0(u8 *self, u32 a1, u32 a2, u32 a3, u32 a4);

/* 0x00768130: ECX = receiver; two stack words, a1 = the global, a2 = -1. */
extern u32 CM_THISCALL cm_fn_00768130(u32 self, u32 target, u32 mode_arg);

void CM_THISCALL cm_fn_007ec6c0(u32 self)
{
    u32 p;        /* [self + 0x0a], the record pointer */
    u32 q;        /* re-read of [self + 0x0a] */
    u32 obj;      /* [self + 0x04] */
    u32 r;        /* return of 0x00768130 */
    u32 x;        /* *r, the table index */
    u32 t;        /* lea chain */
    u32 e;        /* lea chain */
    u32 row;      /* table row pointer */
    u32 value;    /* EDX: *row, or 0 when the row pointer is null */
    u32 word;     /* DI: the word at [P + 0x0d] */
    u32 byte_arg; /* ECX: (P & 0xffffff00) | P[0x0c] */
    u32 receiver; /* ECX of the 0x007671c0 call */
    u32 g;        /* the global argument of 0x00768130 */

    p = *(volatile u32 *)(self + 0x0au);
    if (*(volatile u8 *)(p + 0x0cu) != 5u) {
        /* ---- first block: global 0x009cf930, receiver at [obj + 0x0c] ---- */
        g = CM_GLOBAL(u32, 0x009cf930);
        r = cm_fn_00768130(p, g, 0xffffffffu);
        if (r != 0u) {
            x = *(volatile u32 *)r;
            t = x + x * 4u;              /* lea ecx, [eax + eax*4]  (0x007ec6e4) */
            t = x + t * 2u;              /* lea ecx, [eax + ecx*2]  (0x007ec6e7) */
            e = t + t * 4u;              /* lea edx, [ecx + ecx*4]  (0x007ec6ea) */
            row = *(volatile u32 *)(CM_GLOBAL(u32, 0x00ae23bc) + e * 2u + 0x39u);
            if (row != 0u) {
                value = *(volatile u32 *)row;   /* mov edx, [ecx] (0x007ec6fb) */
            } else {
                value = 0u;                    /* xor edx, edx (0x007ec6ff) */
            }
            q = *(volatile u32 *)(self + 0x0au);        /* 0x007ec701 */
            word = *(volatile u16 *)(q + 0x0du);        /* mov di, [ecx+0xd] */
            byte_arg = (q & 0xffffff00u)
                       | (u32)*(volatile u8 *)(q + 0x0cu); /* mov cl, [ecx+0xc] */
            obj = *(volatile u32 *)(self + 0x04u);       /* 0x007ec70e */
            receiver = *(volatile u32 *)(obj + 0x0cu);   /* 0x007ec712 */
            cm_fn_007671c0((u8 *)receiver, x, value, byte_arg, word);
        }

        /* ---- second block: global 0x009cf92c, receiver at [obj + 0x18] ---- */
        g = CM_GLOBAL(u32, 0x009cf92c);
        q = *(volatile u32 *)(self + 0x0au);            /* 0x007ec71f */
        r = cm_fn_00768130(q, g, 0xffffffffu);
        if (r != 0u) {
            x = *(volatile u32 *)r;
            t = x + x * 4u;              /* lea ecx, [eax + eax*4]  (0x007ec730) */
            t = x + t * 2u;              /* lea ecx, [eax + ecx*2]  (0x007ec733) */
            e = t + t * 4u;              /* lea edx, [ecx + ecx*4]  (0x007ec736) */
            row = *(volatile u32 *)(CM_GLOBAL(u32, 0x00ae23bc) + e * 2u + 0x39u);
            if (row != 0u) {
                value = *(volatile u32 *)row;   /* mov edx, [ecx] (0x007ec747) */
            } else {
                value = 0u;                    /* xor edx, edx (0x007ec74b) */
            }
            q = *(volatile u32 *)(self + 0x0au);        /* 0x007ec74d */
            word = *(volatile u16 *)(q + 0x0du);        /* mov di, [ecx+0xd] */
            byte_arg = (q & 0xffffff00u)
                       | (u32)*(volatile u8 *)(q + 0x0cu); /* mov cl, [ecx+0xc] */
            obj = *(volatile u32 *)(self + 0x04u);       /* 0x007ec75a */
            receiver = *(volatile u32 *)(obj + 0x18u);   /* 0x007ec75e */
            cm_fn_007671c0((u8 *)receiver, x, value, byte_arg, word);
        }
    }
}
