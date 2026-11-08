/* Preferred-image VA 0x00567b00; CM0102 3.9.68, base 0x00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical EAX/position-only EDX/ECX entry, no native stack arguments.
   The two early returns retain the actual incoming EAX. */
#include "cmre.h"
u32 CM_CDECL __attribute__((regparm(3))) cm_fn_00567b00(
    u32 incoming_eax, u32 incoming_edx, u32 self)
{
    u32 destination, source, cursor, remaining, length, count, word;
    u8 byte;
    (void)incoming_edx;
    if (!(CM_GLOBAL(volatile u8, self + 0xcu) & 8u)) return incoming_eax;
    destination = CM_GLOBAL(volatile u32, self + 0x54u);
    if (!destination) return incoming_eax;
    source = self + 0x80u;
    cursor = source;
    remaining = 0xffffffffu;
    do {
        byte = CM_GLOBAL(volatile u8, cursor);
        cursor += 1u;
        remaining -= 1u;
    } while (remaining && byte != 0);
    length = ~remaining;
    count = length >> 2;
    while (count) {
        word = CM_GLOBAL(volatile u32, source);
        CM_GLOBAL(volatile u32, destination) = word;
        source += 4u;
        destination += 4u;
        count -= 1u;
    }
    count = length & 3u;
    while (count) {
        byte = CM_GLOBAL(volatile u8, source);
        CM_GLOBAL(volatile u8, destination) = byte;
        source += 1u;
        destination += 1u;
        count -= 1u;
    }
    return length;
}
