/* Preferred-image VA 00920eb0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   MOVSX DX at00920f10 preserves upper EDX, but SUB's upper result is
   discarded by WORD store00920f23 and MOVSX EAX,AX at00920f27. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u8 CM_THISCALL cm_fn_005fa730(u32, u32, s16, u32 *, u32 *, u32);
extern u32 CM_CDECL cm_fn_004afd80(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00920eb0(u32 self)
{
    u8 scratch[0x20c];
    u16 year = W(self + 0x40);
    D((u32)scratch) = 0xffffffffu;
    D((u32)scratch + 4) = 0xffffffffu;
    u32 competition = D(0x009cf79c);
    u32 manager = D(0x00b5e134);
    u8 found = 0;
    cm_fn_005fa730(manager, competition, (s16)(u16)(year + 2),
                   (u32 *)scratch, (u32 *)(scratch + 4), 1);
    competition = D(0x009cf79c);
    u32 base = D(0x00ae23d0);
    u32 omitted = cm_fn_004afd80(base + competition * 107u, 0);
    s32 delta = (s8)B(self + 0xee);
    D((u32)scratch + 8) = omitted;
    u16 count_word = (u16)(10 - delta);
    W(self + 0x3e) = count_word;
    u32 count = (u32)(s32)(s16)count_word;
    u32 allocation = (u32)cm_fn_00944e46(count * 59u);
    D(self + 0xb1) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x00ad4ce0, 0, 0, scratch + 0x10c, scratch + 0xc);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x10c, scratch + 0xc, 0x204);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    u32 total = D(0x00ae2350);
    u32 nation = D(0x00ae23a8);
    u32 scanned = 0;
    if ((s32)total > 0) {
        do {
            u32 region = D(nation + 0x71);
            if (region) {
                u32 id = D(region);
                u32 target = D(0x009cfa1c);
                if (id == target && (s16)(s8)found < (s16)W(self + 0x3e)) {
                    u32 club = cm_fn_0053b3d0(nation);
                    id = D(nation);
                    u32 a = D((u32)scratch);
                    if (id != a && id != D((u32)scratch + 4) &&
                        club != D((u32)scratch + 8)) {
                        u8 index = found;
                        found = (u8)(found + 1);
                        cm_fn_00687430(self, index, club, 0, 0);
                    }
                }
            }
            total = D(0x00ae2350);
            ++scanned;
            nation += 0x122;
        } while ((s32)(s16)scanned < (s32)total);
    }
    return (u16)(s16)(s8)found == W(self + 0x3e);
}
