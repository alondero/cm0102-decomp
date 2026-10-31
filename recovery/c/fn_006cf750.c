#include "cmre.h"

void CM_THISCALL cm_fn_006cf750(void *object, u8 param_2, u8 param_3)
{
    u32 base = (u32)(unsigned long)object;
    u16 mode = *(volatile u16 *)(base + 0x8ed4u);
    u32 index;
    u8 bl = 0;

    if (mode == 0x1efu)
        index = 0;
    else if (mode == 0x3deu)
        index = 1;
    else
        index = 2;

    ++*(volatile u16 *)(base + 0x214u + index * 2u);
    ++*(volatile u16 *)(base + 0x21au);

    if (param_3 != 0) {
        s32 row = (s8)*(volatile u8 *)(base + 0x8e9bu);
        s16 delta = (s8)*(volatile u8 *)(base + (u32)(row + 0xau));
        *(volatile u16 *)(base + 0x1feu) =
            (u16)(*(volatile u16 *)(base + 0x1feu) - (u16)delta);

        u32 row_base = base + (u32)row;
        u32 total = *(volatile u32 *)(base + 0x8f12u);
        delta = (s8)*(volatile u8 *)(row_base + 0x41u);
        *(volatile u16 *)(base + 0x208u) =
            (u16)(*(volatile u16 *)(base + 0x208u) - (u16)delta);
        delta = (s8)*(volatile u8 *)(row_base + 0x78u);
        *(volatile u16 *)(base + 0x212u) =
            (u16)(*(volatile u16 *)(base + 0x212u) - (u16)delta);

        s32 value = (s8)*(volatile u8 *)(row_base + 0x8d51u);
        *(volatile u32 *)(base + 0x8f12u) = total - (u32)value;
        total = *(volatile u32 *)(base + 0x8f26u);
        value = (s8)*(volatile u8 *)(row_base + 0x8d88u);
        *(volatile u32 *)(base + 0x8f26u) = total - (u32)value;
        total = *(volatile u32 *)(base + 0x8f3au);
        value = (s8)*(volatile u8 *)(row_base + 0x8dbfu);
        *(volatile u32 *)(base + 0x8f3au) = total - (u32)value;
        total = *(volatile u32 *)(base + 0x8f4eu);
        value = (s8)*(volatile u8 *)(row_base + 0x8df6u);
        *(volatile u32 *)(base + 0x8f4eu) = total - (u32)value;
        total = *(volatile u32 *)(base + 0x8f62u);
        value = (s8)*(volatile u8 *)(row_base + 0x8e2du);
        *(volatile u32 *)(base + 0x8f62u) = total - (u32)value;
        total = *(volatile u32 *)(base + 0x8f76u);
        value = (s8)*(volatile u8 *)(row_base + 0x8e64u);
        *(volatile u32 *)(base + 0x8f76u) = total - (u32)value;

        *(volatile u8 *)(row_base + 0x8d51u) = bl;
        row = (s8)*(volatile u8 *)(base + 0x8e9bu);
        *(volatile u8 *)(base + (u32)(row + 0x8d88)) = bl;
        row = (s8)*(volatile u8 *)(base + 0x8e9bu);
        *(volatile u8 *)(base + (u32)(row + 0x8dbf)) = bl;
        row = (s8)*(volatile u8 *)(base + 0x8e9bu);
        *(volatile u8 *)(base + (u32)(row + 0x8df6)) = bl;
        row = (s8)*(volatile u8 *)(base + 0x8e9bu);
        *(volatile u8 *)(base + (u32)(row + 0x8e2d)) = bl;
        row = (s8)*(volatile u8 *)(base + 0x8e9bu);
        *(volatile u8 *)(base + (u32)(row + 0x8e64)) = bl;

        s8 tier = (s8)*(volatile u8 *)(base + 0x8ea8u);
        if (tier <= 3) {
            if (param_2 == 0) {
                u32 value32 = *(volatile u32 *)(base + 0x8f52u + index * 4u);
                *(volatile u32 *)(base + 0x8f52u + index * 4u) = value32 + 1u;
                u32 total_a = *(volatile u32 *)(base + 0x8f5eu);
                u32 total_b = *(volatile u32 *)(base + 0x8f62u);
                *(volatile u32 *)(base + 0x8f5eu) = total_a + 1u;
                row = (s8)*(volatile u8 *)(base + 0x8e9bu);
                *(volatile u32 *)(base + 0x8f62u) = total_b + 1u;
                *(volatile u8 *)(base + (u32)(row + 0x8e2d)) = 1;
            } else if (param_2 == 1) {
                u32 value32 = *(volatile u32 *)(base + 0x8f16u + index * 4u);
                *(volatile u32 *)(base + 0x8f16u + index * 4u) = value32 + 1u;
                u32 total_a = *(volatile u32 *)(base + 0x8f22u);
                u32 total_b = *(volatile u32 *)(base + 0x8f26u);
                *(volatile u32 *)(base + 0x8f22u) = total_a + 1u;
                *(volatile u32 *)(base + 0x8f26u) = total_b + 1u;
                row = (s8)*(volatile u8 *)(base + 0x8e9bu);
                *(volatile u8 *)(base + (u32)(row + 0x8d88)) = 1;
            }
            ++*(volatile u16 *)(base + 0x1f6u + index * 2u);
            ++*(volatile u16 *)(base + 0x1fcu);
            row = (s8)*(volatile u8 *)(base + 0x8e9bu);
            ++*(volatile u16 *)(base + 0x1feu);
            *(volatile u8 *)(base + (u32)(row + 0xau)) = 1;
            row = (s8)*(volatile u8 *)(base + 0x8e9bu);
            *(volatile u8 *)(base + (u32)(row + 0x41)) = 0;
            row = (s8)*(volatile u8 *)(base + 0x8e9bu);
            *(volatile u8 *)(base + (u32)(row + 0x78)) = 0;
        } else if (tier < 8) {
            if (param_2 == 0) {
                u32 value32 = *(volatile u32 *)(base + 0x8f2au + index * 4u);
                *(volatile u32 *)(base + 0x8f2au + index * 4u) = value32 + 1u;
                u32 total_a = *(volatile u32 *)(base + 0x8f36u);
                u32 total_b = *(volatile u32 *)(base + 0x8f3au);
                *(volatile u32 *)(base + 0x8f36u) = total_a + 1u;
                row = (s8)*(volatile u8 *)(base + 0x8e9bu);
                *(volatile u32 *)(base + 0x8f3au) = total_b + 1u;
                *(volatile u8 *)(base + (u32)(row + 0x8dbf)) = 1;
            } else if (param_2 == 1) {
                u32 value32 = *(volatile u32 *)(base + 0x8f3eu + index * 4u);
                *(volatile u32 *)(base + 0x8f3eu + index * 4u) = value32 + 1u;
                u32 total_a = *(volatile u32 *)(base + 0x8f4au);
                u32 total_b = *(volatile u32 *)(base + 0x8f4eu);
                *(volatile u32 *)(base + 0x8f4au) = total_a + 1u;
                *(volatile u32 *)(base + 0x8f4eu) = total_b + 1u;
                row = (s8)*(volatile u8 *)(base + 0x8e9bu);
                *(volatile u8 *)(base + (u32)(row + 0x8df6)) = 1;
            }
            ++*(volatile u16 *)(base + 0x200u + index * 2u);
            ++*(volatile u16 *)(base + 0x206u);
            row = (s8)*(volatile u8 *)(base + 0x8e9bu);
            ++*(volatile u16 *)(base + 0x208u);
            *(volatile u8 *)(base + (u32)(row + 0xau)) = 0;
            row = (s8)*(volatile u8 *)(base + 0x8e9bu);
            *(volatile u8 *)(base + (u32)(row + 0x41)) = 1;
            row = (s8)*(volatile u8 *)(base + 0x8e9bu);
            *(volatile u8 *)(base + (u32)(row + 0x78)) = 0;
        } else {
            if (param_2 == 0) {
                u32 value32 = *(volatile u32 *)(base + 0x8f02u + index * 4u);
                *(volatile u32 *)(base + 0x8f02u + index * 4u) = value32 + 1u;
                u32 total_a = *(volatile u32 *)(base + 0x8f0eu);
                u32 total_b = *(volatile u32 *)(base + 0x8f12u);
                *(volatile u32 *)(base + 0x8f0eu) = total_a + 1u;
                row = (s8)*(volatile u8 *)(base + 0x8e9bu);
                *(volatile u32 *)(base + 0x8f12u) = total_b + 1u;
                *(volatile u8 *)(base + (u32)(row + 0x8d51)) = 1;
            } else if (param_2 == 1) {
                u32 value32 = *(volatile u32 *)(base + 0x8f66u + index * 4u);
                *(volatile u32 *)(base + 0x8f66u + index * 4u) = value32 + 1u;
                u32 total_a = *(volatile u32 *)(base + 0x8f72u);
                u32 total_b = *(volatile u32 *)(base + 0x8f76u);
                *(volatile u32 *)(base + 0x8f72u) = total_a + 1u;
                *(volatile u32 *)(base + 0x8f76u) = total_b + 1u;
                row = (s8)*(volatile u8 *)(base + 0x8e9bu);
                *(volatile u8 *)(base + (u32)(row + 0x8e64)) = 1;
            }
            ++*(volatile u16 *)(base + 0x20au + index * 2u);
            ++*(volatile u16 *)(base + 0x210u);
            row = (s8)*(volatile u8 *)(base + 0x8e9bu);
            ++*(volatile u16 *)(base + 0x212u);
            *(volatile u8 *)(base + (u32)(row + 0xau)) = 0;
            row = (s8)*(volatile u8 *)(base + 0x8e9bu);
            *(volatile u8 *)(base + (u32)(row + 0x41)) = 0;
            row = (s8)*(volatile u8 *)(base + 0x8e9bu);
            *(volatile u8 *)(base + (u32)(row + 0x78)) = 1;
        }

        u8 next = (u8)(*(volatile u8 *)(base + 0x8e9bu) + 1u);
        *(volatile u8 *)(base + 0x8e9bu) = next;
        if ((s8)next >= 0x37)
            *(volatile u8 *)(base + 0x8e9bu) = 0;
        return;
    }

    if (param_2 != 0xffu) {
        s8 tier = (s8)*(volatile u8 *)(base + 0x8ea8u);
        if (tier < 4) {
            if (param_2 != 0) {
                if (param_2 == 1) {
                    u32 value32 = *(volatile u32 *)(base + 0x8f16u + index * 4u);
                    *(volatile u32 *)(base + 0x8f16u + index * 4u) = value32 + 1u;
                    value32 = *(volatile u32 *)(base + 0x8f22u);
                    *(volatile u32 *)(base + 0x8f22u) = value32 + 1u;
                }
                ++*(volatile u16 *)(base + 0x1f6u + index * 2u);
                ++*(volatile u16 *)(base + 0x1fcu);
                return;
            }
            u32 value32 = *(volatile u32 *)(base + 0x8f52u + index * 4u);
            *(volatile u32 *)(base + 0x8f52u + index * 4u) = value32 + 1u;
            value32 = *(volatile u32 *)(base + 0x8f5eu);
            *(volatile u32 *)(base + 0x8f5eu) = value32 + 1u;
            ++*(volatile u16 *)(base + 0x1f6u + index * 2u);
            ++*(volatile u16 *)(base + 0x1fcu);
            return;
        }
        if (tier < 8) {
            if (param_2 != 0) {
                if (param_2 == 1) {
                    u32 value32 = *(volatile u32 *)(base + 0x8f3eu + index * 4u);
                    *(volatile u32 *)(base + 0x8f3eu + index * 4u) = value32 + 1u;
                    value32 = *(volatile u32 *)(base + 0x8f4au);
                    *(volatile u32 *)(base + 0x8f4au) = value32 + 1u;
                }
                ++*(volatile u16 *)(base + 0x200u + index * 2u);
                ++*(volatile u16 *)(base + 0x206u);
                return;
            }
            u32 value32 = *(volatile u32 *)(base + 0x8f2au + index * 4u);
            *(volatile u32 *)(base + 0x8f2au + index * 4u) = value32 + 1u;
            value32 = *(volatile u32 *)(base + 0x8f36u);
            *(volatile u32 *)(base + 0x8f36u) = value32 + 1u;
            ++*(volatile u16 *)(base + 0x200u + index * 2u);
            ++*(volatile u16 *)(base + 0x206u);
            return;
        }
        if (param_2 == 0) {
            u32 value32 = *(volatile u32 *)(base + 0x8f02u + index * 4u);
            *(volatile u32 *)(base + 0x8f02u + index * 4u) = value32 + 1u;
            value32 = *(volatile u32 *)(base + 0x8f0eu);
            *(volatile u32 *)(base + 0x8f0eu) = value32 + 1u;
        } else if (param_2 == 1) {
            u32 value32 = *(volatile u32 *)(base + 0x8f66u + index * 4u);
            *(volatile u32 *)(base + 0x8f66u + index * 4u) = value32 + 1u;
            value32 = *(volatile u32 *)(base + 0x8f72u);
            *(volatile u32 *)(base + 0x8f72u) = value32 + 1u;
        }
        ++*(volatile u16 *)(base + 0x20au + index * 2u);
        ++*(volatile u16 *)(base + 0x210u);
    }
}
