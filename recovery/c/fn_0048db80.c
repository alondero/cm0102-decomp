#include "cmre.h"

u8 CM_THISCALL cm_fn_0048db80(volatile u8 *self, volatile u32 *argument)
{
    u32 upper_global;
    u32 lower_global;
    u32 difference_bits;
    u32 argument_value;
    u32 target;
    u32 other;
    s32 limit;
    u32 outer_bits;
    volatile u8 *type_object;
    volatile u8 *other_object;
    s16 type_count;
    s16 inner;

    upper_global = CM_GLOBAL(volatile u32, 0x00ae235c);
    lower_global = CM_GLOBAL(volatile u32, 0x00ae2350);
    difference_bits = upper_global - lower_global;
    argument_value = *argument;
    if ((s32)argument_value >= (s32)difference_bits) {
        u32 current = *(volatile u32 *)(self + 4);
        target = *(volatile u32 *)(current + 0x59);
        if (target != 0) {
            other = *(volatile u32 *)((volatile u8 *)argument + 0x53);
            if (other != 0 && target == *(volatile u32 *)(other + 0x71))
                goto search;
            if (target != 0)
                return 0;
        }
    } else {
        other = *(volatile u32 *)((volatile u8 *)argument + 0x53);
        if (other != 0) {
            u32 current = *(volatile u32 *)(self + 4);
            if (other == *(volatile u32 *)(current + 0x5d)) {
                u32 owner = *(volatile u32 *)((volatile u8 *)argument + 0x57);
                if (owner != 0 && owner == current)
                    return 1;
            }
            if (other != 0) {
                u32 current_again = *(volatile u32 *)(self + 4);
                u32 candidate = *(volatile u32 *)(other + 0x71);
                if (candidate != *(volatile u32 *)(current_again + 0x59))
                    return 0;
            }
        }
    }

search:
    limit = *(volatile s32 *)(self + 0x30);
    if (limit < -1)
        return 0;
    outer_bits = 0xffffffffu;
    type_object = (volatile u8 *)0;
    other_object = (volatile u8 *)0;
    do {
        volatile u8 *candidate;
        if (outer_bits == 0xffffffffu) {
            candidate = self;
        } else {
            u32 table = *(volatile u32 *)(self + 0x0c);
            candidate = (volatile u8 *)*(volatile u32 *)(table + outer_bits * 4u);
        }
        if (*(volatile u8 *)(candidate + 0x43) == 2) {
            type_object = candidate;
        } else {
            other_object = candidate;
        }

        if (type_object != (volatile u8 *)0) {
            type_count = *(volatile s16 *)(type_object + 0x3e);
            if (type_count > 0) {
                u32 entries = *(volatile u32 *)(type_object + 0xb1);
                inner = 0;
                do {
                    u32 entry = entries + (s32)inner * 0x3b;
                    if (*(volatile u32 *)entry == (u32)argument)
                        return 1;
                    ++inner;
                } while (inner < type_count);
            }
        } else {
            type_count = *(volatile s16 *)(other_object + 0x3e);
            if (type_count > 0) {
                u32 entries = *(volatile u32 *)(other_object + 0xa7);
                inner = 0;
                do {
                    if (*(volatile u32 *)(entries + (s32)inner * 6) ==
                        (u32)argument)
                        return 1;
                    ++inner;
                } while (inner < type_count);
            }
        }
        outer_bits += 1u;
    } while ((s32)outer_bits <= limit);
    return 0;
}
