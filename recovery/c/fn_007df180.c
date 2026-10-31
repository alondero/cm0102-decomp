#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007e7760(u32 self, u32 input);

u32 CM_THISCALL cm_fn_007df180(u32 self, u32 input)
{
    cm_fn_007e7760(self, input);
    *(volatile u32 *)(self + 0x17u) = 0xffffffffu;
    *(volatile u32 *)(self + 0x1bu) = 0xffffffffu;
    *(volatile u8 *)(self + 0x1fu) = 0xffu;
    *(volatile u8 *)(self + 0x20u) = 0xffu;
    *(volatile u8 *)(self + 0x21u) = 0xffu;
    *(volatile u32 *)self = 0x0096ec34u;
    *(volatile u8 *)(self + 4u) = 3;
    *(volatile u8 *)(self + 5u) = 0;
    *(volatile u8 *)(self + 6u) = 10;
    *(volatile u8 *)(self + 0x22u) = 0;
    return self;
}
