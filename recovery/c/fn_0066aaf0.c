#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_004afbd0(u32, s16, s16);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0066aaf0(u32 self)
{
    u8 scratch[0x394];
    u32 local = (u32)scratch;
    u32 allocation, text, nations, club, limit, scanned, nation, competition;
    u32 id, owner, index, cursor, target, value, count, total;
    u8 selected;
    int same;
    selected = 0;
    B(local + 3) = 0;
    W(self + 0x3e) = 32;
    allocation = (u32)cm_fn_00944e46(0xc0);
    D(self + 0xa7) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009eb0e8, 0, 0, local + 0x104, local + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
            local + 0x104, local + 4, 0x11c);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    nations = D(0x00ae2350);
    club = D(0x00ae23b4);
    limit = D(0x00ae235c) - 2u * nations;
    scanned = 0;
    if ((s32)limit > 0) {
        do {
            nation = D(club + 0x53);
            if (nation != 0) {
                competition = D(club + 0x57);
                if (competition != 0) {
                    id = D(0x009cfa00);
                    same = D(competition) == id;
                    selected = B(local + 3);
                    if (same)
                        goto next_club;
                }
                owner = D(self + 4);
                if (nation == D(owner + 0x5d) && (s8)selected < 100) {
                    index = (u32)(s32)(s8)selected;
                    ++selected;
                    D(local + 0x204 + index * 4u) = club;
                    B(local + 3) = selected;
                }
            }
next_club:
            ++scanned;
            club += 581;
        } while ((s32)(s16)scanned < (s32)limit);
    }
    /* The helper's count is consumed as SI, then sign-extended at 004afc07
       and 004afc29; the incoming upper half of EDX is never forwarded. */
    cm_fn_004afbd0(local + 0x204, (s16)(s8)selected, 22);
    count = 22;
    index = 0;
    cursor = local + 0x204;
    do {
        target = D(self + 0xa7);
        value = D(cursor);
        cursor += 4;
        D(target + index) = value;
        target = D(self + 0xa7);
        B(target + index + 4) = 0;
        target = D(self + 0xa7);
        B(target + index + 5) = 0;
        index += 6;
    } while (--count != 0);
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    scanned = 0;
    count = 22;
    if ((s32)total > 0) {
        index = 0x84;
        do {
            competition = D(club + 0x57);
            if (competition != 0) {
                id = D(competition);
                if (id == D(0x009cfa00)) {
                    value = (u32)(s32)(s16)W(self + 0x3e);
                    if ((s32)count < (s32)value) {
                        target = D(self + 0xa7);
                        ++count;
                        D(target + index) = club;
                        target = D(self + 0xa7);
                        B(target + index + 4) = 0;
                        target = D(self + 0xa7);
                        B(target + index + 5) = 0;
                        index += 6;
                    }
                }
            }
            ++scanned;
            club += 581;
            total = D(0x00ae235c);
        } while ((s32)(s16)scanned < (s32)total);
    }
    value = (u32)(s32)(s16)W(self + 0x3e);
    return count == value;
}
