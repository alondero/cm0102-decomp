/* Preferred-image VA 0x00951d10, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_00951d10(u32 input, u32 chars)
{
    volatile u32 bitmap[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    u8 byte;
    while ((byte = CM_GLOBAL(volatile u8, chars)) != 0) {
        ++chars;
        bitmap[byte >> 5] |= 1u << (byte & 31u);
    }
    while ((byte = CM_GLOBAL(volatile u8, input)) != 0) {
        ++input;
        if (bitmap[byte >> 5] & (1u << (byte & 31u)))
            return input - 1u;
    }
    return 0;
}
