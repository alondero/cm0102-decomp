#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007e7760(u32 self, u32 input);

u32 CM_THISCALL cm_fn_007e40f0(u32 self, u32 input)
{
    u32 value;
    cm_fn_007e7760(self, input);
    *(volatile u32 *)self = 0x0096ef14;
    *(volatile u8 *)(self + 4) = 0;
    *(volatile u8 *)(self + 5) = 0x1e;
    *(volatile u8 *)(self + 6) = 10;
    value = CM_GLOBAL(volatile u32, 0x009cf4f8);
    *(volatile u32 *)(self + 0x17) = value;
    value = CM_GLOBAL(volatile u32, 0x009cfa14);
    *(volatile u8 *)(self + 0x1f) = 0x1e;
    *(volatile u8 *)(self + 0x20) = 0xff;
    *(volatile u8 *)(self + 0x21) = 0xff;
    *(volatile u32 *)(self + 0x1b) = value;
    *(volatile u8 *)(self + 0x22) = 0;
    return self;
}
