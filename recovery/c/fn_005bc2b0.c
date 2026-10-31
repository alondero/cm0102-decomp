/* Preferred-image VA 005bc2b0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_THISCALL cm_fn_00687430(u32 self, u32 index, u32 club, u32 flag, u32 base);
extern u32 CM_THISCALL cm_fn_00682f70(u32 self, u32 amount);
extern void CM_THISCALL cm_fn_00683010(u32 self, u32 amount, u32 zero);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005bc2b0(u32 self)
{
    u8 work[0x200];
    u8 selected = 0;
    u32 allocation, version, total, club, pointer, id, expected, raw_index, scanned = 0;
    if (W(self + 0x40u) == 0x7d1u)
        W(self + 0x3eu) = 18;
    else
        W(self + 0x3eu) = 20;
    total = (u32)(s32)(s16)W(self + 0x3eu);
    W(self + 0xa7u) = 0xffff;
    allocation = (u32)cm_fn_00944e46(total * 59u);
    D(self + 0xb1u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009ccb5cu, 0, 0, work, work + 0x100);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work, work + 0x100, 0x1d3u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    total = D(0x00ae235cu);
    club = D(0x00ae23b4u);
    if ((s32)total > 0) {
        do {
            pointer = D(club + 0x57u);
            if (pointer != 0) {
                id = D(pointer);
                expected = D(0x009cf5ccu);
                if (id == expected) {
                    raw_index = (expected & 0xffffff00u) | selected;
                    selected = (u8)(selected + 1u);
                    cm_fn_00687430(self, raw_index, club, 0, 0);
                    if ((s16)(s8)selected == (s16)W(self + 0x3eu))
                        break;
                }
            }
            total = D(0x00ae235cu);
            ++scanned;
            club += 581u;
        } while ((s32)(s16)scanned < (s32)total);
    }
    if ((s16)(s8)selected != (s16)W(self + 0x3eu))
        return 0;
    allocation = cm_fn_00682f70(self, 200000);
    D(self + 0x28u) = allocation;
    B(self + 0xe1u) = 1;
    cm_fn_00683010(self, 6000000, 0);
    return 1;
}
