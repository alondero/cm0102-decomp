#include "cmre.h"

void CM_CDECL cm_fn_009340d0(volatile u8 *object, volatile u8 *arrays,
                              u16 mode, s16 series, s16 requested)
{
    u32 selected_array;
    u32 other_array;
    u32 offset;
    u32 existing;
    u32 candidate;
    u32 aligned;
    u32 target;
    s32 difference;
    s32 count;
    s32 lower;
    s32 upper;
    s32 midpoint;
    u32 sum_bits;
    s32 sign;

    if (mode != 0) {
        selected_array = *(volatile u32 *)(arrays + 0x18);
        other_array = *(volatile u32 *)(object + 0x3c8);
    } else {
        selected_array = *(volatile u32 *)(arrays + 0x1c);
        other_array = *(volatile u32 *)(object + 0x3cc);
    }
    offset = (u32)(s32)requested << 2;
    target = *(volatile u32 *)(selected_array + offset);
    existing = *(volatile u32 *)(other_array + offset);

    if ((u16)series != 0x270fu) {
        s32 comparison = *(volatile s32 *)(object + 0x5c + (s32)series * 4);
        if ((s32)existing > comparison) {
            candidate = existing - 0x60u;
            if (comparison > (s32)candidate)
                existing = (u32)comparison;
            else
                existing = candidate;
        } else {
            candidate = existing + 0x60u;
            if (comparison < (s32)candidate)
                existing = (u32)comparison;
            else
                existing = candidate;
        }
    }
    aligned = existing + 0x20u;
    aligned &= 0xffffffc0u;
    difference = (s32)(target - aligned);
    *(volatile s32 *)(object + 0x42e) = difference;
    if (difference == 0)
        goto commit;

    count = *(volatile s32 *)(object + 0x42a);
    if (count >= 0x0e)
        goto commit;
    if (count != 0) {
        s32 center = *(volatile s32 *)(object + 0x426);
        if (difference > 0) {
            lower = *(volatile s32 *)(object + 0x422);
            *(volatile s32 *)(object + 0x41e) = center;
        } else {
            upper = *(volatile s32 *)(object + 0x41e);
            *(volatile s32 *)(object + 0x422) = center;
            lower = upper;
        }
        if (difference > 0) {
            sum_bits = (u32)lower + (u32)center;
        } else {
            sum_bits = (u32)lower + (u32)center;
        }
        sign = (s32)sum_bits < 0 ? -1 : 0;
        midpoint = (s32)(sum_bits - (u32)sign) >> 1;
        *(volatile s32 *)(object + 0x426) = midpoint;
    }
    ++count;
    *(volatile u32 *)(object + 0x432) = 1;
    *(volatile s32 *)(object + 0x42a) = count;
    return;

commit:
    *(volatile u32 *)(selected_array + offset) = aligned;
    *(volatile u32 *)(object + 0x426) = 0x1000;
    *(volatile u32 *)(object + 0x422) = 0x800;
    *(volatile u32 *)(object + 0x41e) = 0x2000;
    *(volatile u32 *)(object + 0x42a) = 0;
    *(volatile u32 *)(object + 0x432) = 0;
}
