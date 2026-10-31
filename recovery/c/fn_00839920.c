#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

void CM_CDECL cm_fn_00839920(u32 entity_id, s16 requested)
{
    s32 initial_total = (s32)CM_GLOBAL(volatile u32, 0x00ae235cu);
    u32 row = CM_GLOBAL(volatile u32, 0x00ae23b4u);
    u16 found = 0;

    if (initial_total > 0) {
        u32 iteration = 0;
        for (;;) {
            if (cm_fn_0053b900((u32 *)row) != 0)
                break;

            u32 link = *(volatile u32 *)(row + 0x57u);
            if (link != 0 && *(volatile u32 *)link == entity_id) {
                if ((s16)found < requested) {
                    found++;
                } else {
                    u32 index_a = CM_GLOBAL(volatile u32, 0x009cf764u);
                    u32 base_a = CM_GLOBAL(volatile u32, 0x00ae23d0u);
                    u32 assigned = base_a + index_a * 107u;
                    *(volatile u32 *)(row + 0x57u) = assigned;

                    u32 base_b = CM_GLOBAL(volatile u32, 0x00ae23d0u);
                    u32 item = base_b + entity_id * 107u + 4u;
                    (void)cm_fn_00944c9f(0x00de1f64u, 0x00a82578u, item);
                }
            }

            s32 live_total = (s32)CM_GLOBAL(volatile u32, 0x00ae235cu);
            iteration++;
            row += 0x245u;
            if ((s32)iteration >= live_total)
                break;
        }
    }

    u16 threshold = (u16)requested;
    if ((s16)found < (s16)threshold) {
        s32 found_signed = (s16)found;
        s32 requested_signed = (s16)threshold;
        u32 base = CM_GLOBAL(volatile u32, 0x00ae23d0u);
        u32 item = base + entity_id * 107u + 4u;
        (void)cm_fn_00944c9f(0x00de1f64u, 0x00a82530u, item,
                             (u32)found_signed, (u32)requested_signed);
    }

    s32 second_total = (s32)CM_GLOBAL(volatile u32, 0x00ae235cu);
    row = CM_GLOBAL(volatile u32, 0x00ae23b4u);
    u32 second_iteration = 0;
    if (second_total <= 0)
        return;

    if ((s16)found >= requested)
        return;
    for (;;) {
        if (cm_fn_0053b900((u32 *)row) != 0)
            return;

        u32 current = *(volatile u32 *)(row + 0x53u);
        if (current != 0) {
            u32 table_base = CM_GLOBAL(volatile u32, 0x00ae23d0u);
            u32 target = table_base + entity_id * 107u;
            if (current == *(volatile u32 *)(target + 0x5du)) {
                u32 link = *(volatile u32 *)(row + 0x57u);
                if (link == 0) {
                    found++;
                    *(volatile u32 *)(row + 0x57u) = target;
                } else {
                    u32 linked_id = *(volatile u32 *)link;
                    u32 forbidden_a = CM_GLOBAL(volatile u32, 0x009cf764u);
                    if (linked_id == forbidden_a) {
                        found++;
                        *(volatile u32 *)(row + 0x57u) = target;
                    } else {
                        u32 forbidden_b = CM_GLOBAL(volatile u32, 0x009cf6b8u);
                        if (linked_id == forbidden_b) {
                            found++;
                            *(volatile u32 *)(row + 0x57u) = target;
                        } else {
                            s16 link_rank = *(volatile s16 *)(link + 0x69u);
                            s16 target_rank = *(volatile s16 *)(target + 0x69u);
                            if (link_rank < target_rank) {
                                found++;
                                *(volatile u32 *)(row + 0x57u) = target;
                            }
                        }
                    }
                }
            }
        }

        s32 live_total = (s32)CM_GLOBAL(volatile u32, 0x00ae235cu);
        second_iteration++;
        row += 0x245u;
        if ((s32)second_iteration >= live_total)
            return;
        if ((s16)found >= requested)
            return;
    }
}
