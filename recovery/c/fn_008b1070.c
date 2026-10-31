/* Preferred-image VA 0x008b1070, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);
extern unsigned long long CM_STDCALL cm_fn_009462bb(u32 base, u32 size, u32 count, u32 destructor);

unsigned long long CM_THISCALL cm_fn_008b1070(u32 self)
{
    u32 value = D(self + 0x651du);
    if (value) {
        cm_fn_009452ca(value);
        D(self + 0x651du) = 0;
    }
    value = D(self + 0x6521u);
    if (value) {
        cm_fn_009452ca(value);
        D(self + 0x6521u) = 0;
    }
    return cm_fn_009462bb(self, 0x9bu, 0xa7u, 0x005a53c0u);
}
