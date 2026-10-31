/* Preferred-image VA 009002c0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_009002c0(u32 self)
{
    u8 scratch[0x204];
    u8 found = 0;
    W(self + 0x3e) = 10;
    B((u32)scratch) = 0;
    u32 allocation = (u32)cm_fn_00944e46(0x24e);
    D(self + 0xb1) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x00aa18b8, 0, 0, scratch + 0x104, scratch + 4);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x104, scratch + 4, 0x1c4);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    u32 nations = D(0x00ae2350);
    u32 clubs = D(0x00ae235c);
    u32 club = D(0x00ae23b4);
    u32 scanned = 0;
    if ((s32)(clubs - nations * 2u) > 0) {
        do {
            u32 competition = D(club + 0x57);
            if (competition) {
                u32 group = D(club + 0x60);
                if (group) {
                    u32 group_id = D(group);
                    u32 group_target = D(0x009cf8e4);
                    if (group_id == group_target) {
                        u32 competition_id = D(competition);
                        u32 competition_target = D(0x009cf8e0);
                        if (competition_id == competition_target) {
                            if ((s16)(s8)found >= (s16)W(self + 0x3e)) goto overflow;
                            u8 index = (u8)D((u32)scratch);
                            cm_fn_00687430(self, index, club, 0, 0);
                            found = (u8)(found + 1);
                            B((u32)scratch) = found;
                        }
                    }
                }
            }
            nations = D(0x00ae2350);
            clubs = D(0x00ae235c);
            ++scanned;
            club += 0x245;
        } while ((s32)scanned < (s32)(clubs - nations * 2u));
    }
    goto done;
overflow:
    cm_fn_00944cff(0x00aa18b8, 0, 0, scratch + 4, scratch + 0x104);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                   scratch + 4, scratch + 0x104, 0x1d5);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    D(club + 0x57) = 0;
done:
    return (u16)(s16)(s8)found == W(self + 0x3e);
}
