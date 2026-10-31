/* Preferred-image VA 00924ab0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

static void store(u32 self, u32 offset, u32 club)
{
    u32 table = D(self + 0xa7);
    D(table + offset) = club;
    table = D(self + 0xa7);
    B(table + offset + 4) = 0;
    table = D(self + 0xa7);
    B(table + offset + 5) = 0;
}

u32 CM_THISCALL cm_fn_00924ab0(u32 self)
{
    u8 scratch[0x204];
    u32 found = 0;
    W(self + 0x3e) = 32;
    u32 allocation = (u32)cm_fn_00944e46(0xc0);
    D(self + 0xa7) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x00ad5684, 0, 0, scratch + 0x104, scratch + 4);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x104, scratch + 4, 0x11b);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    u32 total = D(0x00ae235c);
    u32 club = D(0x00ae23b4);
    D((u32)scratch) = 0;
    if ((s32)total > 0) {
        u32 offset = 0;
        do {
            u32 competition = D(club + 0x57);
            if (competition) {
                u32 nation = D(club + 0x53);
                if (nation) {
                    u32 id = D(competition);
                    if (id == D(0x009cf960)) {
                        id = D(nation);
                        u32 target = D(0x009cf510);
                        if (id == target && (s32)offset < 0x54) {
                            ++found;
                            store(self, offset, club);
                            offset += 6;
                        }
                    }
                }
            }
            u32 scanned = D((u32)scratch);
            club += 0x245;
            ++scanned;
            D((u32)scratch) = scanned;
            total = D(0x00ae235c);
            if ((s32)(s16)scanned >= (s32)total) break;
        } while (1);
    }
    club = D(0x00ae23b4);
    u32 scanned = 0;
    if ((s32)total > 0) {
        u32 offset = found * 6u;
        do {
            u32 competition = D(club + 0x57);
            if (competition) {
                u32 id = D(competition);
                if (id == D(0x009cf94c) && (s32)found < (s16)W(self + 0x3e)) {
                    ++found;
                    store(self, offset, club);
                    offset += 6;
                }
            }
            ++scanned;
            club += 0x245;
            D((u32)scratch) = scanned;
            total = D(0x00ae235c);
            if ((s32)(s16)scanned >= (s32)total) break;
            scanned = D((u32)scratch);
        } while (1);
    }
    return found == (u32)(s32)(s16)W(self + 0x3e);
}
