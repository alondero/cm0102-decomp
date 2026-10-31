#include "cmre.h"

u32 CM_THISCALL cm_fn_00735780(u32 self, u32 key_pointer)
{
    s32 count = *(volatile s32 *)(self + 8u);
    u32 key, address, i;
    if (count <= 0)
        return 0;
    key = *(volatile u32 *)key_pointer;
    address = *(volatile u32 *)self + 0x10u;
    for (i = 0; i < (u32)count; ++i) {
        if (*(volatile u32 *)(address - 8u) == key &&
            *(volatile u8 *)address == 2)
            return 1;
        address += 0x17u;
    }
    return 0;
}
