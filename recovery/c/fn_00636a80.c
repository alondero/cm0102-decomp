#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_004afbd0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_006399c0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00636a80(u32 self)
{
    u8 scratch[0x2a8];
    u32 allocated, nations, clubs, club, scanned, offset, competition, count, id, target, table, source, text;
    D((u32)scratch) = 0;
    W(self + 0x3e) = 16;
    allocated = (u32)cm_fn_00944e46(0x60);
    D(self + 0xa7) = allocated;
    if (allocated == 0) {
        cm_fn_00944cff(0x009e9f0c, 0, 0, (u32)scratch + 0x1a8, (u32)scratch + 0xa8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x1a8, (u32)scratch + 0xa8, 0x108);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    nations = D(0x00ae2350);
    clubs = D(0x00ae235c);
    club = D(0x00ae23b4);
    D((u32)scratch + 4) = 0;
    if ((s32)(clubs - nations * 2u) > 0) {
        offset = 0;
        do {
            competition = D(club + 0x57);
            if (competition != 0) {
                count = D((u32)scratch);
                if ((s16)count < 40) {
                    id = D(competition);
                    if (id == D(0x009cf854)) {
                        D((u32)scratch + (u32)(s32)(s16)count * 4u + 8) = club;
                        D((u32)scratch) = count + 1u;
                        goto next;
                    }
                }
                id = D(competition);
                target = D(0x009cf80c);
                if (id == target || id == D(0x009cf810)) {
                    if (cm_fn_006399c0(club) != 0 && (s32)offset < 0x4e) {
                        table = D(self + 0xa7);
                        offset += 6;
                        D(table + offset + 0x12) = club;
                        table = D(self + 0xa7);
                        B(table + offset + 0x17) = 0;
                    }
                }
            }
next:
            nations = D(0x00ae2350);
            scanned = D((u32)scratch + 4) + 1u;
            club += 0x245;
            clubs = D(0x00ae235c);
            D((u32)scratch + 4) = scanned;
        } while ((s32)scanned < (s32)(clubs - nations * 2u));
    }
    count = D((u32)scratch);
    cm_fn_004afbd0((u32)scratch + 8, count, 4);
    source = (u32)scratch + 8;
    for (offset = 0; offset < 0x18; offset += 6) {
        table = D(self + 0xa7);
        id = D(source);
        source += 4;
        D(table + offset) = id;
        table = D(self + 0xa7);
        B(table + offset + 5) = 0;
    }
    return 1;
}
