#include "cmre.h"

u8 CM_THISCALL cm_fn_006bba50(u32 self)
{
    u32 object;
    if (*(volatile u8 *)(self + 0x475au) == 0) return 0xffu;
    object = *(volatile u32 *)(self + 0x4761u);
    if (object == 0) return 0xffu;
    return *(volatile u8 *)(object + 0x27u) == 0;
}
