#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007e7760(u32, u32);
u32 CM_THISCALL cm_fn_007dbca0(u32 self, u32 input)
{
    u32 value;
    cm_fn_007e7760(self, input);
    D(self) = 0x0096ea74;
    B(self + 4) = 0;
    B(self + 5) = 4;
    B(self + 6) = 5;
    value = D(0x009cf244);
    D(self + 0x17) = value;
    value = D(0x009cfa10);
    D(self + 0x1b) = value;
    B(self + 0x20) = 5;
    B(self + 0x1f) = 4;
    B(self + 0x21) = 1;
    B(self + 0x22) = 0;
    return self;
}
