/* Preferred-image VA 0x0070a8c0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u8 CM_STDCALL cm_fn_0070a8c0(u32 value)
{
    u16 word = (u16)value;
    return word == 0x0d || word == 0x0c || word == 0x15 ||
           word == 0x2e || word == 0x3b || word == 0x25;
}
