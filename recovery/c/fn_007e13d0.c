#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007e7760(u32, u32);

u32 CM_THISCALL cm_fn_007e13d0(u32 self, u32 input)
{
    u32 global;
    cm_fn_007e7760(self, input);
    D(self) = 0x0096ed74;
    B(self + 4) = 2;
    B(self + 5) = 0x23;
    B(self + 6) = 0x0a;
    D(self + 0x17) = 0xffffffffu;
    global = D(0x009cfa18);
    B(self + 0x1f) = 0xff;
    B(self + 0x20) = 0xff;
    D(self + 0x1b) = global;
    B(self + 0x21) = 0x23;
    B(self + 0x22) = 0;
    return self;
}

