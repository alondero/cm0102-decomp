/* Preferred-image VA 007e3650, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_THISCALL cm_fn_007e7760(u32 self, u32 input);

u32 CM_THISCALL cm_fn_007e3650(u32 self, u32 input)
{
    u32 first, second;
    cm_fn_007e7760(self, input);
    D(self) = 0x0096eeb4u;
    B(self + 4u) = 0;
    B(self + 5u) = 0x1c;
    B(self + 6u) = 0xc;
    first = D(0x009cf4a8u);
    D(self + 0x17u) = first;
    second = D(0x009cfa10u);
    B(self + 0x1fu) = 0x1c;
    D(self + 0x1bu) = second;
    B(self + 0x20u) = 0x1d;
    B(self + 0x21u) = 1;
    B(self + 0x22u) = 0;
    return self;
}
