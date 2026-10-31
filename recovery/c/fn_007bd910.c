/* Preferred-image VA 0x007bd910, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

u32 CM_CDECL __attribute__((regparm(3))) cm_fn_007bd910(
    u32 incoming_eax, u32 incoming_edx, u32 self)
{
    s32 count = (s8)*(volatile u8 *)(self + 0x1a1u);
    u8 index = *(volatile u8 *)(self + 0x1a0u);
    u32 sum = 0;
    (void)incoming_edx;
    if (count <= 0) return incoming_eax & 0xffff0000u;
    do {
        s32 slot = (s8)index;
        sum += (u32)(s32)(s8)*(volatile u8 *)(self + (u32)slot * 41u + 0x18u);
        index++;
        if (index == 10) index = 0;
    } while (--count != 0);
    return sum & 0xffffu;
}
