#include "cmre.h"
/* Preferred-image VA 0x00515cb0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The original 00687430 reads only BYTE[entry ESP+4] for the index
   (006874fb), then sign-extends AL at 00687509. Pushed upper bytes are dead.
   Scan and selected indices deliberately retain their signed low-word/byte
   comparisons and wrapping increments. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern u32 CM_THISCALL cm_fn_00682f70(u32, u32);
extern void CM_THISCALL cm_fn_00683010(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_THISCALL cm_fn_00515cb0(u32 self)
{
    volatile u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 allocated, count, club, profile, country, wanted, scan = 0, text;
    u8 selected = 0, argument;
    W(self + 0x3eu) = 16;
    allocated = (u32)cm_fn_00944e46(0x3b0);
    D(self + 0xb1u) = allocated;
    if (allocated == 0) {
        cm_fn_00944cff(0x009b53f8, 0, 0, local + 0x100u, local);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x100u, local, 0x145);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    count = D(0x00ae235c);
    club = D(0x00ae23b4);
    if ((s32)count > 0) {
        do {
            profile = D(club + 0x57u);
            if (profile != 0) {
                country = D(profile);
                wanted = D(0x009cf8a8);
                if (country == wanted) {
                    argument = selected;
                    selected = (u8)(selected + 1u);
                    cm_fn_00687430(self, argument, club, 0, 0);
                    if ((u16)(s16)(s8)selected == W(self + 0x3eu)) break;
                }
            }
            count = D(0x00ae235c);
            ++scan;
            club += 0x245u;
        } while ((s32)(s16)(u16)scan < (s32)count);
    }
    if ((u16)(s16)(s8)selected != W(self + 0x3eu)) return 0;
    allocated = cm_fn_00682f70(self, 0x2710);
    D(self + 0x28u) = allocated;
    *(volatile u8 *)(self + 0xe1u) = 1;
    cm_fn_00683010(self, 0x4e20, 0);
    return 1;
}
