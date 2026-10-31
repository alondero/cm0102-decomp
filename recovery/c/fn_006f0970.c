#include "cmre.h"

u32 CM_THISCALL cm_fn_006f0970(u32 self)
{
    u32 object = *(volatile u32 *)(self + 0x19e);
    *(volatile u32 *)(object + 0x4786) = 0;
    object = *(volatile u32 *)(self + 0x19e);
    *(volatile u8 *)(object + 0x8eae) = 0xff;
    object = *(volatile u32 *)(self + 0x19e);
    *(volatile u32 *)(object + 0xf582) = 0;
    object = *(volatile u32 *)(self + 0x19e);
    *(volatile u8 *)(object + 0x1a7) = 0;
    *(volatile u16 *)(self + 0x29) = 0;
    *(volatile u16 *)(self + 0x19c) = 0;
    return 0;
}
