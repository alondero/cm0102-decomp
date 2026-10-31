#include "cmre.h"

u32 CM_THISCALL cm_fn_005face0(u32 self)
{
    u32 address = *(volatile u32 *)self;
    s32 count = *(volatile s16 *)(self + 4u);
    u32 index = 0;
    if (count <= 0)
        return address;
    do {
        u32 value = *(volatile u32 *)address;
        u32 reference = CM_GLOBAL(volatile u32, 0x009cf7cc);
        if (value == reference) {
            u32 replacement = CM_GLOBAL(volatile u32, 0x009cf224);
            u32 current = *(volatile u32 *)(address + 6u);
            if (current == replacement)
                replacement = CM_GLOBAL(volatile u32, 0x009cf3f4);
            *(volatile u32 *)(address + 6u) = replacement;
        }
        count = *(volatile s16 *)(self + 4u);
        ++index;
        address += 0x22u;
    } while ((s32)index < count);
    return address;
}
