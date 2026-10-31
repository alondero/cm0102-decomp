#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00517bb0(u32 self)
{
    u8 scratch[0x208];
    u32 total, nations, club, competition, id, target, table, offset, value;
    u32 selected, scanned, limit, allocation, text;
    D((u32)scratch + 4) = 0;
    W(self + 0x3e) = 0x30;
    allocation = (u32)cm_fn_00944e46(0x120);
    D(self + 0xa7) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009b5468, 0, 0, (u32)scratch + 0x108, (u32)scratch + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      (u32)scratch + 0x108, (u32)scratch + 8, 0x12b);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    total = D(0x00ae235c);
    nations = D(0x00ae2350);
    club = D(0x00ae23b4);
    D((u32)scratch) = 0;
    offset = 0;
    limit = total - nations * 2u;
    if ((s32)limit > 0) {
        do {
            competition = D(club + 0x57);
            if (competition != 0) {
                id = D(competition);
                target = D(0x009cf8a8);
                if (id == target || id == D(0x009cf8ac)) {
                    table = D(self + 0xa7);
                    D(table + offset) = club;
                    table = D(self + 0xa7);
                    B(table + offset + 5) = 0;
                    value = cm_fn_0090cfc0(0x32);
                    table = D(self + 0xa7);
                    B(table + offset + 4) = (u8)value;
                    selected = D((u32)scratch + 4);
                    total = D(0x00ae235c);
                    nations = D(0x00ae2350);
                    ++selected;
                    offset += 6;
                    D((u32)scratch + 4) = selected;
                }
            }
            scanned = D((u32)scratch);
            ++scanned;
            club += 581;
            D((u32)scratch) = scanned;
            limit = total - nations * 2u;
            scanned = D((u32)scratch);
        } while ((s32)scanned < (s32)limit);
    }
    club = D(0x00ae23b4);
    limit = total - nations * 2u;
    scanned = 0;
    if ((s32)limit > 0) {
        offset = D((u32)scratch + 4) * 6u;
        do {
            competition = D(club + 0x57);
            if (competition != 0) {
                id = D(competition);
                if (id == D(0x009cf8a4)) {
                    table = D(self + 0xa7);
                    offset += 6;
                    D(offset + table - 6) = club;
                    table = D(self + 0xa7);
                    B(offset + table - 1) = 0;
                    total = D(0x00ae235c);
                    nations = D(0x00ae2350);
                }
            }
            ++scanned;
            D((u32)scratch) = scanned;
            club += 581;
            limit = total - nations * 2u;
            scanned = D((u32)scratch);
        } while ((s32)scanned < (s32)limit);
    }
    return 1;
}
