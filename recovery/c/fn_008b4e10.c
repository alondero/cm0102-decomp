/* Preferred-image VA 0x008b4e10, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_008bba10(u32 self);
u32 CM_THISCALL cm_fn_008b4e10(u32 self)
{
    cm_fn_008bba10(self + 0x18u);
    CM_GLOBAL(volatile u8, self + 0x2eu) = 0;
    CM_GLOBAL(volatile u16, self + 0x16u) = 0;
    CM_GLOBAL(volatile u32, self + 0x97u) = 0;
    CM_GLOBAL(volatile u32, self + 0x29u) = 0x200cbu;
    CM_GLOBAL(volatile u8, self + 0x2du) = 1;
    CM_GLOBAL(volatile u32, self + 0x93u) = 1;
    return self;
}
