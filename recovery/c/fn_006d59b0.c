#include "cmre.h"

extern u8 CM_THISCALL cm_fn_00714d30(volatile u8 *self);
extern u32 CM_THISCALL cm_fn_007160f0(u32 self);

u8 CM_THISCALL cm_fn_006d59b0(u32 self, volatile u8 *record)
{
    u32 cursor = self + 0x4796u;
    u8 count = 0;
    u32 i;
    if (cm_fn_00714d30(record) == 0)
        cursor += 0x22d8u;
    for (i = 0; i != 20; ++i) {
        if (*(volatile s8 *)(cursor + 0x19) >= 0 &&
            cm_fn_007160f0(cursor) == (u32)record)
            ++count;
        cursor += 0x1beu;
    }
    return count;
}
