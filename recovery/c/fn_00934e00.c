#include "cmre.h"

extern u32 CM_CDECL cm_fn_00935080(s32 *a1, s32 *a2);

static s16 cm_fn_00934e00_mid16(s16 x, s16 y)
{
    u16 sum = (u16)((u16)x + (u16)y + 1u);
    return (s16)((s16)sum >> 1);
}

static s16 cm_fn_00934e00_mid32(s32 x, s32 y)
{
    u32 sum = (u32)x + (u32)y + 1u;
    return (s16)((s32)sum >> 1);
}

void CM_CDECL cm_fn_00934e00(u32 object)
{
    u32 flags = *(volatile u32 *)(object + 0x0e);
    u32 array_a = *(volatile u32 *)(object + 0x12);
    u32 array_b = *(volatile u32 *)(object + 0x16);
    s32 outer = 0;
    s32 outer_limit = *(volatile s16 *)(object + 4);

    if (outer_limit <= 0) return;
    do {
        u32 upper_indices = *(volatile u32 *)(object + 0x0a);
        u32 lower_indices = *(volatile u32 *)(object + 0x06);
        s32 upper = *(volatile s16 *)(upper_indices + (u32)outer * 2u);
        s32 j = *(volatile s16 *)(lower_indices + (u32)outer * 2u);

        if (j < upper) {
            s16 previous_b = *(volatile s16 *)(array_b + (u32)upper * 2u);
            s16 previous_a = *(volatile s16 *)(array_a + (u32)upper * 2u);
            u32 map_delta = array_a - array_b;
            s32 previous_flag = upper;

            for (;;) {
                u32 map = *(volatile u32 *)(object + 0x1c);
                u32 b_j_address = array_b + (u32)j * 2u;
                s32 mapped = *(volatile s16 *)(map + (u32)j * 2u);
                s16 a_j = *(volatile s16 *)(b_j_address + map_delta);
                s16 b_j = *(volatile s16 *)b_j_address;
                s16 b_mapped = *(volatile s16 *)(array_b + (u32)mapped * 2u);
                s16 a_mapped = *(volatile s16 *)(array_a + (u32)mapped * 2u);
                s32 delta[4];
                s16 effective_mapped_a = a_mapped;
                s16 effective_mapped_b = b_mapped;
                s16 effective_previous_a = previous_a;
                s16 effective_previous_b = previous_b;
                s16 output_a;
                s16 output_b;

                if (*(volatile s8 *)(flags + (u32)j) == 0) {
                    if (*(volatile s8 *)(flags + (u32)mapped) == 0) {
                        effective_mapped_a = cm_fn_00934e00_mid16(a_mapped, a_j);
                        effective_mapped_b = cm_fn_00934e00_mid16(b_mapped, b_j);
                    }
                    if (*(volatile s8 *)(flags + (u32)previous_flag) == 0) {
                        effective_previous_a = cm_fn_00934e00_mid16(previous_a, a_j);
                        effective_previous_b = cm_fn_00934e00_mid16(previous_b, b_j);
                    }
                    s16 aa = cm_fn_00934e00_mid32(effective_mapped_a, a_j);
                    s16 ap = cm_fn_00934e00_mid32(effective_previous_a, a_j);
                    s16 bb = cm_fn_00934e00_mid32(effective_mapped_b, b_j);
                    s16 bp = cm_fn_00934e00_mid32(effective_previous_b, b_j);
                    delta[0] = (s32)aa - (s32)ap;
                    delta[1] = (s32)bb - (s32)bp;
                    delta[2] = delta[0];
                    delta[3] = delta[1];
                    output_a = cm_fn_00934e00_mid32(ap, aa);
                    output_b = cm_fn_00934e00_mid32(bp, bb);
                } else {
                    delta[0] = (s32)a_mapped - (s32)a_j;
                    delta[2] = (s32)a_j - (s32)previous_a;
                    delta[1] = (s32)b_mapped - (s32)b_j;
                    delta[3] = (s32)b_j - (s32)previous_b;
                    output_a = a_j;
                    output_b = b_j;
                }

                (void)cm_fn_00935080(&delta[0], &delta[1]);
                *(volatile s32 *)(*(volatile u32 *)(object + 0x40) + (u32)j * 4u) = delta[0];
                *(volatile s32 *)(*(volatile u32 *)(object + 0x44) + (u32)j * 4u) = delta[1];

                if (*(volatile s8 *)(flags + (u32)j) != 0 &&
                    (delta[2] != 0 || delta[0] != 0) &&
                    (delta[3] != 0 || delta[1] != 0)) {
                    (void)cm_fn_00935080(&delta[2], &delta[3]);
                    *(volatile s32 *)(*(volatile u32 *)(object + 0x48) + (u32)j * 4u) = delta[2];
                    *(volatile s32 *)(*(volatile u32 *)(object + 0x4c) + (u32)j * 4u) = delta[3];
                } else {
                    u32 src = *(volatile u32 *)(object + 0x40);
                    u32 dst = *(volatile u32 *)(object + 0x48);
                    s32 value = *(volatile s32 *)(src + (u32)j * 4u);
                    *(volatile s32 *)(dst + (u32)j * 4u) = value;
                    src = *(volatile u32 *)(object + 0x44);
                    dst = *(volatile u32 *)(object + 0x4c);
                    value = *(volatile s32 *)(src + (u32)j * 4u);
                    *(volatile s32 *)(dst + (u32)j * 4u) = value;
                }

                *(volatile s16 *)(*(volatile u32 *)(object + 0x2c) + (u32)j * 2u) = output_a;
                *(volatile s16 *)(*(volatile u32 *)(object + 0x30) + (u32)j * 2u) = output_b;
                previous_a = a_j;
                previous_b = b_j;
                previous_flag = j;
                ++j;
                if (j > upper) break;
            }
        }

        ++outer;
        outer_limit = *(volatile s16 *)(object + 4);
    } while (outer < outer_limit);
}
