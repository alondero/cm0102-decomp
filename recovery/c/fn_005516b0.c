#include "cmre.h"
u32 CM_THISCALL cm_fn_005516b0(u8 *self)
{
    u32 table = *(volatile u32 *)self;
    u32 result;
    *(volatile u32 *)(self + 0x28) = 0xc3500;
    result = ((u32 (CM_THISCALL *)(u8 *, u32, void *, void *, u32))
              *(volatile u32 *)(table + 0x3c))(self, 0xffffffffu, self + 0x3c, self + 0x3a, 0);
    *(volatile u32 *)(self + 0xa3) = result;
    *(volatile u8 *)(self + 0x49) = 4;
    *(volatile u8 *)(self + 0x4a) = 3;
    return 1;
}
