/* Preferred-image VA 0078ac90, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The final twelve source slots remain uninitialized where the original
   did not collect them. No minimum-count guard or slot defaults are added. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_004afbd0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0078ac90(u32 self)
{
    u8 scratch[0x27c];
    D((u32)scratch) = 0;
    W(self + 0x3e) = 32;
    u32 allocation = (u32)cm_fn_00944e46(0xc0);
    D(self + 0xa7) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x00a69628, 0, 0, scratch + 0x17c, scratch + 0x7c);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x17c, scratch + 0x7c, 0x11d);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    u32 nations = D(0x00ae2350);
    u32 club = D(0x00ae23b4);
    u32 clubs = D(0x00ae235c);
    u32 scanned = 0;
    if ((s32)(clubs - nations * 2u) > 0) {
        u32 offset = 0;
        do {
            u32 competition = D(club + 0x57);
            if (competition) {
                if ((s16)W((u32)scratch) < 30) {
                    u32 id = D(competition);
                    if (id == D(0x009cf8b8)) {
                        u32 count = D((u32)scratch);
                        u32 index = (u32)(s32)(s16)count;
                        ++count;
                        D((u32)scratch + index * 4u + 4) = club;
                        D((u32)scratch) = count;
                        goto next;
                    }
                }
                u32 id = D(competition);
                u32 target = D(0x009cf8b0);
                if (id == target || id == D(0x009cf8b4)) {
                    u32 table = D(self + 0xa7);
                    offset += 6;
                    D(table + offset + 0x42) = club;
                    table = D(self + 0xa7);
                    B(table + offset + 0x47) = 0;
                    table = D(self + 0xa7);
                    B(table + offset + 0x46) = 1;
                }
            }
next:
            nations = D(0x00ae2350);
            clubs = D(0x00ae235c);
            ++scanned;
            club += 0x245;
        } while ((s32)scanned < (s32)(clubs - nations * 2u));
    }
    u32 count = D((u32)scratch);
    cm_fn_004afbd0((u32)scratch + 4, count, 12);
    u32 source = (u32)scratch + 4;
    for (u32 offset = 0; offset < 0x48; offset += 6) {
        u32 table = D(self + 0xa7);
        u32 seed = D(source);
        source += 4;
        D(table + offset) = seed;
        table = D(self + 0xa7);
        B(table + offset + 5) = 0;
        table = D(self + 0xa7);
        B(table + offset + 4) = 2;
    }
    return 1;
}
