#include "cmre.h"
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);

u32 CM_THISCALL cm_fn_007a8780(u32 self, u32 source)
{
    u32 scratch[2], value;
    u8 byte;
    value = *(volatile u32 *)source;
    *(volatile u32 *)self = value;
    value = *(volatile u32 *)(source + 4);
    *(volatile u32 *)(self + 4) = value;
    cm_fn_00417c10((u32 *)(self + 8), scratch, (u16 *)(source + 8));
    cm_fn_00417c10((u32 *)(self + 0x10), scratch, (u16 *)(source + 0x10));
    byte = *(volatile u8 *)(source + 0x18);
    *(volatile u8 *)(self + 0x18) = byte;
    byte = *(volatile u8 *)(source + 0x19);
    *(volatile u8 *)(self + 0x19) = byte;
    return self;
}
