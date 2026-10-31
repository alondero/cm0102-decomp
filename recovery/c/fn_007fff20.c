#include "cmre.h"

u32 CM_THISCALL cm_fn_007fff20(u32 self, u32 arg)
{
    u32 i;
    for (i = 0; i < 16u; ++i) {
        s16 value = *(volatile s16 *)(self + i * 2u + 0x380au);
        if (value > 0) {
            u32 address = self + i * 0x300u;
            if (*(volatile u16 *)(address + 0x7d4u) == (u16)arg &&
                *(volatile u32 *)(address + 0x7f2u) == 0)
                return 0;
        }
    }
    return 1;
}
