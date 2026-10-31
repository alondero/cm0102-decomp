#include "cmre.h"

u32 CM_THISCALL cm_fn_006bba10(u32 self)
{
    *(volatile u16 *)(self + 0x475fu) = 0xffffu;
    *(volatile u32 *)(self + 0x4761u) = 0;
    *(volatile u32 *)(self + 0x4765u) = 0;
    *(volatile u8 *)(self + 0x4769u) = 0;
    *(volatile u8 *)(self + 0x475au) = 0;
    *(volatile u8 *)(self + 0x475bu) = 0xffu;
    *(volatile u8 *)(self + 0x475cu) = 0xffu;
    *(volatile u8 *)(self + 0x475du) = 0xffu;
    *(volatile u8 *)(self + 0x475eu) = 0xffu;
    return 0xffffffffu;
}
