/* Preferred-image VA 004c16f0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_004c1ed0(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_004c16f0(u32 self, u32 year, u32 competition)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00956989;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_00682200(self);
    D(self + 4) = competition;
    frame.state = 0;
    D(self) = 0x00968b40;
    W(self + 0x40) = (u16)year;
    B(self + 0x50) = 0xff;
    D(self + 0xee) = 0;
    B(self + 0x51) = 0x0f;
    if (!cm_fn_00687b10(self, 1)) {
        W(self + 0xd9) = 40;
        B(self + 0x44) = 0xff;
        D(self + 0x30) = 0xffffffffu;
        D(self + 0x2c) = 4;
        u32 allocation = (u32)cm_fn_00944e46(16);
        D(self + 0xc) = allocation;
        u32 line;
        u8 *name = scratch + 8;
        u8 *extension = scratch + 0x108;
        if (!cm_fn_004c1ed0(self)) {
            line = 0x4d;
            name = scratch + 0x108;
            extension = scratch + 8;
            goto error;
        }
        W(self + 0x3e) = 0;
        D(self + 0xb1) = 0;
        allocation = (u32)cm_fn_00944cf1(0x5ce);
        B((u32)&frame.state) = 1;
        u32 child = 0;
        if (allocation) child = cm_fn_0049ee70(allocation, self);
        B((u32)&frame.state) = 0;
        D(self + 8) = child;
        if (child) { D(self + 0x45) = 0; goto done; }
        line = 0x59;
error:
        cm_fn_00944cff(0x009a7d6c, 0, 0, name, extension);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    }
done:
    CM_FS(u32, 0) = frame.prev;
    return self;
}

