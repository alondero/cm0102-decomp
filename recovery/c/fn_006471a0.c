/* Faithful C recovery at preferred-image VA 006471a0. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_00682b30(void *self, u32 arg);
extern void cm_fn_00945333(u32 base, u32 count, u32 stride, void *compare);

void CM_THISCALL cm_fn_006471a0(void *self, u16 key)
{
    u32 base = (u32)self;
    u32 records = *(volatile u32 *)(base + 0xb1u);
    cm_fn_00682b30(self, records);

    s16 count = *(volatile s16 *)(base + 0x3eu);
    u32 match = 0;
    u32 matches = 0;
    if (count > 0) {
        u32 record = *(volatile u32 *)(base + 0xb1u);
        s32 remaining = count;
        do {
            u16 value = *(volatile u16 *)(record + 0x19u);
            if (value == key) {
                if (match == 0)
                    match = record;
                matches += 1u;
            }
            record += 0x3bu;
            --remaining;
        } while (remaining != 0);
    }
    cm_fn_00945333(match, (u32)(s32)(s16)matches, 0x3bu,
                   (void *)0x00683410u);

    s16 i = 0;
    if (*(volatile s16 *)(base + 0x3eu) > 0) {
        do {
            u32 record = *(volatile u32 *)(base + 0xb1u);
            u32 offset = (u32)(s32)i * 0x3bu;
            *(volatile u8 *)(record + offset + 4u) = (u8)i;
            i = (s16)(i + 1);
        } while (i < *(volatile s16 *)(base + 0x3eu));
    }
}
