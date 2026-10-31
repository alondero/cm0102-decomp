#include "cmre.h"

/* Callee C: cdecl, one stack argument, returns the 32-bit value in EAX. */
extern u32 CM_CDECL cm_fn_005409b0(u32 *input);

/* Thiscall with ECX = object (read before written). Stack arguments: a (first)
   and b (second); the routine pops 8 bytes. Returns the byte in AL. */
u8 CM_THISCALL cm_fn_007754c0(u32 self, u32 a, u32 b)
{
    u32 r1;
    u32 sel;
    u32 p0;
    u32 p;
    u32 idx;
    u32 field;
    u8 out;

    r1 = cm_fn_005409b0((u32 *)a);

    /* bl = 0 when r1 == 0 and b == 0; 1 when r1 == 0 and b != 0;
       2 when r1 != 0 and b == 0; 1 when r1 != 0 and b != 0. */
    if (r1 != 0) {
        if (b == 0) {
            sel = 2;
        } else {
            sel = 1;
        }
    } else {
        if (b != 0) {
            sel = 1;
        } else {
            sel = 0;
        }
    }

    /* Second call on the same argument; its result is not used. */
    cm_fn_005409b0((u32 *)a);

    /* Pointer at a+0x53 to a u32 value p; index = 110*p + 20*sel (wrapping). */
    p0 = *(volatile u32 *)(a + 0x53);
    p = *(volatile u32 *)p0;
    idx = (p * 110u) + (sel * 20u);

    field = *(volatile u32 *)(self + 4);
    out = *(volatile u8 *)(idx + field + 0x42u);
    return out;
}
