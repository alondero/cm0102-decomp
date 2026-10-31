#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007af600(u32 self, u32 input);

u32 CM_THISCALL cm_fn_00804540(u32 self, volatile u32 arg)
{
    u32 i;
    for (i = 0; i < 10u; ++i) {
        u32 address = self + i * 688u;
        if (*(volatile u8 *)(address + 11u) == 1)
            return cm_fn_007af600(address + 1u, arg);
    }
    return 0;
}
