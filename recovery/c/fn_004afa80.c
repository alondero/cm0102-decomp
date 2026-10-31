#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);

/* 32-bit field at byte offset `off` from the address `addr`, read through a
   volatile access so every load happens in listing order. */
#define FIELD32(addr, off) (*(volatile u32 *)((addr) + (off)))

/* Cdecl, two stack arguments (p = arg1 at entry+4, s = arg2 at entry+8),
   `ret 0`. ECX and EDX are not read before written.
   Return values come from partial-register writes in the listing: `setcc al`
   and `mov al, K` keep the upper 24 bits of EAX, which hold the last loaded
   field value. Those returns are written as ((value & 0xffffff00) | K), and
   the `inc eax` / `add eax, K` forms add to the full 32-bit value.
   Field offsets: s+0x18 is a pointer (y), s+0x1c and s+0x20 are pointers,
   y+0x38 is a pointer (z), z+0x1f, p+0x53 and p+0x69 are 32-bit fields. */
u32 CM_CDECL cm_fn_004afa80(u32 p, u32 s)
{
    u32 y;
    u32 x;
    u32 z;
    u32 q;
    u32 other;
    u32 pf69;
    u32 qq;
    u32 zf1f;
    u32 o53;

    y = FIELD32(s, 0x18);
    if (y == 0) {
        x = FIELD32(s, 0x1c);
        q = FIELD32(x, 0x69);
        if (q == 0)
            return 3;
        return ((q & 0xffffff00u) | (u8)(p != x)) + 1u;
    }

    if (cm_fn_0053b900((u32 *)p) == 0) {
        x = FIELD32(s, 0x1c);
        q = (x == p) ? FIELD32(s, 0x20) : x;
        y = FIELD32(s, 0x18);
        pf69 = FIELD32(p, 0x69);
        if (y != pf69) {
            qq = FIELD32(q, 0x69);
            return ((pf69 & 0xffffff00u) | (u8)(y != qq)) + 2u;
        }
        qq = FIELD32(q, 0x69);
        if (y == qq)
            return ((pf69 & 0xffffff00u) | (u8)(p != x)) + 1u;
        return (pf69 & 0xffffff00u) | 1u;
    }

    z = FIELD32(FIELD32(s, 0x18), 0x38);
    if (z != 0) {
        zf1f = FIELD32(z, 0x1f);
        o53 = FIELD32(p, 0x53);
        if (zf1f == o53)
            return (z & 0xffffff00u) | 1u;
    }

    x = FIELD32(s, 0x1c);
    other = (x == p) ? FIELD32(s, 0x20) : x;
    if (z == 0)
        return 3;
    zf1f = FIELD32(z, 0x1f);
    o53 = FIELD32(other, 0x53);
    if (zf1f != o53)
        return (o53 & 0xffffff00u) | 3u;
    return (o53 & 0xffffff00u) | 2u;
}
