#include "cmre.h"

u8 CM_THISCALL cm_fn_007ae350(u32 self, u32 key_ptr)
{
    s32 count;
    u32 key, cursor;
    s32 index;
    if (key_ptr == 0) return 0;
    count = *(volatile s32 *)(self + 8u);
    if (count <= 0) return 0;
    key = *(volatile u32 *)key_ptr;
    cursor = *(volatile u32 *)(self + 4u);
    for (index = 0; index < count; ++index, cursor += 0x1au)
        if (*(volatile u32 *)cursor == key) return 1;
    return 0;
}
