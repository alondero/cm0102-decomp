#include "cmre.h"

extern void CM_CDECL cm_fn_00945333(u32 *base, u32 count, u32 item_size,
                                     void *compare);

u32 CM_THISCALL cm_fn_00768130(u32 self, u32 target, u32 mode_arg)
{
    u32 count = *(volatile u32 *)(self + 4u);
    u32 base = *(volatile u32 *)self;
    u32 match_count = 0;

    cm_fn_00945333((u32 *)base, count, 0xcu, (void *)0x00768200u);
    /* qsort operates on the object-owned array; reload both fields afterwards. */
    base = *(volatile u32 *)self;
    count = *(volatile u32 *)(self + 4u);
    s8 mode = (s8)(u8)mode_arg;
    u32 table = CM_GLOBAL(volatile u32, 0x00ae23bcu);
    u32 target_id = target;

    s8 threshold = 3;
    do {
        u32 record = base;
        for (s32 i = 0; i < (s32)count; ++i, record += 0xcu) {
            if (target_id != 0xffffffffu && *(volatile u32 *)(record + 4u) != target_id)
                continue;
            if (*(volatile s8 *)(record + 0xau) < threshold)
                continue;

            if ((u8)mode != 0xffu) {
                s8 value = *(volatile s8 *)(record + 0xbu);
                if (value > mode) {
                    match_count++;
                    continue;
                }
                if ((s32)match_count < 3)
                    continue;
            }

            u32 id = *(volatile u32 *)record;
            if (target_id == 0xffffffffu)
                return record;

            u32 offset = id * 110u + 0x39u;
            u32 link = *(volatile u32 *)(table + offset);
            if (link == 0)
                continue;
            u32 next = *(volatile u32 *)(link + 0x57u);
            if (next == 0)
                continue;
            if (*(volatile u32 *)next == target_id)
                return record;
        }
        threshold--;
    } while (threshold >= 0);

    return 0;
}
