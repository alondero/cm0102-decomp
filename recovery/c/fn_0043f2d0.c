/* Preferred-image VA 0x0043f2d0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   EAX retains high16(count-1) and the final unswapped WORD in AX. */
#include "cmre.h"

u32 CM_CDECL cm_fn_0043f2d0(u32 address, u32 count)
{
    register u32 cur = address;
    register u32 remaining = count;
    register u32 word = 0u;
    u32 high;

    if (count == 0u) {
        return count - 1u;
    }

    high = (count - 1u) & 0xffff0000u;

    do {
        word = (u32)(*(volatile u16 *)cur);
        cur += 2u;
        --remaining;
        *(volatile u16 *)(cur - 2u) = __builtin_bswap16((u16)word);
    } while (remaining != 0u);

    return high | word;
}
