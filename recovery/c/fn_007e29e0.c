#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007e7760(u32 self, u32 input);

u32 CM_THISCALL cm_fn_007e29e0(u32 self, u32 input)
{
    u32 value;
    cm_fn_007e7760(self, input);
    *(volatile u32 *)self = 0x0096ee34u;
    *(volatile u8 *)(self + 4u) = 2;
    *(volatile u8 *)(self + 5u) = 2;
    *(volatile u8 *)(self + 6u) = 10;
    *(volatile u32 *)(self + 0x17u) = 0xffffffffu;
    value = CM_GLOBAL(volatile u32, 0x009cfa1c);
    *(volatile u8 *)(self + 0x1fu) = 0xffu;
    *(volatile u8 *)(self + 0x20u) = 0xffu;
    *(volatile u32 *)(self + 0x1bu) = value;
    *(volatile u8 *)(self + 0x21u) = 2;
    *(volatile u8 *)(self + 0x22u) = 0;
    return self;
}
