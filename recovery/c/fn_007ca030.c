#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_004afbd0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006830b0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006831a0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007ca030(u32 self)
{
    u8 scratch[0x208];
    u32 table, id, group, total, allocation, club, count, scanned, country, competition, value;
    u32 index, base, target, end, adjustment, row, text, result;
    /* The original forwards ECX to 006830b0/006831a0, both stdcall
       boundaries that overwrite ECX before any incoming-value use. */
    (void)self;
    table = D(0x00adadfc);
    id = D(0x009cf85c);
    group = D(table + id * 4u);
    total = D(0x00ae235c);
    D((u32)scratch) = group;
    allocation = (u32)cm_fn_00944e46(total * 4u);
    if (allocation == 0) {
        cm_fn_00944cff(0x00a78598, 0, 0, (u32)scratch + 0x108, (u32)scratch + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x108, (u32)scratch + 8, 0x2f8);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    count = 0;
    scanned = 0;
    if ((s32)total > 0) {
        do {
            country = D(club + 0x53);
            if (country != 0) {
                value = D(country);
                target = D(0x009cf430);
                if (value == target) {
                    competition = D(club + 0x57);
                    if (competition != 0) {
                        value = D(competition);
                        target = D(0x009cf868);
                        if (value == target) {
                            index = (u32)(s32)(s16)count;
                            ++count;
                            D(allocation + index * 4u) = club;
                            competition = D(club + 0x57);
                            D(club + 0x5b) = competition;
                        }
                    }
                }
            }
            competition = D(club + 0x57);
            ++scanned;
            D((u32)scratch + 4) = competition;
            total = D(0x00ae235c);
            club += 0x245;
        } while ((s32)scanned < (s32)total);
        if ((s16)count >= 4) {
            cm_fn_004afbd0(allocation, count, 4);
            for (index = 0; index != 4; ++index) {
                id = D(0x009cf85c);
                base = D(0x00ae23d0);
                club = D(allocation + index * 4u);
                cm_fn_006830b0(club, base + id * 107u, 1);
            }
            group = D((u32)scratch);
            end = (u32)(s32)(s16)W(group + 0x3e);
            adjustment = (u32)(s32)(s8)B(group + 0xc1);
            index = end - adjustment;
            while ((s32)index < (s32)end) {
                id = D(0x009cf868);
                base = D(0x00ae23d0);
                competition = base + id * 107u;
                row = cm_fn_006826d0(group, index);
                club = D(row);
                /* competition and 1 were pushed before 006826d0; that
                   RET4 consumes only index. 006831a0 then pops all three. */
                cm_fn_006831a0(club, competition, 1);
                ++index;
                if ((s32)index < (s32)end)
                    group = D((u32)scratch);
            }
        }
    }
    return (u32)cm_fn_009452ca(allocation);
}
