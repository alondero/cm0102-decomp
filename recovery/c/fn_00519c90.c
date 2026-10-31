/* Preferred-image VA 00519c90, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00519c90(u32 self, u32 club)
{
    u8 scratch[0x200];
    u32 index, table, row, candidate, club_id, text;
    s32 stage, count, slot;
    if (club == 0) {
        cm_fn_00944cff(0x009b55acu, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, scratch + 0x100, scratch, 0x387u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0xffffffffu;
    }
    stage = (s16)W(club + 0x34u);
    table = D(self + 0xa3u);
    count = (s16)W(table + (u32)stage * 104u + 0x1au);
    if (count <= 0)
        return 0xffffffffu;
    table = D(self + 0xadu);
    row = D(table + (u32)stage * 4u);
    for (index = 0; (s32)index < count; ++index, row += 0x35u) {
        slot = (s16)W(row);
        if (slot >= 0) {
            table = D(self + 0xa7u);
            candidate = D(table + (u32)slot * 6u);
            club_id = D(club + 0xcu);
            if (D(candidate) == club_id)
                return index;
        }
        slot = (s16)W(row + 2u);
        if (slot >= 0) {
            table = D(self + 0xa7u);
            candidate = D(table + (u32)slot * 6u);
            club_id = D(club + 0xcu);
            if (D(candidate) == club_id)
                return index;
        }
    }
    return 0xffffffffu;
}
