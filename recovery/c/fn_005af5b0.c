#include "cmre.h"

/* Cdecl, one stack argument arg_1 (ret 0). Reads a pointer at arg_1+0x53 (P); a
   zero P returns 0xff. Q = dword at P. Globals 0x009cf2e4 (G1) and 0x009cf510 (G2)
   select a base: when Q == G1 the selector is G2; otherwise Q must equal G2 (else
   return G1|0xff) and the selector is G1. base = dword(0x00ae23a8) + selector*290.
   Scan k = 0..34 over 72-byte rows at 0x00b63d60: a nonzero row item whose dword at
   +0x11c is nonzero and equals base yields the byte at 0x00b63d74 + 72k. If nothing
   matches, return (last item) | 0xff. */
u32 CM_CDECL cm_fn_005af5b0(u32 arg_1)
{
    u32 p;
    u32 q;
    u32 g1;
    u32 sel;
    u32 base;
    u32 item;
    u8 k;

    p = *(volatile u32 *)(arg_1 + 0x53u);
    if (p == 0u) {
        return 0xffu;
    }
    q = *(volatile u32 *)p;

    g1 = CM_GLOBAL(volatile u32, 0x009cf2e4);
    if (q == g1) {
        sel = CM_GLOBAL(volatile u32, 0x009cf510);
    } else {
        if (q != CM_GLOBAL(volatile u32, 0x009cf510)) {
            return g1 | 0xffu;
        }
        sel = g1;
    }

    base = CM_GLOBAL(volatile u32, 0x00ae23a8) + sel * 290u;

    item = 0u;
    for (k = 0u; k < 0x23u; k = (u8)(k + 1u)) {
        item = CM_GLOBAL(volatile u32, 0x00b63d60u + 72u * (u32)k);
        if (item != 0u) {
            if (*(volatile u8 *)(item + 0x11cu) != 0u) {
                if (item == base) {
                    return CM_GLOBAL(volatile u8, 0x00b63d74u + 72u * (u32)k);
                }
            }
        }
    }

    return item | 0xffu;
}
