#include "cmre.h"

u32 CM_THISCALL cm_fn_008afd90(u32 self)
{
    u32 address = self + 0x52u;
    u32 i;
    for (i = 0; i < 20u; ++i) {
        *(volatile u32 *)(address - 4u) = 0xffffffffu;
        *(volatile u32 *)address = 0xffffffffu;
        *(volatile u32 *)(address + 4u) = 0xffffffffu;
        *(volatile u32 *)(address + 8u) = 0xffffffffu;
        *(volatile u32 *)(address + 12u) = 0xffffffffu;
        *(volatile u32 *)(address + 17u) = 0;
        address += 25u;
    }
    *(volatile u8 *)(self + 0x252u) = 0;
    return address;
}
