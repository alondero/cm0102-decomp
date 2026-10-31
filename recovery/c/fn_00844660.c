#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 record);
extern u32 CM_THISCALL cm_fn_00843e10(u32 self, u32 record, u32 target, u8 constant, u8 flag);

void CM_THISCALL cm_fn_00844660(u32 self, u32 target, u32 row_slot)
{
    u32 offset;
    u32 rows = 0x28u;
    for (offset = 0u; rows != 0u; offset += 0x25u, --rows) {
        u32 row_base = *(volatile u32 *)row_slot;
        u32 limit = CM_GLOBAL(volatile u32, 0x00ae2364u) - 1u;
        u32 field = *(volatile u32 *)(row_base + offset + 0x0fu);
        if ((s32)field > (s32)limit)
            *(volatile u32 *)(row_base + offset + 0x0fu) = 0xffffffffu;

        row_base = *(volatile u32 *)row_slot;
        field = *(volatile u32 *)(row_base + offset + 0x0fu);
        if ((s32)field <= -1)
            continue;

        u32 self_for_lookup = CM_GLOBAL(volatile u32, 0x00ae1050u);
        u32 scaled = field * 110u;
        u32 record = CM_GLOBAL(volatile u32, 0x00ae23bcu) + scaled;
        u32 looked_up = cm_fn_004cf5a0(self_for_lookup, record);
        CM_GLOBAL(volatile u32, 0x00ae1080u) = looked_up;
        u32 mapping = 0u;
        if (looked_up != 0u &&
            ((*(volatile u8 *)(looked_up + 0x35u) & 0x3fu) != 5u)) {
            u32 mapped_index = *(volatile u32 *)(looked_up + 4u);
            mapping = CM_GLOBAL(volatile u32, 0x00ae23b4u) + mapped_index * 581u;
        }
        if (mapping == target || *(volatile u32 *)(record + 0x39u) == target)
            continue;

        row_base = *(volatile u32 *)row_slot;
        if (*(volatile u32 *)(row_base + offset + 0x17u) != 0xffffffffu)
            continue;
        u8 row_byte = *(volatile u8 *)(row_base + offset);
        (void)cm_fn_00843e10(self, record, target, 3u, (u8)(row_byte != 0x0bu));
    }
}


