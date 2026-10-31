/* Preferred-image VA 0x00945df0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
unsigned long long CM_CDECL __attribute__((regparm(3))) cm_fn_00945df0(u32 low, u32 high, u32 count)
{
    unsigned long long value = ((unsigned long long)high << 32) | low;
    unsigned long long fill = (high & 0x80000000u) ? ~0ull : 0ull;
    count = (u8)count;
    if (count >= 64u) return fill;
    if (!count) return value;
    return (value >> count) | (fill << (64u - count));
}
