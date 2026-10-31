#include "cmre.h"

extern u8 *cm_fn_00951d10(u8 *param_1, u8 *param_2);

u32
cm_fn_0094e277(u8 *param_1, u8 *param_2)
{
    u32 pattern = (u32)param_1;
    u32 candidate_base = (u32)param_2;
u32 candidate;
    u8 candidate_byte;
    u8 pattern_byte;
    u8 next_candidate_byte;
    u8 next_pattern_byte;
    u8 *fallback;

    if (CM_GLOBAL(volatile s32, 0x00de441c) == 0) {
        fallback = cm_fn_00951d10(param_1, param_2);
        return (u32)fallback;
    }

    if (*(volatile u8 *)pattern == 0)
        goto result;
    for (;;) {
        candidate = candidate_base;
        candidate_byte = *(volatile u8 *)candidate;
        if (candidate_byte == 0)
            goto candidate_end;

        for (;;) {
        candidate_byte = *(volatile u8 *)candidate;
            if ((CM_GLOBAL(volatile u8, 0x00de4521u + (u32)candidate_byte) & 4u) != 0) {
                pattern_byte = *(volatile u8 *)pattern;
                if (candidate_byte == pattern_byte) {
                    next_candidate_byte = *(volatile u8 *)(candidate + 1u);
                    next_pattern_byte = *(volatile u8 *)(pattern + 1u);
                    if (next_candidate_byte == next_pattern_byte)
                        goto candidate_end;
                }
                next_candidate_byte = *(volatile u8 *)(candidate + 1u);
                if (next_candidate_byte == 0)
                    goto candidate_end;
                candidate += 2u;
                if (*(volatile u8 *)candidate == 0)
                    goto candidate_end;
            } else {
                pattern_byte = *(volatile u8 *)pattern;
                if (candidate_byte == pattern_byte)
                    goto candidate_end;
                ++candidate;
                if (*(volatile u8 *)candidate == 0)
                    goto candidate_end;
            }
        }

candidate_end:
        if (*(volatile u8 *)candidate != 0)
            goto result;
        pattern_byte = *(volatile u8 *)pattern;
        if ((CM_GLOBAL(volatile u8, 0x00de4521u + (u32)pattern_byte) & 4u) != 0) {
            ++pattern;
            if (*(volatile u8 *)pattern == 0)
                break;
            ++pattern;
            if (*(volatile u8 *)pattern == 0)
                break;
        } else {
            ++pattern;
            if (*(volatile u8 *)pattern == 0)
                break;
        }
    }

result:
    pattern_byte = *(volatile u8 *)pattern;
    return ((pattern_byte != 0) ? 0xffffffffu : 0u) & pattern;
}

