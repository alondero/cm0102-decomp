#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00665a00(u32 self)
{
    u8 scratch[0x204];
    u32 allocated, total, club, selected = 0, scanned = 0, offset = 0;
    u32 competition, id, target, table, random, text;
    W(self + 0x3e) = 28;
    allocated = (u32)cm_fn_00944e46(0xa8);
    D(self + 0xa7) = allocated;
    if (allocated == 0) {
        cm_fn_00944cff(0x009eaf14, 0, 0, (u32)scratch + 0x104, (u32)scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      (u32)scratch + 0x104, (u32)scratch + 4, 0x11a);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    if ((s32)total > 0) {
        do {
            competition = D(club + 0x57);
            if (competition != 0) {
                id = D(competition);
                target = D(0x009cf768);
                if (id == target && (s32)selected < (s32)(s16)W(self + 0x3e)) {
                    table = D(self + 0xa7);
                    ++selected;
                    D(table + offset) = club;
                    table = D(self + 0xa7);
                    B(table + offset + 4) = 0;
                    table = D(self + 0xa7);
                    B(table + offset + 5) = 0;
                    offset += 6;
                    if (selected == (u32)(s32)(s16)W(self + 0x3e))
                        break;
                }
            }
            total = D(0x00ae235c);
            ++scanned;
            club += 581;
        } while ((s32)(s16)scanned < (s32)total);
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    D((u32)scratch) = 0;
    if ((s32)total > 0) {
        offset = selected * 6u;
        do {
            competition = D(club + 0x57);
            if (competition != 0) {
                id = D(competition);
                target = D(0x009cf65c);
                if (id == target && (s32)selected < (s32)(s16)W(self + 0x3e)) {
                    table = D(self + 0xa7);
                    D(table + offset) = club;
                    random = cm_fn_0090cfc0(20);
                    table = D(self + 0xa7);
                    ++selected;
                    B(table + offset + 4) = (u8)random;
                    table = D(self + 0xa7);
                    B(table + offset + 5) = 0;
                    offset += 6;
                    if (selected == (u32)(s32)(s16)W(self + 0x3e))
                        break;
                }
            }
            scanned = D((u32)scratch);
            club += 581;
            ++scanned;
            D((u32)scratch) = scanned;
            total = D(0x00ae235c);
        } while ((s32)(s16)scanned < (s32)total);
    }
    table = D(self + 0xa7);
    cm_fn_00945333(table + 0x42, 16, 6, 0x00522190);
    total = (u32)(s32)(s16)W(self + 0x3e);
    return selected == total;
}
