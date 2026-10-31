/* Preferred-image VA 007ae950, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))

extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second,
                                  u32 third, u32 date);

u32 CM_THISCALL cm_fn_007ae950(u32 self)
{
    u32 index = D(self + 0x180u);
    u32 first, second, third, base;

    if ((s32)index <= -1)
        return 0;
    third = D(self + 0x17cu);
    second = D(self + 0x178u);
    first = D(self + 0x174u);
    base = D(0x00ae23bcu);
    if (cm_fn_0053e990(base + index * 110u, first, second, third,
                       self + 0x184u) == 0)
        return 0;
    index = D(self + 0x180u);
    base = D(0x00ae23bcu);
    return base + index * 110u;
}
