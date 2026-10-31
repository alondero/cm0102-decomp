/* Preferred-image VA 005bef10, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern void CM_THISCALL cm_fn_00683010(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005bef10(u32 self)
{
    u8 scratch[0x200];
    W(self + 0x3e) = 20;
    u8 found = 0;
    u32 allocation = (u32)cm_fn_00944e46(0x49c);
    D(self + 0xb1) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x009ccc0c, 0, 0, scratch + 0x100, scratch);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x100, scratch, 0x171);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    u32 count = D(0x00ae235c);
    u32 club = D(0x00ae23b4);
    u32 scanned = 0;
    if ((s32)count > 0) {
        do {
            u32 competition = D(club + 0x57);
            if (competition) {
                u32 id = D(competition);
                u32 target = D(0x009cf5d0);
                if (id == target) {
                    u8 index = found;
                    found = (u8)(found + 1);
                    cm_fn_00687430(self, index, club, 0, 0);
                    if ((u16)(s16)(s8)found == W(self + 0x3e)) break;
                }
            }
            count = D(0x00ae235c);
            ++scanned;
            club += 0x245;
        } while ((s32)(s16)scanned < (s32)count);
    }
    if ((u16)(s16)(s8)found != W(self + 0x3e)) return 0;
    D(self + 0x28) = 0;
    B(self + 0xe1) = 2;
    u32 id = D(0x009cf5cc);
    u32 base = D(0x00ae23d0);
    cm_fn_00683010(self, 0x1e8480, base + id * 107u);
    id = D(0x009cf5d0);
    base = D(0x00ae23d0);
    cm_fn_00683010(self, 0x16e360, base + id * 107u);
    id = D(0x009cf5d4);
    base = D(0x00ae23d0);
    cm_fn_00683010(self, 0x16e360, base + id * 107u);
    return 1;
}
