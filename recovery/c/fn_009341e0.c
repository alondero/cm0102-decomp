#include "cmre.h"

u32 CM_CDECL cm_fn_009341e0(u32 unused, u32 object, u32 selector, u32 source, u32 destination)
{
    u32 base = *(volatile u32 *)(object + ((u16)selector ? 0x18 : 0x1c));
    u32 address = base + (u32)(s32)(s16)destination * 4u;
    s32 value = *(volatile s32 *)(base + (u32)(s32)(s16)source * 4u);
    s32 current = *(volatile s32 *)address;
    (void)unused;
    if (current < value)
        *(volatile s32 *)address = value;
    return address;
}
