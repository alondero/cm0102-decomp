/* WIP: cleanup file-enumeration events mismatch, case 203 at both optimization levels.
   Preferred-image VA 00796fd0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern u32 CM_THISCALL cm_fn_007979b0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00796fd0(u32 self, u32 year, u32 competition)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095eaa9;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_00682200(self);
    frame.state = 0;
    D(self) = 0x0096e018;
    W(self + 0x40) = (u16)year;
    D(self + 4) = competition;
    B(self + 0x42) = 2;
    D(self + 0x1c) = 0xffffffffu;
    D(self + 0x20) = 0xffffffffu;
    if (((u16)year & 1) == 0) {
        u16 current = W(self + 0x40);
        if ((s16)current < 0x7d1) W(self + 0x40) = (u16)(current + 1);
    }
    B(self + 0x50) = 0x23;
    B(self + 0x51) = 0x0f;
    if (!cm_fn_00687b10(self, 1)) {
        B(self + 0x44) = 0xff;
        D(self + 0x30) = 0xffffffffu;
        D(self + 0x2c) = 2;
        u32 allocation = (u32)cm_fn_00944e46(8);
        D(self + 0xc) = allocation;
        D(self + 0x45) = 0;
        allocation = (u32)cm_fn_00944cf1(0x5ce);
        B((u32)&frame.state) = 1;
        u32 child = 0;
        if (allocation) child = cm_fn_0049ee70(allocation, self);
        B((u32)&frame.state) = 0;
        D(self + 8) = child;
        if (!child) {
            cm_fn_00944cff(0x00a6a068, 0, 0, scratch + 0x108, scratch + 8);
            u32 text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                           scratch + 0x108, scratch + 8, 0x51);
            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
        } else {
            cm_fn_007979b0(self);
        }
    }
    CM_FS(u32, 0) = frame.prev;
    return self;
}
