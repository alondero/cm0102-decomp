/* Preferred-image VA 0x006834F0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (plain ret) with two stack arguments; returns a signed 32-bit
   difference in EAX, or 0. Both arguments must have a nonzero first dword. The
   signed words at +0x19 are compared first; if they differ the result is
   (second +0x19) - (first +0x19). Otherwise the global dword 0x00B679FC is a
   base address; for i = 0..3 the byte at base+0xC5+i selects a case
   (value-1 as unsigned, cases 0..4). Each case compares one field pair and
   returns second - first on the first difference. A zero byte returns 0.
   Field meanings remain unknown. Object accesses are volatile. */
#include "cmre.h"

s32 CM_CDECL cm_fn_006834f0(u8 *first, u8 *second)
{
    volatile u8 *a = (volatile u8 *)first;
    volatile u8 *b = (volatile u8 *)second;
    u32 base;
    u32 ebx;
    u32 edi;
    u32 kind_index;
    s32 pa;
    s32 pb;

    if (*(volatile u32 *)a == 0) {
        return 0;
    }
    if (*(volatile u32 *)b == 0) {
        return 0;
    }
    if (*(volatile s16 *)(a + 0x19) != *(volatile s16 *)(b + 0x19)) {
        pa = (s32)*(volatile s16 *)(a + 0x19);
        pb = (s32)*(volatile s16 *)(b + 0x19);
        return pb - pa;
    }
    base = CM_GLOBAL(u32, 0x00b679fc);
    ebx = 0xffffff3bu - base;
    edi = base + 0xc5u;
    do {
        u8 kind = *(volatile u8 *)edi;

        if (kind == 0) {
            return 0;
        }
        kind_index = (u32)((s32)(s8)kind - 1);
        switch (kind_index) {
        case 0:
            pa = (s32)*(volatile s16 *)(a + 0x15) - (s32)*(volatile s16 *)(a + 0x17);
            pb = (s32)*(volatile s16 *)(b + 0x15) - (s32)*(volatile s16 *)(b + 0x17);
            if (pa != pb) {
                return pb - pa;
            }
            break;
        case 1:
            pa = (s32)*(volatile s16 *)(a + 0x15);
            pb = (s32)*(volatile s16 *)(b + 0x15);
            if (pa != pb) {
                return pb - pa;
            }
            break;
        case 2:
            pa = (s32)*(volatile s8 *)(a + 0x12);
            pb = (s32)*(volatile s8 *)(b + 0x12);
            if (pa != pb) {
                return pb - pa;
            }
            break;
        case 3:
            break;
        case 4:
            pa = (s32)*(volatile s16 *)(a + 0x31);
            pb = (s32)*(volatile s16 *)(b + 0x31);
            if (pa != pb) {
                return pb - pa;
            }
            break;
        default:
            break;
        }
        edi = edi + 1u;
    } while ((s32)(ebx + edi) < 4);
    return 0;
}
