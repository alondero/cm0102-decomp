#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern void CM_THISCALL cm_fn_00683010(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008fddc0(u32 self)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 year, nations, club, total, limit, scanned, cursor, competition;
    u32 id, target, value, base, count, allocation, text;
    u8 selected, argument;
    selected = 0;
    year = W(self + 0x40);
    W(self + 0x3e) = 18;
    if (year == 2001) {
        nations = D(0x00ae2350);
        club = D(0x00ae23b4);
        total = D(0x00ae235c);
        limit = total - 2u * nations;
        scanned = 0;
        if ((s32)limit > 0) {
            cursor = club + 0x57;
            do {
                competition = D(cursor);
                if (competition != 0) {
                    id = D(competition);
                    target = D(0x009cf8e4);
                    if (id == target || id == D(0x009cf8e8) ||
                        id == D(0x009cf8ec) || id == D(0x009cf8f0) || id == D(0x009cf8f4)) {
                        value = D(cursor);
                        D(cursor + 9) = value;
                        id = D(0x009cf8e0);
                        base = D(0x00ae23d0);
                        D(cursor) = base + id * 107u;
                    }
                }
                nations = D(0x00ae2350);
                ++scanned;
                cursor += 581;
                total = D(0x00ae235c);
                limit = total - 2u * nations;
            } while ((s32)scanned < (s32)limit);
        }
    }
    count = (u32)(s32)(s16)W(self + 0x3e);
    allocation = (u32)cm_fn_00944e46(count * 59u);
    D(self + 0xb1) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x00aa1840, 0, 0, local + 0x100, local);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x100, local, 0x1a4);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    scanned = 0;
    if ((s32)total > 0) {
        do {
            competition = D(club + 0x57);
            if (competition != 0) {
                id = D(competition);
                target = D(0x009cf8d8);
                if (id == target) {
                    /* The accepted callee consumes the index as a BYTE. */
                    argument = selected;
                    ++selected;
                    cm_fn_00687430(self, argument, club, 0, 0);
                    if ((s16)(s8)selected == (s16)W(self + 0x3e))
                        break;
                }
            }
            total = D(0x00ae235c);
            ++scanned;
            club += 581;
        } while ((s32)scanned < (s32)total);
    }
    if ((s16)(s8)selected != (s16)W(self + 0x3e))
        return 0;
    D(self + 0x28) = 0;
    B(self + 0xe1) = 2;
    cm_fn_00683010(self, 1000000, 0);
    return 1;
}
