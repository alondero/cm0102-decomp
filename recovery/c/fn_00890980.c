#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_004afbd0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006830b0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006831a0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00890980(u32 self)
{
    u8 scratch[0x208];
    u32 table, id, parent, allocated, total, club, scanned, selected, competition;
    u32 nation, target, index, value, base, row, count, text, result;
    table = D(0x00adadfc);
    D((u32)scratch + 4) = self;
    id = D(0x009cf5f0);
    parent = D(table + id * 4u);
    selected = 0;
    D((u32)scratch) = parent;
    allocated = (u32)cm_fn_00944e46(0x120);
    if (allocated == 0) {
        cm_fn_00944cff(0x00a8b578, 0, 0, (u32)scratch + 0x108, (u32)scratch + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      (u32)scratch + 0x108, (u32)scratch + 8, 0x224);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    scanned = 0;
    if ((s32)total > 0) {
        do {
            if ((s16)selected >= 72)
                break;
            competition = D(club + 0x57);
            if (competition != 0) {
                nation = D(club + 0x53);
                if (nation != 0) {
                    id = D(competition);
                    target = D(0x009cf5f8);
                    if (id == target || id == D(0x009cf600) || id == D(0x009cf608) ||
                        id == D(0x009cf5fc) || id == D(0x009cf604) || id == D(0x009cf5f4)) {
                        index = (u32)(s32)(s16)selected;
                        ++selected;
                        D(allocated + index * 4u) = club;
                    }
                }
            }
            total = D(0x00ae235c);
            ++scanned;
            club += 581;
        } while ((s32)scanned < (s32)total);
        if ((s16)selected != 0) {
            cm_fn_004afbd0(allocated, selected, 3);
            for (index = 0; index != 3; ++index) {
                id = D(0x009cf5f0);
                value = D(allocated + index * 4u);
                base = D(0x00ae23d0);
                competition = id * 107u + base;
                /* Both helpers overwrite incoming ECX; retain its stack read. */
                (void)D((u32)scratch + 4);
                cm_fn_006830b0(value, competition, 1);
            }
            parent = D((u32)scratch);
            count = W(parent + 0x3e);
            if ((s16)count > 0) {
                index = 0;
                row = 0;
                do {
                    table = D(parent + 0xb1);
                    value = B(table + row + 0x37);
                    if (value == 3) {
                        id = D(0x009cf5f8);
                        value = D(table + row);
                        base = D(0x00ae23d0);
                        competition = id * 107u + base;
                        (void)D((u32)scratch + 4);
                        cm_fn_006831a0(value, competition, 1);
                        parent = D((u32)scratch);
                    }
                    count = W(parent + 0x3e);
                    ++index;
                    row += 59;
                } while ((s32)index < (s32)(s16)count);
            }
        }
    }
    return (u32)cm_fn_009452ca(allocated);
}
