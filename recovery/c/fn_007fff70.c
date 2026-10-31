#include "cmre.h"

u32 CM_THISCALL cm_fn_007fff70(u32 self)
{
    u32 count = 0, i;
    for (i = 0; i < 16u; ++i) {
        s16 value = *(volatile s16 *)(self + 0x380au + i * 2u);
        if (value <= 0 || *(volatile u32 *)(self + 0x7eeu + i * 0x300u) != 0)
            ++count;
    }
    return count;
}
