#include "cmre.h"

/* Callee C: cdecl, two byte-string pointers, returns 0 on match. */
extern s32 CM_CDECL cm_fn_0094f4e0(u8 *s1, u8 *s2);

/* Cdecl, three stack arguments (caller cleans up), returns 0 or 1 in EAX:
     a   object A; field +0x04 is a pointer X to a record whose byte +0x08 must be
         non-zero (else the routine returns 1); flag bytes are read at +0x00
     b   object B; field +0x04 is a pointer Y; flag byte +0x00
     p3  pointer to a dword flag word F
   The string call compares X+0x08 with Y+0x08 only when X != Y. */
u32 CM_CDECL cm_fn_0094a046(u32 a, u32 b, u32 p3)
{
    u32 x;
    u32 y;
    s32 r;
    u32 f;

    x = *(volatile u32 *)(a + 4);
    if (x == 0) {
        return 1;
    }
    if (*(volatile u8 *)(x + 8) == 0) {
        return 1;
    }

    y = *(volatile u32 *)(b + 4);
    if (x != y) {
        r = cm_fn_0094f4e0((u8 *)(x + 8), (u8 *)(y + 8));
        if (r != 0) {
            return 0;
        }
    }

    if ((*(volatile u8 *)b & 2) != 0) {
        if ((*(volatile u8 *)a & 8) == 0) {
            return 0;
        }
    }

    f = *(volatile u32 *)p3;
    if ((f & 1) != 0) {
        if ((*(volatile u8 *)a & 1) == 0) {
            return 0;
        }
    }

    if ((f & 2) == 0) {
        return 1;
    }
    if ((*(volatile u8 *)a & 2) != 0) {
        return 1;
    }
    return 0;
}
