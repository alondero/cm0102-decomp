#include "cmre.h"

/* Preferred-image VA 0x00856cd0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

static void league_error(u32 name, u32 ext, u32 line)
{
    u32 text;
    cm_fn_00944cff(0x00a828f8, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}

u32 CM_THISCALL cm_fn_00856cd0(u32 self)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, row, scan, removed, total, bound;
    u32 competition, nation, id, other, value, owner, raw_index;
    u16 year = W(self + 0x40);
    u8 index = 0;
    W(self + 0x3e) = 20;
    if (year == 2001) {
        removed = D(0x00ae2350);
        total = D(0x00ae235c);
        row = D(0x00ae23b4);
        bound = total - removed * 2u;
        scan = 0;
        if ((s32)bound > 0) {
            row += 0x57;
            do {
                competition = D(row);
                if (competition != 0) {
                    id = D(competition);
                    other = D(0x009cf62c);
                    if (id == other || id == D(0x009cf630) || id == D(0x009cf634) || id == D(0x009cf638)) {
                        competition = D(row);
                        D(row + 9) = competition;
                        owner = D(self + 4);
                        D(row) = owner;
                    }
                }
                removed = D(0x00ae2350);
                total = D(0x00ae235c);
                ++scan;
                bound = total - removed * 2u;
                row += 0x245;
            } while ((s32)scan < (s32)bound);
        }
    }
    value = cm_fn_00944e46((u32)(s32)(s16)W(self + 0x3e) * 59u);
    D(self + 0xb1) = value;
    if (value == 0) {
        league_error(local + 0x100, local, 0x1cf);
        return 0;
    }
    removed = D(0x00ae2350);
    total = D(0x00ae235c);
    row = D(0x00ae23b4);
    bound = total - removed * 2u;
    scan = 0;
    if ((s32)bound > 0) {
        do {
            competition = D(row + 0x57);
            if (competition != 0) {
                nation = D(row + 0x60);
                if (nation != 0) {
                    id = D(nation);
                    other = D(0x009cf62c);
                    if (id == other) {
                        id = D(competition);
                        other = D(0x009cf7b8);
                        if (id == other) {
                            if ((s16)(s8)index >= (s16)W(self + 0x3e)) {
                                league_error(local, local + 0x100, 0x1e0);
                                D(row + 0x57) = 0;
                                goto done;
                            }
                            raw_index = (id & 0xffffff00u) | index;
                            index = (u8)(index + 1);
                            cm_fn_00687430(self, raw_index, row, 0, 0);
                        }
                    }
                }
            }
            removed = D(0x00ae2350);
            total = D(0x00ae235c);
            ++scan;
            row += 0x245;
            bound = total - removed * 2u;
        } while ((s32)scan < (s32)bound);
    }
done:
    return (s16)(s8)index == (s16)W(self + 0x3e);
}
