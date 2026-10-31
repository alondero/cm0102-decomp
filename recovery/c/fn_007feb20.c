#include "cmre.h"

u32 CM_THISCALL cm_fn_007feb20(u32 self, volatile u32 a, volatile u32 b,
                                volatile u32 c, volatile u32 d, volatile u32 e,
                                volatile u32 f, volatile u32 g)
{
    u32 eax = a, edx = b;
    *(volatile u32 *)(self + 0x37d4u) = eax;
    eax = c;
    *(volatile u32 *)(self + 0x37d8u) = edx;
    edx = d;
    *(volatile u32 *)(self + 0x37dcu) = eax;
    eax = e;
    *(volatile u32 *)(self + 0x37e0u) = edx;
    edx = f;
    *(volatile u32 *)(self + 0x37e4u) = eax;
    eax = (eax & 0xffff0000u) | (u16)g;
    *(volatile u32 *)(self + 0x37e8u) = edx;
    *(volatile u16 *)(self + 0x37ecu) = (u16)eax;
    return eax;
}
