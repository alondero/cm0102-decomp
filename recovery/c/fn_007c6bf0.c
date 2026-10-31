/* Preferred-image VA 0x007C6BF0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Fields retain their observed offsets. Reload the base/count at each original
   read: record movement and final stores may alias those fields. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00946464(void *record, u32 base, u32 count,
                                 int size, void *compare);

void CM_THISCALL cm_fn_007c6bf0(u8 *self, u32 *param_2, u8 param_3)
{
    u8 record[8];
    u32 object = (u32)self;
    u32 base = *(volatile u32 *)(object + 0x3cu);
    if (base != 0) {
        *(volatile u32 *)record = *(volatile u32 *)param_2;
        u32 count = *(volatile u32 *)(object + 0x44u);
        *(volatile u8 *)(record + 4) = param_3;
        u32 found = cm_fn_00946464(record, base, count, 6, (void *)0x007c6d40u);
        if (found != 0) {
            base = *(volatile u32 *)(object + 0x3cu);
            u32 index = (found - base) / 6u;
            count = *(volatile u32 *)(object + 0x44u);
            if ((s32)index < (s32)(count - 1u)) {
                u32 offset = index * 6u;
                do {
                    base = *(volatile u32 *)(object + 0x3cu);
                    u32 source = base + offset + 6u;
                    u32 destination = base + offset;
                    ++index;
                    offset += 6u;
                    u32 word = *(volatile u32 *)source;
                    *(volatile u32 *)destination = word;
                    u16 half = *(volatile u16 *)(source + 4u);
                    *(volatile u16 *)(destination + 4u) = half;
                    count = *(volatile u32 *)(object + 0x44u);
                } while ((s32)index < (s32)(count - 1u));
            }
            count = *(volatile u32 *)(object + 0x44u);
            base = *(volatile u32 *)(object + 0x3cu);
            *(volatile u32 *)(base + count * 6u - 6u) = 0xffffffffu;
            count = *(volatile u32 *)(object + 0x44u);
            base = *(volatile u32 *)(object + 0x3cu);
            *(volatile u8 *)(base + count * 6u - 2u) = 0;
            count = *(volatile u32 *)(object + 0x44u);
            *(volatile u32 *)(object + 0x44u) = count - 1u;
        }
    }
}
