#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

void CM_FASTCALL cm_fn_007fef10(volatile u8 *self)
{
    s32 index = *(volatile s16 *)(self + 0x382a);
    volatile u8 *slot_base = self + index * 0x300;
    volatile u32 *item = (volatile u32 *)(slot_base + 0x7da);
    u32 node = *item;

    while (node != 0) {
        u32 idx;
        volatile u32 *pair = (volatile u32 *)(node + 0x14u);
        *(volatile u32 *)(slot_base + 0x7e2) = *(volatile u32 *)(node + 0x1f8u);
        for (s32 count = 0x3c; count != 0; --count, pair += 2) {
            u32 first = pair[0];
            if (first != 0 && pair[1] != 0)
                (void)cm_fn_00944c94((void *)first);
        }
        (void)cm_fn_00944c94((void *)node);
        idx = (s32)*(volatile s16 *)(self + 0x382a);
        --*(volatile s16 *)(self + idx * 0x300 + 0x7e8);
        idx = (s32)*(volatile s16 *)(self + 0x382a);
        slot_base = self + idx * 0x300;
        node = *(volatile u32 *)(slot_base + 0x7e2);
    }

    index = *(volatile s16 *)(self + 0x382a);
    *(volatile u32 *)(self + index * 0x300 + 0x7da) = 0;
    index = *(volatile s16 *)(self + 0x382a);
    *(volatile u32 *)(self + index * 0x300 + 0x7de) = 0;
    index = *(volatile s16 *)(self + 0x382a);
    *(volatile u32 *)(self + index * 0x300 + 0x7e2) = 0;
    index = *(volatile s16 *)(self + 0x382a);
    *(volatile u16 *)(self + index * 0x300 + 0x7e8) = 0;
    index = *(volatile s16 *)(self + 0x382a);
    *(volatile u16 *)(self + index * 0x300 + 0x7e6) = 0;
}
