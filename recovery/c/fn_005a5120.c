/* Preferred-image VA 0x005a5120, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern void CM_FASTCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 value);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);

u32 CM_THISCALL cm_fn_005a5120(u32 self)
{
    u32 value = D(self);
    if (value) {
        cm_fn_0068d040(value);
        cm_fn_00944c94(value);
        D(self) = 0;
    }
    value = D(self + 4u);
    if (value) {
        cm_fn_0068d040(value);
        cm_fn_00944c94(value);
        D(self + 4u) = 0;
    }
    value = D(self + 8u);
    if (value) {
        value = (u32)cm_fn_009452ca(value);
        D(self + 8u) = 0;
    }
    return value;
}
