/* Preferred-image VA 007d1520, base 00400000; CM0102 3.9.68.
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
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00682f70(u32, u32);

u32 CM_THISCALL cm_fn_007d1520(u32 self)
{
    u8 scratch[0x208];
    u8 found = 0;
    W(self + 0x3e) = 18;
    B((u32)scratch) = 0;
    u32 allocation = (u32)cm_fn_00944e46(0x426);
    D(self + 0xb1) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x00a7875c, 0, 0, scratch + 0x108, scratch + 8);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x108, scratch + 8, 0x16c);
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
                u32 number = D(competition);
                u32 target = D(0x009cf610);
                if (number == target) {
                    cm_fn_00687430(self, found, club, 0, 0);
                    u32 other = cm_fn_00540a50(club, (u32)scratch + 4, 1);
                    if (other && D((u32)scratch + 4) == 0) {
                        u32 other_competition = D(other + 0x57);
                        if (other_competition) {
                            u32 other_number = D(other_competition);
                            u32 other_target = D(0x009cf60c);
                            if (other_number == other_target) {
                                u32 position = (u32)(s32)(s8)found * 59u;
                                u32 table = D(self + 0xb1);
                                B(table + position + 0x37) = 0xfc;
                            }
                        }
                    }
                    found = (u8)(found + 1);
                    u16 limit = W(self + 0x3e);
                    B((u32)scratch) = found;
                    if ((u16)(s16)(s8)found == limit) break;
                }
            }
            count = D(0x00ae235c);
            ++scanned;
            club += 0x245;
        } while ((s32)(s16)scanned < (s32)count);
    }
    if ((u16)(s16)(s8)found != W(self + 0x3e)) return 0;
    u32 value = cm_fn_00682f70(self, 0xc350);
    D(self + 0x28) = value;
    B(self + 0xe1) = 1;
    return 1;
}
