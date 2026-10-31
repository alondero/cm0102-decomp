#include "cmre.h"

extern void CM_CDECL cm_fn_00945333(u32 *slots, u32 signed_count,
                                    u32 element_size, void *sentinel);

void CM_CDECL cm_fn_00596050(void)
{
    u32 base_index = CM_GLOBAL(volatile u32, 0x009cf7ec);
    u32 list_base = CM_GLOBAL(volatile u32, 0x00ae23d0);
    u32 base_offset = ((base_index * 3u) * 9u) << 2;
    base_offset -= base_index;
    u32 second_index = CM_GLOBAL(volatile u32, 0x009cf7f0);
    u32 second_offset = ((second_index * 3u) * 9u) << 2;
    second_offset -= second_index;
    u32 second_target = second_offset + list_base;
    u32 first_target = base_offset + list_base;
    u32 current = CM_GLOBAL(volatile u32, 0x00ae23b4);
    u8 stored_count = 0;
    u16 row = 0;
    u32 slots[20];
    s32 bound = CM_GLOBAL(volatile s32, 0x00ae235c);

    if (bound > 0) {
        do {
            if (stored_count >= 20)
                break;
            u32 candidate = *(volatile u32 *)(current + 0x57u);
            if (candidate != 0) {
                u32 key = *(volatile u32 *)candidate;
                u32 wanted = CM_GLOBAL(volatile u32, 0x009cf7f8);
                if (key == wanted) {
                    s32 slot_index = (s8)stored_count;
                    stored_count++;
                    slots[slot_index] = current;
                }
            }
            bound = CM_GLOBAL(volatile s32, 0x00ae235c);
            row++;
            s32 compare_row = (s16)row;
            current += 0x245u;
            if (compare_row >= bound)
                break;
        } while (1);
    }

    s32 count_arg = (s8)stored_count;
    cm_fn_00945333(slots, (u32)count_arg, 4, (void *)0x0053fd40u);

    s32 i = 0;
    s32 signed_count = (s8)stored_count;
    if (signed_count > 0) {
        do {
            u32 object = slots[i];
            u32 target = (i < 10) ? second_target : first_target;
            *(volatile u32 *)(object + 0x60u) = target;
            i++;
        } while (i < signed_count);
    }
}
