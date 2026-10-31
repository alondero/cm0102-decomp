#include "cmre.h"

/* Preferred-image VA 00901d00, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch204: allocation L0, diagnostics L4/L104. Numbering advances only
   for nonnull allocations. The original final SEH state remains five. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0092fe80(u32 *, u32, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008af520(void *, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00901d00(u32 self, volatile u32 year_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    u32 l = (u32)scratch, owner, eax, allocation, array, counter = 0;
    u32 year, i, number, priority, filter, version;
    frame.prev = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00965194;
    CM_FS(volatile u32, 0) = (u32)&frame;
    owner = D(self + 0x13);
    W(self + 8) = 6;
    eax = (u32)cm_fn_00944e46(0x18);
    D(self + 4) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x00aa1970, 0, 0, (void *)(l + 0x104), (void *)(l + 4));
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x104, l + 4, 0x64);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    year = 0;
    for (i = 0; i < 6; ++i) {
        allocation = (u32)cm_fn_00944cf1(i < 2 ? 0x2b : 0x25d);
        D(l) = allocation;
        if (i == 0) year = year_arg;
        frame.state = i;
        if (allocation == 0) eax = 0;
        else if (i == 0) {
            counter = 1;
            priority = D(0x009cfec8);
            eax = cm_fn_0092fe80((u32 *)allocation, owner, 0, priority,
                                  0xbb8, 0x5dc, 0xffffffffu, 0, 0, 0, year, 1);
        } else if (i == 1) {
            priority = D(0x009cfecc);
            number = counter;
            ++counter;
            eax = cm_fn_0092fe80((u32 *)allocation, owner, number, priority,
                                  0x7d0, 0x3e8, 0xffffffffu, 0, 0, 0, year, 0);
        } else {
            filter = D(i < 4 ? 0x009cf8d8 : 0x009cf8dc);
            priority = D(0x009cfed0u + (i - 2u) * 4u);
            number = counter;
            if (i != 5) ++counter;
            eax = cm_fn_008af520((void *)allocation, owner, number, priority,
                                  0, 0, filter, 0, 0, year, 1);
        }
        array = D(self + 4);
        if (i != 5) frame.state = 0xffffffffu;
        D(array + i * 4u) = eax;
    }
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
