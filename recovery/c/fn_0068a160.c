#include "cmre.h"

/* Thiscall callee: ECX is the scratch buffer; four stack arguments, popped by
   the callee. The return value is not used by this routine. */
extern u32 CM_THISCALL cm_fn_00549f70(u32 self, u32 a1, u32 a2, u32 a3, u32 a4);

/* Cdecl, nine stack arguments (caller cleans up). p1 is a base pointer; p2 is
   read as a signed 16-bit index (low word only); p7 contributes its low byte
   and p9 is stored as a dword. The routine returns the element address it
   wrote, which is the last value left in EAX. */
u32 CM_CDECL cm_fn_0068a160(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5,
                            u32 p6, u32 p7, u32 p8, u32 p9)
{
    /* 8-byte scratch buffer at the base of the routine's `sub esp, 8` frame.
       The callee fills it; offsets 0 (word) and 2 (dword) are read back. */
    u8 scratch[8];
    s16 index;
    u32 dst;
    u16 cx;
    u32 dx;

    cm_fn_00549f70((u32)scratch, p3, p4, p5 + p8, p6);

    index = (s16)(u16)p2;
    dst = p1 + (u32)(s32)index * 65u;

    cx = *(volatile u16 *)(scratch + 0);
    dx = *(volatile u32 *)(scratch + 2) - p8;

    *(volatile u16 *)(dst + 0u) = cx;
    *(volatile u16 *)(dst + 2u) = (u16)dx;
    *(volatile u8 *)(dst + 4u) = (u8)p7;
    *(volatile u32 *)(dst + 0x3du) = p9;
    return dst;
}
