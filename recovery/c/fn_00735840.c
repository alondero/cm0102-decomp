#include "cmre.h"

u8 CM_THISCALL cm_fn_00735840(u32 self, u32 key_pointer)
{
    s32 count = *(volatile s32 *)(self + 8u);
    u32 key, cursor, index;
    if (count <= 0) return 0;
    key = *(volatile u32 *)key_pointer;
    cursor = *(volatile u32 *)self + 8u;
    for (index = 0; (s32)index < count; ++index, cursor += 0x17u)
        if (*(volatile u32 *)cursor == key) return 1;
    return 0;
}
