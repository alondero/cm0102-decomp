#include "cmre.h"
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);

u32 CM_THISCALL cm_fn_008cb940(u32 self, u32 source)
{
    u32 scratch[2];
    u32 i, value;
    u8 byte;
    for (i = 0; i < 7; ++i) {
        value = *(volatile u32 *)(source + i * 4);
        *(volatile u32 *)(self + i * 4) = value;
    }
    byte = *(volatile u8 *)(source + 0x1c);
    *(volatile u8 *)(self + 0x1c) = byte;
    cm_fn_00417c10((u32 *)(self + 0x1d), scratch, (u16 *)(source + 0x1d));
    return self;
}
