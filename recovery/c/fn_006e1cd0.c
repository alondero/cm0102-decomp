#include "cmre.h"

u32 CM_THISCALL cm_fn_006e1cd0(u32 self, u32 value)
{
    u32 address = *(volatile u32 *)(self + 0x23u);
    u32 i;
    for (i = 0; i < 20u; ++i) {
        if (*(volatile u32 *)(address + 0x69u) == value)
            return address;
        address += 0x1beu;
    }
    return 0;
}
