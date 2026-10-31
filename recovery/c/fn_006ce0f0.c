#include "cmre.h"

u32 CM_THISCALL cm_fn_006ce0f0(u32 self, u32 selector)
{
    if ((u8)selector == 0)
        *(volatile u32 *)(self + 0xf59e) = 0;
    else
        *(volatile u32 *)(self + 0xf5a2) = 0;
    return 0;
}
