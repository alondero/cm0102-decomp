#include "cmre.h"

u32 CM_STDCALL cm_fn_0063c330(u32 unused, u32 destination)
{
    u32 i;
    (void)unused;
    for (i = 0; i < 10u; ++i)
        *(volatile u32 *)(destination + i * 4u) = 0xffffffffu;
    *(volatile u16 *)(destination + 40u) = 0xffffu;
    return 0xffffffffu;
}
