#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00456770(u8 *self, u32 first, u32 second);
extern void CM_THISCALL cm_fn_00777590(u8 *destination, u8 *source);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);

void CM_THISCALL cm_fn_007774b0(u8 *self)
{
    s32 first_global = CM_GLOBAL(volatile s32, 0x00ae2350);
    s32 second_global = CM_GLOBAL(volatile s32, 0x00ae235c);
    s32 remaining = (s32)((u32)second_global - (u32)first_global * 2u);
    u32 rows = *(volatile u32 *)(self + 0x20u);
    u8 *row = (u8 *)rows;

    if (remaining <= 0)
        return;

    do {
        u8 entry_count = *(volatile u8 *)(row + 5u);
        s8 attempt = 0;
        s8 successes = 0;

        if (attempt < (s8)entry_count) {
            do {
            u32 arg2 = CM_GLOBAL(volatile u32, 0x00ae2c94);
            u32 arg1 = CM_GLOBAL(volatile u32, 0x00ae2c90);
            u32 row_data = *(volatile u32 *)row;
            u32 slot_delta = (u32)(s32)attempt * 9u;
            u32 result = cm_fn_00456770((u8 *)(row_data + slot_delta), arg1, arg2);
            if (result != 0u)
                successes = (s8)((u8)successes + 1u);
            entry_count = *(volatile u8 *)(row + 5u);
            attempt = (s8)((u8)attempt + 1u);
            } while (attempt < (s8)entry_count);
        }

        attempt = successes;
        while (attempt < (s8)*(volatile u8 *)(row + 5u)) {
            u32 row_data = *(volatile u32 *)row;
            s32 source_index = (s32)attempt;
            s32 destination_index = source_index - (s32)successes;
            cm_fn_00777590((u8 *)(row_data + (u32)destination_index * 9u),
                           (u8 *)(row_data + (u32)source_index * 9u));
            attempt = (s8)((u8)attempt + 1u);
        }

        {
            u8 current_count = *(volatile u8 *)(row + 5u);
            u8 new_count = (u8)(current_count - (u8)successes);
            *(volatile u8 *)(row + 5u) = new_count;
            if (new_count == 0u) {
                u32 list = *(volatile u32 *)row;
                if (list != 0u) {
                    (void)cm_fn_009452ca(list);
                    *(volatile u8 *)(row + 4u) = 0u;
                    *(volatile u32 *)row = 0u;
                }
            }
        }

        row += 6u;
        --remaining;
    } while (remaining != 0);
}
