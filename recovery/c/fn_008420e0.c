#include "cmre.h"

extern u32 CM_CDECL cm_fn_00946464(u32 *output, u32 base, u32 count,
                                    s32 element_size, u32 sentinel);
extern u32 CM_THISCALL cm_fn_0068d0d0(u32 *self, u32 record, u32 size);

void CM_THISCALL cm_fn_008420e0(u8 *self, u32 *key_pointer, u32 receiver)
{
    volatile u32 output[2];
    u32 key = *(volatile u32 *)key_pointer;
    output[1] = key;
    u32 count = *(volatile u32 *)(self + 0x14u);
    u32 base = *(volatile u32 *)(self + 4u);
    output[0] = 0xffffffffu;

    u32 record = cm_fn_00946464((u32 *)&output[0], base, count, 8, 0x00844740u);
    if (record == 0)
        return;

    base = *(volatile u32 *)(self + 4u);
    count = *(volatile u32 *)(self + 0x14u);
    u32 last = base + count * 8u - 8u;
    if (record > base) {
        key = *(volatile u32 *)key_pointer;
        for (;;) {
            u32 previous_key = *(volatile u32 *)(record - 4u);
            record -= 8u;
            if (previous_key != key) {
                record += 8u;
                break;
            }
            if (record <= base)
                break;
        }
    }

    u32 record_key = *(volatile u32 *)(record + 4u);
    key = *(volatile u32 *)key_pointer;
    if (record_key != key)
        return;

    for (;;) {
        (void)cm_fn_0068d0d0((u32 *)receiver, record, 8u);
        if (record == last)
            return;
        u32 next_key = *(volatile u32 *)(record + 0x0cu);
        key = *(volatile u32 *)key_pointer;
        record += 8u;
        if (next_key != key)
            return;
    }
}
