/* Preferred-image VA 0057e7e0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_0057d9d0(u32 self, u8 code);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_THISCALL cm_fn_00687430(u32 self, u8 index, u32 club, u8 flag, u32 base);
extern void CM_THISCALL cm_fn_00684230(u32 self);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0057e7e0(u32 self)
{
    u8 work[0x204];
    u32 count, allocation, table, club, argument, text;
    u16 capacity, multiplier;
    u8 index;
    s16 limit;
    count = cm_fn_0057d9d0(self, 0xffu);
    W(self + 0x3eu) = (u16)count;
    allocation = (u32)cm_fn_00944e46((u32)(s32)(s16)count * 59u);
    D(self + 0xb1u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009c4674u, 0, 0, work + 0x104, work + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x104, work + 4, 0x2d2u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    capacity = W(self + 0x3eu);
    index = 0;
    *(volatile u8 *)work = index;
    multiplier = W(self + 0x3cu);
    W(self + 0xa7u) = (u16)(((u32)capacity - 1u) * (u32)multiplier);
    if ((s16)capacity > 0) {
        do {
            table = D(self + 0xf7u);
            club = D(table + (u32)(s32)(s8)index * 4u);
            /* Preserve the original DWORD read after only a byte store. */
            argument = *(volatile u32 *)work;
            cm_fn_00687430(self, (u8)argument, club, 0, 0);
            index = (u8)(index + 1u);
            limit = (s16)W(self + 0x3eu);
            *(volatile u8 *)work = index;
        } while ((s16)(s8)index < limit);
    }
    cm_fn_00684230(self);
    return 1;
}
