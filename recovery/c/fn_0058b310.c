/* WIP: O2case185 import sequence differs at original stale FindFirstFileA output. */
/* Preferred-image VA 0058b310, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00682200(u32 self);
extern u32 CM_THISCALL cm_fn_00687b10(u32 self, u32 option);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_THISCALL cm_fn_0049ee70(u32 self, u32 owner);
extern u32 CM_THISCALL cm_fn_0058bdb0(u32 self);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0058b310(u32 self, volatile u16 year_argument, volatile u32 competition_argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x208];
    u32 previous, competition, result, allocation, version;
    u16 year;
    frame.state = 0xffffffffu;
    frame.handler = 0x00958d69u;
    previous = CM_FS(volatile u32, 0);
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    D((u32)work + 4u) = self;
    cm_fn_00682200(self);
    year = year_argument;
    competition = competition_argument;
    D(self) = 0x0096a270u;
    frame.state = 0;
    W(self + 0x40u) = year;
    D(self + 4u) = competition;
    B(self + 0x42u) = 4;
    D(self + 0x1cu) = 0xffffffffu;
    D(self + 0x20u) = 0xffffffffu;
    for (;;) {
        year = W(self + 0x40u);
        if ((year & 1u) != 0 && year != 2001)
            break;
        year = (u16)(year + 1u);
        W(self + 0x40u) = year;
    }
    B(self + 0x50u) = 0;
    B(self + 0x51u) = 15;
    result = cm_fn_00687b10(self, 1);
    if (result != 0)
        goto done;
    D(self + 0x45u) = 0;
    B(self + 0x44u) = 0xff;
    D(self + 0x30u) = 0xffffffffu;
    D(self + 0x2cu) = 2;
    allocation = (u32)cm_fn_00944e46(8);
    D(self + 0xcu) = allocation;
    allocation = (u32)cm_fn_00944cf1(0x5ceu);
    D((u32)work) = allocation;
    B((u32)&frame.state) = 1;
    if (allocation != 0)
        result = cm_fn_0049ee70(allocation, self);
    else
        result = 0;
    B((u32)&frame.state) = 0;
    D(self + 8u) = result;
    if (result != 0) {
        cm_fn_0058bdb0(self);
    } else {
        cm_fn_00944cff(0x009c7474u, 0, 0, work + 0x108, work + 8);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x108, work + 8, 0x53u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    }
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return self;
}
