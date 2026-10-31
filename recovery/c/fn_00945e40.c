/* Preferred-image VA 0x00945e40, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
unsigned long long CM_STDCALL cm_fn_00945e40(u32 a_low, u32 a_high, u32 b_low, u32 b_high)
{
    unsigned long long product = (unsigned long long)a_low * b_low;
    u32 high = (u32)(product >> 32);
    high += a_high * b_low;
    high += a_low * b_high;
    return ((unsigned long long)high << 32) | (u32)product;
}
