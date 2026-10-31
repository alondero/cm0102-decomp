#include "cmre.h"
/* Preferred-image VA0040e3b0, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/fullEAX from original free or diagnostic; scratch200 diagnosticsL0/L100.
   SignedWORD count/index and all original reloads retained. Two counted sorts
   are followed by fixed32 reads/calls, even for counts below16. Do not fill
   missing entries or shrink the fixed reads. Preserve qsort incoming EAX. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00945333(u32, u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_004a2540(u32, u32, u8);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0040e3b0(u32 self)
{
    u8 scratch[0x200];
    u32 l = (u32)scratch, pool, eax, p, value, index, offset, target;
    u32 count, output_index, first, second, line, version, rank, row;
    if (D(self + 8) == 0) {
        first = l + 0x100; second = l; line = 0x3db;
        goto error;
    }
    count = (u32)(s32)(s16)W(self + 0x3e);
    eax = (u32)cm_fn_00944e46(count * 8u);
    pool = eax;
    if (pool == 0) {
        first = l; second = l + 0x100; line = 0x3e3;
        goto error;
    }
    index = 0;
    if ((s16)W(self + 0x3e) > 0) {
        do {
            p = D(self + 0xa7);
            offset = (u32)(s32)(s16)(u16)index;
            value = D(p + offset * 6u);
            D(pool + offset * 4u) = value;
            p = D(self + 0xc);
            count = (u32)(s32)(s16)W(self + 0x3e);
            p = D(p);
            output_index = count + offset;
            ++index;
            p = D(p + 0xa7);
            eax = D(p + offset * 6u);
            D(pool + output_index * 4u) = eax;
        } while ((s16)(u16)index < (s16)W(self + 0x3e));
    }
    count = (u32)(s32)(s16)W(self + 0x3e);
    cm_fn_00945333(eax, pool, count, 4, 0x004b07d0);
    eax = (u32)(s32)(s16)W(self + 0x3e);
    p = pool + eax * 4u;
    cm_fn_00945333(eax, p, eax, 4, 0x004b07d0);
    for (row = 0; row < 32; ++row) {
        /* The original repeats the identical16-entry ranking sequence twice. */
        index = row & 15u;
        rank = index < 4 ? index * 2u + 1u : (index < 8 ? 9u : 17u);
        value = D(pool + row * 4u);
        target = D(self + 8);
        cm_fn_004a2540(target, value, (u8)rank);
    }
    return (u32)cm_fn_009452ca(pool);
error:
    cm_fn_00944cff(0x00987b60, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    G(0x00b67a34) = 0;
    return eax;
}
