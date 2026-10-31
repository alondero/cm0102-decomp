#include "cmre.h"

/* Preferred-image VA 0x00514ed0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_004afbd0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006830b0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006831a0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00514ed0(u32 self)
{
    u8 scratch[0x20c];
    u32 local = (u32)scratch, leagues, key, league, second_league, value, total;
    u32 allocated, row, scan = 0, count = 0, id, other, nation, competition;
    u32 cursor, remaining, target, table, entrant, index, limit, text;
    /* Original ECX is forwarded to 6830b0/6831a0, which overwrite it
       before use; both boundaries have three physical stack arguments. */
    (void)self;
    leagues = D(0x00adadfc);
    key = D(0x009cf8a8);
    league = D(leagues + key * 4u);
    key = D(0x009cf8ac);
    D(local) = league;
    second_league = D(leagues + key * 4u);
    total = D(0x00ae235c);
    D(local + 4) = second_league;
    allocated = cm_fn_00944e46(total * 4u);
    D(local + 8) = allocated;
    if (allocated == 0) {
        cm_fn_00944cff(0x009b53c0, 0, 0, local + 0x10c, local + 12);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x10c, local + 12, 0x2d2);
        value = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return value;
    }
    total = D(0x00ae235c);
    row = D(0x00ae23b4);
    if ((s32)total <= 0) goto done;
    do {
        nation = D(row + 0x53);
        if (nation != 0) {
            id = D(nation);
            other = D(0x009cf2ac);
            if (id == other) {
                competition = D(row + 0x57);
                if (competition != 0) {
                    id = D(competition);
                    other = D(0x009cf764);
                    if (id == other || id == D(0x009cf888) || id == D(0x009cf890) ||
                        id == D(0x009cf88c) || id == D(0x009cf894)) {
                        index = (u32)(s32)(s16)count;
                        ++count;
                        D(allocated + index * 4u) = row;
                    }
                }
            }
        }
        total = D(0x00ae235c);
        ++scan;
        row += 581;
    } while ((s32)scan < (s32)total);
    if ((s16)count < 4) goto done;
    cm_fn_004afbd0(allocated, count, 4);
    cursor = allocated;
    remaining = 4;
    do {
        key = D(0x009cf8a8);
        entrant = D(cursor);
        table = D(0x00ae23d0);
        target = table + key * 107u;
        cm_fn_006830b0(entrant, target, 1);
        cursor += 4;
        --remaining;
    } while (remaining != 0);
    league = D(local);
    limit = (u32)(s32)(s16)W(league + 0x3e);
    if ((s32)limit <= 0) goto done;
    index = 0;
    do {
        league = D(local);
        row = cm_fn_006826d0(league, index);
        if (B(row + 0x37) == 3) {
            key = D(0x009cf764);
            table = D(0x00ae23d0);
            target = table + key * 107u;
            league = D(local);
            row = cm_fn_006826d0(league, index);
            entrant = D(row);
            cm_fn_006831a0(entrant, target, 1);
        }
        league = D(local + 4);
        row = cm_fn_006826d0(league, index);
        if (B(row + 0x37) == 3) {
            key = D(0x009cf764);
            table = D(0x00ae23d0);
            target = table + key * 107u;
            league = D(local + 4);
            row = cm_fn_006826d0(league, index);
            entrant = D(row);
            cm_fn_006831a0(entrant, target, 1);
        }
        ++index;
    } while ((s32)index < (s32)limit);
    allocated = D(local + 8);
done:
    return cm_fn_009452ca(allocated);
}
