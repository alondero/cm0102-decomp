#include "cmre.h"

extern u32 CM_CDECL cm_fn_00945ce6(u32 destination, u32 element_size,
    u32 count, u32 context);

void CM_THISCALL cm_fn_00914410(u8 *self, u32 context)
{
    u32 count_address = (u32)self + 0xf3u;
    (void)cm_fn_00945ce6((u32)self + 0xeeu, 1u, 1u, context);
    (void)cm_fn_00945ce6(count_address, 4u, 1u, context);

    u32 index = 0;
    s32 count = *(volatile s32 *)count_address;
    while ((s32)index < count) {
        u32 array = *(volatile u32 *)(self + 0xefu);
        u32 cell = array + (u32)index * 4u;
        u32 entry = *(volatile u32 *)cell;
        u32 value = entry != 0 ? *(volatile u32 *)entry : 0xffffffffu;
        *(volatile u32 *)cell = value;
        count = *(volatile s32 *)count_address;
        ++index;
    }

    u32 allocation_count = (u32)*(volatile s32 *)count_address;
    u32 array = *(volatile u32 *)(self + 0xefu);
    (void)cm_fn_00945ce6(array, 4u, allocation_count, context);

    index = 0;
    count = *(volatile s32 *)count_address;
    while ((s32)index < count) {
        array = *(volatile u32 *)(self + 0xefu);
        u32 cell = array + (u32)index * 4u;
        s32 value = *(volatile s32 *)cell;
        if (value == -1) {
            *(volatile u32 *)cell = 0;
        } else {
            u32 base = CM_GLOBAL(volatile u32, 0x00ae23b4u);
            u32 scaled = (u32)value * 0x245u;
            *(volatile u32 *)cell = base + scaled;
        }
        count = *(volatile s32 *)count_address;
        ++index;
    }
}
