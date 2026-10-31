/* Preferred-image VA 0x007f6aa0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern void CM_STDCALL cm_fn_009462bb(u32 base, u32 size, u32 count, u32 destructor);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 value);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);

u32 CM_THISCALL cm_fn_007f6aa0(u32 self)
{
    u32 value = D(self), count;
    if (value) {
        count = D(value - 4u);
        cm_fn_009462bb(value, 0x2b0u, count, 0x007ae4f0u);
        cm_fn_00944c94(value - 4u);
        D(self) = 0;
    }
    value = D(self + 0x5cu);
    if (value) {
        value = (u32)cm_fn_009452ca(value);
        D(self + 0x5cu) = 0;
    }
    return value;
}
