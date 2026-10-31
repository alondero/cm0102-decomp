/* Preferred-image VA 0x00944300, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_009445d0(u32 allocator, u32 address);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);

unsigned long long CM_CDECL cm_fn_00944300(u32 self)
{
    u32 index;
    for (index = 0; index < 6; ++index) {
        u32 slot = self + 0x50u + index * 4u;
        cm_fn_009445d0(self, D(slot));
        D(slot) = 0;
        D(slot + 0x18u) = 0;
        D(slot + 0x30u) = 1;
    }
    cm_fn_009452ca(D(self + 0xcu));
    return cm_fn_009452ca(self);
}
