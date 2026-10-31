/* Preferred-image VA 0x00944250, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_009443a0(u32 allocator, u32 size);
extern u32 CM_CDECL cm_fn_009445d0(u32 allocator, u32 address);

u32 CM_CDECL cm_fn_00944250(u32 self, u32 size, u32 index)
{
    u32 offset = index * 4u;
    u32 value = D(self + offset + 0x80u);
    if (!value) return cm_fn_009443a0(self, size);
    D(self + offset + 0x80u) = 0;
    value = D(self + offset + 0x68u);
    if (size > value) {
        value = D(self + offset + 0x50u);
        cm_fn_009445d0(self, value);
        value = cm_fn_009443a0(self, size);
        D(self + offset + 0x50u) = value;
        D(self + offset + 0x68u) = size;
    }
    return D(self + offset + 0x50u);
}
