#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007e7760(u32 self, u32 input);

u32 CM_THISCALL cm_fn_007db8f0(u32 self, u32 input)
{
    u32 value;
    cm_fn_007e7760(self, input);
    *(volatile u32 *)self = 0x0096ea54;
    *(volatile u8 *)(self + 4) = 1;
    *(volatile u8 *)(self + 5) = 5;
    *(volatile u8 *)(self + 6) = 5;
    value = CM_GLOBAL(volatile u32, 0x009cf244);
    *(volatile u32 *)(self + 0x17) = value;
    value = CM_GLOBAL(volatile u32, 0x009cfa10);
    *(volatile u8 *)(self + 0x20) = 5;
    *(volatile u32 *)(self + 0x1b) = value;
    *(volatile u8 *)(self + 0x1f) = 4;
    *(volatile u8 *)(self + 0x21) = 1;
    *(volatile u8 *)(self + 0x22) = 0;
    return self;
}
