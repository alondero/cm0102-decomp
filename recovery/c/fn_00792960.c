#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_CDECL cm_fn_004afbd0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006830b0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006831a0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32, u32);

u32 CM_FASTCALL cm_fn_00792960(u32 self)
{
    u8 scratch[0x208];
    u32 local = (u32)scratch;
    u32 table, key, competition, total, entries, text, i, count;
    u32 person, nation, club, id, wanted, j, matched, pointer, target;
    table = D(0x00adadfc);
    key = D(0x009cf6c0);
    D(local + 4) = self;
    competition = D(table + key * 4);
    total = D(0x00ae235c);
    D(local) = competition;
    entries = (u32)cm_fn_00944e46(total * 4);
    if (entries == 0) {
        cm_fn_00944cff(0x00a6977c, 0, 0, local + 0x108, local + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      local + 0x108, local + 8, 0x24b);
        text = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return text;
    }
    total = D(0x00ae235c);
    person = D(0x00ae23b4);
    i = 0;
    count = 0;
    if ((s32)total > 0) {
        do {
            nation = D(person + 0x57);
            if (nation != 0) {
                club = D(person + 0x53);
                if (club != 0) {
                    id = D(nation);
                    matched = 0;
                    for (j = 0; j < 8; ++j) {
                        wanted = D(0x009cf6c4 + j * 4);
                        if (id == wanted) {
                            matched = 1;
                            break;
                        }
                    }
                    if (matched) {
                        id = D(club);
                        wanted = D(0x009cf408);
                        if (id == wanted) {
                            pointer = (u32)(s32)(s16)count;
                            ++count;
                            D(entries + pointer * 4) = person;
                        }
                    }
                }
            }
            total = D(0x00ae235c);
            ++i;
            person += 0x245;
        } while ((s32)i < (s32)total);
        if ((u16)count != 0) {
            cm_fn_004afbd0(entries, count, 4);
            pointer = entries;
            for (i = 4; i != 0; --i) {
                key = D(0x009cf6c0);
                person = D(pointer);
                target = key * 107;
                table = D(0x00ae23d0);
                cm_fn_006830b0(person, target + table, 1);
                pointer += 4;
            }
            competition = D(local);
            i = 0;
            if ((s16)W(competition + 0x3e) > 0) {
                do {
                    competition = D(local);
                    person = cm_fn_006826d0(competition, i);
                    if (B(person + 0x37) == 3) {
                        key = D(0x009cf6c4);
                        target = key * 107;
                        table = D(0x00ae23d0);
                        target += table;
                        person = cm_fn_006826d0(competition, i);
                        person = D(person);
                        (void)D(local + 4); /* original ECX reload; callee overwrites it */
                        cm_fn_006831a0(person, target, 1);
                    }
                    total = (u32)(s32)(s16)W(competition + 0x3e);
                    ++i;
                } while ((s32)i < (s32)total);
            }
        }
    }
    return (u32)cm_fn_009452ca(entries);
}
