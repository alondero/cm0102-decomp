/* Preferred-image VA 0x00951f87, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_00951f87(u32 left, u32 right, u32 output)
{
    u32 sum = left + right;
    u32 carry = sum < left || sum < right;
    CM_GLOBAL(volatile u32, output) = sum;
    return carry;
}
