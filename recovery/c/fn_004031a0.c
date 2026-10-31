/* Preferred-image VA 0x004031a0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_005e42b0(u32 address);

u32 CM_THISCALL cm_fn_004031a0(u32 self)
{
    u32 value = D(self + 0xba6u);
    if (value) {
        cm_fn_005e42b0(value);
        D(self + 0xba6u) = 0;
    }
    value = D(self + 0x200u);
    if (value) {
        cm_fn_005e42b0(value);
        D(self + 0x200u) = 0;
    }
    value = D(0x00ad9c58u);
    if (self + 0xb7au == value) D(0x00ad9c58u) = 0;
    return value;
}
