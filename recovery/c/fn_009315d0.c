#include "cmre.h"

extern void CM_THISCALL cm_fn_00931690(u32 self, u32 entry, u32 record,
    u32 list);
typedef u32 (CM_CDECL *record_predicate_fn)(u32 row, u32 record);

void CM_THISCALL cm_fn_009315d0(u32 self, u8 *record, u32 base,
    record_predicate_fn predicate)
{
    if (*(volatile u8 *)(record + 0x3au) == 1u)
        return;

    for (s8 pass = 0; pass < 2; ++pass) {
        u32 list = *(volatile u32 *)(record + (pass == 0 ? 0x1cu : 0x20u));
        u32 table = base + (u32)(s32)pass * 0x625u + 0x190u;
        for (s8 slot = 0; slot < 0x14; ++slot) {
            u32 row = table;
            s32 id = *(volatile s32 *)row;
            if (id != -1 && *(volatile u8 *)(row + 0x1du) != 0xfbu &&
                slot < 0x0b) {
                u32 scaled_id = (u32)id * 0x6eu;
                u32 player = CM_GLOBAL(volatile u32, 0x00ae23bcu) + scaled_id;
                if (player != 0) {
                    u32 row_value = *(volatile u32 *)player;
                    s32 cutoff = CM_GLOBAL(volatile s32, 0x00ae2364u);
                    if ((s32)row_value >= cutoff)
                        goto next_slot;
                }
                if (predicate(player, (u32)record) != 0)
                    cm_fn_00931690(self, row, (u32)record, list);
            }
        next_slot:
            table += 0x49u;
        }
    }
}
