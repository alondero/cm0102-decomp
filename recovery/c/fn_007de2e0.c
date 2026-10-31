/* Preferred-image VA 0x007de2e0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007e7760(u32 self, u32 input);

u32 CM_THISCALL cm_fn_007de2e0(u32 self, u32 input)
{
    u32 value;

    (void)cm_fn_007e7760(self, input);
    CM_GLOBAL(volatile u32, self) = 0x0096ebb4u;
    CM_GLOBAL(volatile u8, self + 4u) = 1;
    CM_GLOBAL(volatile u8, self + 5u) = 0x0e;
    CM_GLOBAL(volatile u8, self + 6u) = 0x0a;
    value = CM_GLOBAL(volatile u32, 0x009cf314u);
    CM_GLOBAL(volatile u32, self + 0x17u) = value;
    value = CM_GLOBAL(volatile u32, 0x009cfa10u);
    CM_GLOBAL(volatile u8, self + 0x20u) = 0x0e;
    CM_GLOBAL(volatile u32, self + 0x1bu) = value;
    CM_GLOBAL(volatile u8, self + 0x1fu) = 0x0d;
    CM_GLOBAL(volatile u8, self + 0x21u) = 1;
    CM_GLOBAL(volatile u8, self + 0x22u) = 0;
    return self;
}
