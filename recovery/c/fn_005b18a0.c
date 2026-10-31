/* WIP: memory mismatch, case 117 (-O0), after preserving all pushed bits.
   Two failed tests; investigate the first differing shared-callee write. */
#include "cmre.h"

/* Preferred-image VA 0x005b18a0, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve all pushed argument bits, including the EAX high half left by
   0x005b1a00 on the first iteration and by the accumulator on later iterations.
   Empty input preserves the high half of the pointer loaded from self. */
extern u32 CM_THISCALL cm_fn_005b1a00(u32 self, u32 flags, u32 side);
extern u32 CM_THISCALL cm_fn_005b1ba0(u32 self, u32 flags, u32 side, u32 id);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))
#define U16_AT(base, offset) (*(volatile u16 *)((base) + (offset)))

u32 CM_THISCALL cm_fn_005b18a0(u32 self, u32 flags, u32 side, u32 ids)
{
    u32 root = U32_AT(self, 0);
    u32 sum = 0, count = 0, record, value, base, call_word;
    s32 index = 0;
    u16 id;
    if (U32_AT(root, 0) == 0)
        return root & 0xffff0000u;
    call_word = cm_fn_005b1a00(self, flags, side);
    id = U16_AT(ids, 0);
    while (id != 0) {
        call_word = (call_word & 0xffff0000u) | id;
        record = cm_fn_005b1ba0(self, flags, side, call_word);
        if (U16_AT(ids + (u32)index * 2u, 2) != 0) {
            value = U32_AT(record, 0x18);
            sum += value;
        } else {
            value = U32_AT(record, 8);
            base = U32_AT(record, 0);
            sum += ((value << 6) + base) * 0x400u - 1u;
        }
        ++count;
        index = (s16)count;
        id = U16_AT(ids + (u32)index * 2u, 0);
        call_word = sum;
    }
    return (u32)((s32)(sum + 0x8000u) >> 16);
}
