/* Preferred-image VA 0055ef50, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

u32 CM_THISCALL cm_fn_0055ef50(u32 self, u32 input)
{
    D(self + 0x12f541u) = input;
    D(self + 0x12f525u) = 0;
    D(self + 0x12f521u) = 0;
    D(self + 0x12f505u) = 0;
    D(self + 0x12f555u) = 0;
    D(self + 0x12f53du) = 0;
    D(self + 0x12f535u) = 0;
    D(self + 0x12f545u) = 0;
    D(self) = 0;
    B(self + 0x12f55du) = 1;
    D(self + 0x12f529u) = 0;
    W(self + 0x12e99eu) = 0;
    W(self + 0x12e9a0u) = 0;
    W(self + 0x12f539u) = 0;
    W(self + 0x12f53bu) = 0;
    W(self + 0x12f4ffu) = 0xffffu;
    B(self + 0x12f4feu) = 0;
    W(self + 0x12f501u) = 0xffffu;
    W(self + 0x12f503u) = 0xffffu;
    return self;
}
