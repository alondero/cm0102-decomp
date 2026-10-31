#include "cmre.h"

/* Classifies an IEEE double passed as two cdecl stack dwords: the low dword is the
   first argument ([ebp+8]) and the high dword the second ([ebp+0xc]). Returns the
   class in EAX as a full dword. */
u32 cm_fn_00950fc4(u32 low, u32 high)
{
    u32 bits;

    if (high == 0x7ff00000u) {
        if (low == 0u) {
            return 1u;
        }
    } else if (high == 0xfff00000u && low == 0u) {
        return 2u;
    }

    /* The listing reads the dword at [ebp+0xe] and masks it with 0x7ff8 before the
       16-bit compare, so only bits 16..31 of the high dword contribute. */
    bits = (high >> 16) & 0x7ff8u;
    if (bits == 0x7ff8u) {
        return 3u;
    }
    if (bits != 0x7ff0u) {
        return 0u;
    }
    if ((high & 0x7ffffu) == 0u && low == 0u) {
        return 0u;
    }
    return 4u;
}
