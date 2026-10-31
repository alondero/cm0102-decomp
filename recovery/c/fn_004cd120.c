/* Preferred-image VA 004cd120, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole direct caller VA006830f7 overwrites EAX immediately after the call. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0085a850(u32 self, u32 player, u32 club, u8 flag);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);

void CM_THISCALL cm_fn_004cd120(u32 self, u32 club, u32 update)
{
    u32 row, i, player, id, limit, receiver, other, current, table, record, club_id;
    u32 direction = club;
    u32 pass;
    if (club == 0)
        return;
    if (update != 0) {
        row = club + 0xd7u;
        for (i = 0; i < 50u; ++i, row += 4u) {
            player = D(row);
            if (player != 0) {
                id = D(player);
                limit = D(0x00ae2364u);
                if ((s32)id < (s32)limit) {
                    receiver = D(0x00dd7ef0u);
                    cm_fn_0085a850(receiver, player, club, 0);
                }
            }
        }
    }
    other = cm_fn_00540a50(club, (u32)&direction, 1);
    if (other != 0 && *(volatile u32 *)&direction == 0)
        return;
    current = club;
    for (pass = 0; pass < 2u; ++pass) {
        if (current != 0) {
            row = current + 0xd7u;
            for (i = 0; i < 50u; ++i, row += 4u) {
                player = D(row);
                if (player == 0)
                    continue;
                id = D(player);
                limit = D(0x00ae2364u);
                if ((s32)id >= (s32)limit)
                    continue;
                /* The native repeats the count read and signed guard. */
                limit = D(0x00ae2364u);
                if ((s32)id >= (s32)limit)
                    continue;
                table = D(0x00ae2d0cu);
                record = D(table + id * 79u);
                if ((s32)record < 0)
                    continue;
                table = D(self);
                record = record * 80u + table;
                if (record == 0)
                    continue;
                club_id = D(record + 4u);
                if (club_id != D(current))
                    continue;
                if (B(record + 0x1fu) == 1)
                    B(record + 0x1fu) = 0;
                if (B(record + 0x1cu) == 1)
                    B(record + 0x1cu) = 0;
            }
        }
        current = other;
    }
}
