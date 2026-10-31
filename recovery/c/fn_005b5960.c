#include "cmre.h"

u32 CM_THISCALL cm_fn_005b5960(u32 self)
{
    u32 i;
    for (i = 0; i < 11u; ++i) {
        u32 address = self + 0x5b4u + i;
        u8 value = *(volatile u8 *)address;
        if ((value & 4u) != 0 && (value & 8u) != 0)
            *(volatile u8 *)address = 2;
        *(volatile u32 *)(self + 0x5bfu + i * 4u) = 0xffffffffu;
    }
    return 1;
}
