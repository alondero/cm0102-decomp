/* Preferred-image VA 0058bcb0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 size, u32 comparator);
extern u32 CM_THISCALL cm_fn_00687430(u32 self, u8 index, u32 club, u8 flag, u32 base);
extern void CM_THISCALL cm_fn_00684230(u32 self);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0058bcb0(u32 self)
{
    u8 scratch[0x204];
    u32 allocation, count, table, club, argument, text;
    u8 index;
    s16 limit;
    W(self + 0x3eu) = 4;
    allocation = (u32)cm_fn_00944e46(0xecu);
    D(self + 0xb1u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009c7474u, 0, 0, scratch + 0x104, scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, scratch + 0x104, scratch + 4, 0x1c0u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    count = D(self + 0xf2u);
    table = D(self + 0xeeu);
    cm_fn_00945333(table, count, 4, 0x004b07d0u);
    index = 0;
    limit = (s16)W(self + 0x3eu);
    *(volatile u8 *)scratch = index;
    if (limit > 0) {
        do {
            table = D(self + 0xeeu);
            club = D(table + (u32)(s32)(s8)index * 8u);
            /* Only the low byte was initialized; retain the native DWORD read. */
            argument = *(volatile u32 *)scratch;
            cm_fn_00687430(self, (u8)argument, club, 0, 0);
            index = (u8)(index + 1u);
            limit = (s16)W(self + 0x3eu);
            *(volatile u8 *)scratch = index;
        } while ((s16)(s8)index < limit);
    }
    cm_fn_00684230(self);
    return 1;
}
