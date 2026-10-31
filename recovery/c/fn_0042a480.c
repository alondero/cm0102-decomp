#include "cmre.h"

/* Preferred-image VA 0x0042a480, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve the byte selection count, signed WORD traversal counter, and
   original record setter/reset boundaries. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern void CM_THISCALL cm_fn_00683010(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_FASTCALL cm_fn_0042a480(u32 self)
{
    u8 scratch[512];
    u32 allocation, club, value, other, total, counter = 0;
    u8 selected = 0, index;
    char *version;
    W(self + 0x3e) = 28;
    allocation = (u32)cm_fn_00944e46(0x674);
    D(self + 0xb1) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x0098aa30, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x1a1);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    if ((s32)total > 0) {
        do {
            value = D(club + 0x57);
            if (value) {
                value = D(value);
                other = D(0x009cf9ec);
                if (value == other) {
                    index = selected;
                    ++selected;
                    cm_fn_00687430(self, index, club, 0, 0);
                    if ((u16)(s16)(s8)selected == W(self + 0x3e)) break;
                }
            }
            total = D(0x00ae235c);
            ++counter;
            club += 581;
        } while ((s32)(s16)counter < (s32)total);
    }
    if ((u16)(s16)(s8)selected != W(self + 0x3e)) return 0;
    cm_fn_00683010(self, 0x249f0, 0);
    return 1;
}
