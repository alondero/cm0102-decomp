#include "cmre.h"

u32 CM_THISCALL cm_fn_0086fc20(u32 self, u32 index_ptr)
{
    u32 index = *(volatile u32 *)index_ptr;
    u32 base = *(volatile u32 *)(self + 8);
    u32 record = base + index * 210;
    u32 i;
    for (i = 0; i < 0x34; ++i)
        *(volatile u32 *)(record + i * 4) = 0;
    *(volatile u16 *)(record + 0xd0) = 0;
    *(volatile u32 *)(record + 0x86) = 0xffffffff;
    *(volatile u32 *)(record + 0xc2) = 0xffffffff;
    *(volatile u32 *)(record + 0xc6) = 0xffffffff;
    *(volatile u32 *)(record + 0x4a) = 0xffffffff;
    *(volatile u32 *)(record + 0x5e) = 0xffffffff;
    *(volatile u32 *)(record + 0x72) = 0xffffffff;
    return 0xffffffff;
}
