#include "cmre.h"

u16 CM_THISCALL cm_fn_007fffa0(u32 self, u32 key)
{
    s32 outer;
    if (key == 0) return 0xffffu;
    for (outer = 0; outer < 16; ++outer) {
        s16 count = *(volatile s16 *)(self + 0x380au + (u32)outer * 2u);
        s32 inner;
        for (inner = 0; inner < count; ++inner) {
            u32 address = self + 0x7d6u + ((u32)outer * 16u + (u32)inner) * 48u;
            if (*(volatile u32 *)address == key) return (u16)outer;
        }
    }
    return 0xffffu;
}
