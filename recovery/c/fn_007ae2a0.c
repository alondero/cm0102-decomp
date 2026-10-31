/* Preferred-image VA 0x007AE2A0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (two stack arguments a and b, ret 0), dword result in EAX.
   - a == 0 -> -1; b == 0 -> 1.
   - Global words W92 = [0x00AE2C92] and W90 = [0x00AE2C90] (unsigned/signed
     word compares as the listing uses them).
   - Select record a+0x10 when word a+0xa < W92, or when a+0xa == W92 and signed
     word a+8 < W90; otherwise a+0x8. The same selection is made for b.
   - If the word at selected a +2 differs from the word at selected b +2, return
     zero-extended difference (u16 minus u16).
   - Else if the words at selected a +0 and b +0 differ, return their signed
     word difference.
   - Else return the dword difference a[0] - b[0] (plain dword reads at a and b).
   Field meanings remain unknown. Global meanings remain unknown. */
#include "cmre.h"

s32 CM_CDECL cm_fn_007ae2a0(u8 *a, u8 *b)
{
    u8 *esi;
    u8 *edx;
    u16 ax;
    u16 cx;
    u16 dx;

    if (a == 0) {
        return -1;
    }
    if (b == 0) {
        return 1;
    }
    cx = CM_GLOBAL(u16, 0x00ae2c92);
    dx = CM_GLOBAL(u16, 0x00ae2c90);

    ax = *(u16 *)(a + 0x0a);
    if (ax < cx || (ax == cx && *(s16 *)(a + 0x08) < (s16)dx)) {
        esi = a + 0x10;
    } else {
        esi = a + 0x08;
    }

    ax = *(u16 *)(b + 0x0a);
    if (ax < cx || (ax == cx && *(s16 *)(b + 0x08) < (s16)dx)) {
        edx = b + 0x10;
    } else {
        edx = b + 0x08;
    }

    ax = *(u16 *)(esi + 0x02);
    cx = *(u16 *)(edx + 0x02);
    if (ax != cx) {
        return (s32)(u32)ax - (s32)(u32)cx;
    }

    if (*(u16 *)esi != *(u16 *)edx) {
        return (s32)*(s16 *)esi - (s32)*(s16 *)edx;
    }
    return (s32)(*(u32 *)a - *(u32 *)b);
}
