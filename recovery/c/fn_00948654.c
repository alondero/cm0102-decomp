/* Preferred-image VA 0x00948654, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL cm_fn_00948654(u32 value)
{
    if (value == 0x3a4u) return 0x411u;
    if (value == 0x3a8u) return 0x804u;
    if (value == 0x3b5u) return 0x412u;
    if (value == 0x3b6u) return 0x404u;
    return 0;
}
