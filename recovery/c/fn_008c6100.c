#include "cmre.h"

extern int CM_THISCALL cm_fn_004cf5a0(void *receiver, u32 key);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(void *self, const u8 *source, s16 count);

u32 CM_THISCALL cm_fn_008c6100(volatile u8 *self, u32 target, u32 destination)
{
    volatile u32 index;
    u32 matches;
    u32 current_index = 0u;
    s32 initial_count;

    matches = 0u;
    initial_count = *(volatile s32 *)(self + 4u);
    index = 0u;
    if (initial_count <= 0)
        return matches;

    for (;;) {
        u32 array = *(volatile u32 *)self;
        u32 entry_address = array + current_index * 4u;
        u32 entry = *(volatile u32 *)entry_address;

        if (entry != 0u) {
            u8 type = *(volatile u8 *)(entry + 0x2cu);
            u8 skip = 0u;

            if (type == 2u || type == 3u || type == 0x14u || type == 0x10u || type == 4u)
                skip = 1u;
            else if (type == 0x0fu && *(volatile u8 *)(entry + 0x2eu) >= 3u)
                skip = 1u;

            if (skip == 0u) {
                u32 id = *(volatile u32 *)(entry + 0x0cu);
                u32 base = CM_GLOBAL(volatile u32, 0x00ae23bc);
                u32 key = base + id * 110u;
                void *receiver = (void *)CM_GLOBAL(volatile u32, 0x00ae1050);
                int found = cm_fn_004cf5a0(receiver, key);
                u32 mapped = (u32)found;
                CM_GLOBAL(volatile u32, 0x00ae1080) = mapped;

                if (mapped != 0u &&
                    ((*(volatile u8 *)(mapped + 0x35u) & 0x3fu) != 5u)) {
                    u32 mapped_id = *(volatile u32 *)(mapped + 4u);
                    u32 mapped_base = CM_GLOBAL(volatile u32, 0x00ae23b4);
                    mapped = mapped_base + mapped_id * 581u;
                } else {
                    mapped = 0u;
                }

                if (mapped == target && destination != 0u)
                    (void)cm_fn_0068d0d0((void *)destination,
                                         (const u8 *)&index, 4);
                if (mapped == target)
                    ++matches;
            }
        }

        {
            u32 next_index = index;
            s32 current_count = *(volatile s32 *)(self + 4u);
            ++next_index;
            int continue_loop = (s32)next_index < current_count;
            index = next_index;
            current_index = next_index;
            if (!continue_loop)
                break;
        }
    }

    return matches;
}
