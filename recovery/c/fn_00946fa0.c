/* Preferred-image VA 0x00946fa0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
s32 CM_CDECL cm_fn_00946fa0(u32 first, u32 second, u32 count)
{
    u32 length = 0;
    u32 i;
    u8 a, b;
    if (!count)
        return 0;
    do {
        a = CM_GLOBAL(volatile u8, first + length);
        ++length;
    } while (a && length < count);
    i = 0;
    do {
        b = CM_GLOBAL(volatile u8, second + i);
        a = CM_GLOBAL(volatile u8, first + i);
        ++i;
    } while (a == b && i < length);
    b = CM_GLOBAL(volatile u8, second + i - 1u);
    a = CM_GLOBAL(volatile u8, first + i - 1u);
    if (b > a)
        return -1;
    if (b < a)
        return 1;
    return 0;
}
