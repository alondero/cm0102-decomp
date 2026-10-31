#include "cmre.h"

u8 CM_STDCALL cm_fn_006cb650(s8 value)
{
    u32 index = (u32)((s32)value - 0x25);
    u8 selector;
    u32 target;
    if (index > 0x3cu) return 0;
    selector = *(volatile u8 *)(0x006cb680u + index);
    target = *(volatile u32 *)(0x006cb678u + (u32)selector * 4u);
    return target == 0x006cb66cu;
}
