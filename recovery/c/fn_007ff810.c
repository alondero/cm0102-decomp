#include "cmre.h"

u32 CM_THISCALL cm_fn_007ff810(void *self, u32 key1, u32 key2)
{
    volatile u8 *base = (volatile u8 *)self;
    s16 group = *(volatile s16 *)(base + 0x382a);
    u32 group_offset = (u32)(s32)group;
    s16 count = *(volatile s16 *)(base + 0x380a + group_offset * 2u);
    s16 found1 = -1;
    s16 found2 = -1;

    if (count <= 0)
        return 0;

    for (s32 i = 0; i < count; ++i) {
        u32 record_offset = 0x7d6u + group_offset * 0x300u + (u32)i * 0x30u;
        u32 value = *(volatile u32 *)(base + record_offset);
        if (value == key1)
            found1 = (s16)i;
        if (value == key2)
            found2 = (s16)i;
    }

    if (found1 < 0 || found2 < 0)
        return 0;

    {
        u32 offset1 = 0x7d6u + (group_offset * 0x10u + (u32)(s32)found1) * 0x30u;
        u32 first_value = *(volatile u32 *)(base + offset1);
        u32 offset2 = 0x7d6u + (group_offset * 0x10u + (u32)(s32)found2) * 0x30u;
        u32 second_value = *(volatile u32 *)(base + offset2);
        *(volatile u32 *)(base + offset1) = second_value;
        group = *(volatile s16 *)(base + 0x382a);
        group_offset = (u32)(s32)group;
        offset2 = 0x7d6u + (group_offset * 0x10u + (u32)(s32)found2) * 0x30u;
        *(volatile u32 *)(base + offset2) = first_value;
    }
    return 1;
}
