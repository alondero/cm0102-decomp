/* Preferred-image VA 008a9370, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_008aa9d0(u32 self, u32 raw_kind, u32 second, u32 third);
extern u32 CM_THISCALL cm_fn_008ab480(u32 self, u32 raw_kind, u32 second);
extern u32 CM_THISCALL cm_fn_00601880(u32 self, u32 key, u32 person);
extern u32 CM_THISCALL cm_fn_00898150(u32 self, u32 record, u32 raw_slot, u32 raw_value, u32 type);

u32 CM_THISCALL cm_fn_008a9370(u32 self)
{
    u8 work[0x64];
    u32 raw_index, receiver, candidate, player, best, pointer, left, right;
    u32 object, raw_selected, type, record, raw_slot, i;
    u8 index, outer, score, best_score, candidate_value, best_value;
    D((u32)work + 12u) = self;
    index = 0;
    B((u32)work + 16u) = index;
    do {
        raw_index = D((u32)work + 16u);
        cm_fn_008aa9d0(self, raw_index, 0, 0);
        index = (u8)(index + 1u);
        B((u32)work + 16u) = index;
    } while ((s8)index < 20);
    B((u32)work + 16u) = 0;
    for (i = 0; i < 20u; ++i)
        D((u32)work + 20u + i * 4u) = 0;
    do {
        B((u32)work + 4u) = 0xff;
        best = 0;
        B((u32)work) = 0;
        D((u32)work + 8u) = (u32)work + 20u;
        do {
            raw_index = D((u32)work);
            receiver = D((u32)work + 12u);
            candidate = cm_fn_008ab480(receiver, raw_index, 0);
            if (candidate == 0)
                goto next;
            player = D(candidate + 0x61u);
            if (player == 0)
                goto next;
            receiver = D((u32)work + 12u);
            if (cm_fn_008aa9d0(receiver, raw_index, 0, 0) == 0)
                goto next;
            pointer = D((u32)work + 8u);
            if (D(pointer) != 0)
                goto next;
            receiver = D(0x00b63c98u);
            score = (u8)cm_fn_00601880(receiver, candidate, 0);
            if (score == 0xff)
                score = 51;
            if (best != 0) {
                receiver = D(0x00b63c98u);
                best_score = (u8)cm_fn_00601880(receiver, best, 0);
                if (best_score == 0xff)
                    best_score = 51;
            } else {
                best_score = 51;
            }
            if (B((u32)work + 4u) == 0xff || (s8)score < (s8)best_score)
                goto select;
            if (score != best_score)
                goto next;
            left = D(candidate + 0x61u);
            right = D(best + 0x61u);
            candidate_value = B(left + 0x38u);
            best_value = B(right + 0x38u);
            if ((s8)candidate_value <= (s8)best_value)
                goto next;
select:
            index = B((u32)work);
            B((u32)work + 4u) = index;
            best = candidate;
next:
            index = B((u32)work);
            pointer = D((u32)work + 8u);
            index = (u8)(index + 1u);
            pointer += 4u;
            B((u32)work) = index;
            D((u32)work + 8u) = pointer;
        } while ((s8)index < 20);
        object = D((u32)work + 12u);
        raw_selected = D((u32)work + 4u);
        type = D(object + 8u);
        record = D(object);
        raw_slot = D((u32)work + 16u);
        receiver = D(0x00de1d08u);
        cm_fn_00898150(receiver, record, raw_slot, raw_selected, type);
        index = B((u32)work + 4u);
        if ((s8)index > -1)
            D((u32)work + 20u + (u32)(s32)(s8)index * 4u) = 1;
        outer = B((u32)work + 16u);
        outer = (u8)(outer + 1u);
        B((u32)work + 16u) = outer;
    } while ((s8)outer < 11);
    return 1;
}
