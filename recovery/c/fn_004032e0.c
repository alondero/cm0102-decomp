#include "cmre.h"

u32 CM_THISCALL cm_fn_004032e0(u32 self)
{
    u32 flag = *(volatile u32 *)(self + 0xbedu);
    u32 index, value, base;
    if (flag == 0)
        return 0;
    index = (u32)(s32)*(volatile s16 *)(self + 0xb76u);
    value = (u32)(s32)*(volatile s16 *)(self + index * 2u + 0x212u);
    base = *(volatile u32 *)(self + 0x208u);
    return *(volatile u32 *)(base + value * 396u + 0x24u);
}
