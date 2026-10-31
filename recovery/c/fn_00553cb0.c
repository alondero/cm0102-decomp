#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern void CM_CDECL cm_fn_004afbd0(u32, s16, s16);
extern u32 CM_STDCALL cm_fn_006830b0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006831a0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00553cb0(u32 self)
{
    u8 scratch[0x208];
    u32 local = (u32)scratch;
    u32 table, id, parent, total, allocation, club, scanned, selected;
    u32 competition, nation, target, index, value, base, cursor, row, text, result;
    /* Both 006830b0 and 006831a0 overwrite incoming ECX. */
    (void)self;
    table = D(0x00adadfc);
    id = D(0x009cf5b4);
    parent = D(table + id * 4u);
    total = D(0x00ae235c);
    D(local + 4) = parent;
    allocation = (u32)cm_fn_00944e46(total * 4u);
    D(local) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009b8e6c, 0, 0, local + 0x108, local + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
            local + 0x108, local + 8, 0x256);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    scanned = 0;
    selected = 0;
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    if ((s32)total > 0) {
        do {
            /* A nonzero result exits the entire scan, before its footer. */
            if (cm_fn_0053b900(club) != 0)
                break;
            competition = D(club + 0x57);
            if (competition != 0) {
                nation = D(club + 0x53);
                if (nation != 0) {
                    id = D(nation);
                    target = D(0x009cf2c0);
                    if (id == target) {
                        id = D(competition);
                        target = D(0x009cf764);
                        if (id == target) {
                            allocation = D(local);
                            index = (u32)(s32)(s16)selected;
                            ++selected;
                            D(allocation + index * 4u) = club;
                        }
                    }
                }
            }
            total = D(0x00ae235c);
            ++scanned;
            club += 581;
        } while ((s32)scanned < (s32)total);
        if ((s16)selected != 0) {
            allocation = D(local);
            cm_fn_004afbd0(allocation, (s16)selected, 4);
            cursor = allocation;
            index = 4;
            do {
                id = D(0x009cf5b4);
                value = D(cursor);
                base = D(0x00ae23d0);
                cm_fn_006830b0(value, base + id * 107u, 1);
                cursor += 4;
            } while (--index != 0);
            parent = D(local + 4);
            index = 0;
            if ((s16)W(parent + 0x3e) > 0) {
                do {
                    row = cm_fn_006826d0(parent, index);
                    if (B(row + 0x37) == 3) {
                        id = D(0x009cf764);
                        base = D(0x00ae23d0);
                        competition = base + id * 107u;
                        row = cm_fn_006826d0(parent, index);
                        value = D(row);
                        cm_fn_006831a0(value, competition, 1);
                    }
                    total = (u32)(s32)(s16)W(parent + 0x3e);
                    ++index;
                } while ((s32)index < (s32)total);
                allocation = D(local);
            }
            return (u32)cm_fn_009452ca(allocation);
        }
        allocation = D(local);
    }
    return (u32)cm_fn_009452ca(allocation);
}
