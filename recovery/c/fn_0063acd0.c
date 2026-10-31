#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32, u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0063acd0(u32 self, u32 year, u32 competition)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 previous, vtable, value, allocated, child, text;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095b629;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    D((u32)scratch + 4) = self;
    cm_fn_00682200(self);
    D(self + 4) = competition;
    frame.state = 0;
    D(self) = 0x0096c0c8;
    W(self + 0x40) = (u16)year;
    B(self + 0x49) = 7;
    B(self + 0x4a) = 5;
    B(self + 0x50) = 0x10;
    if (cm_fn_00687b10(self, 1) == 0) {
        B(self + 0xc4) = 2;
        B(self + 0xc7) = 2;
        B(self + 0xc2) = 3;
        B(self + 0xc8) = 3;
        vtable = D(self);
        B(self + 0x44) = 0xff;
        D(self + 0x30) = 0xffffffffu;
        D(self + 0x2c) = 0;
        D(self + 0xc) = 0;
        W(self + 0x3c) = 1;
        B(self + 0xc3) = 1;
        B(self + 0x42) = 1;
        B(self + 0xc5) = 4;
        B(self + 0xc6) = 1;
        B(self + 0xbe) = 0;
        B(self + 0xbf) = 0;
        B(self + 0xc0) = 0;
        B(self + 0xc1) = 0;
        D(self + 0x1c) = 0xffffffffu;
        D(self + 0x20) = 0xffffffffu;
        value = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))D(vtable + 0x3c))
            (self, 0xffffffffu, self + 0xa9, self + 0x3a, 0);
        D(self + 0xba) = value;
        W(self + 0x3e) = 0;
        D(self + 0xb1) = 0;
        D(self + 0x45) = 0;
        allocated = (u32)cm_fn_00944cf1(0x5ce);
        D((u32)scratch) = allocated;
        B((u32)&frame.state) = 1;
        child = allocated != 0 ? cm_fn_0049ee70(allocated, self) : 0;
        B((u32)&frame.state) = 0;
        D(self + 8) = child;
        if (child == 0) {
            cm_fn_00944cff(0x009ea0b8, 0, 0, (u32)scratch + 0x108, (u32)scratch + 8);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x108, (u32)scratch + 8, 0x51);
            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
        }
    }
    CM_FS(u32, 0) = frame.prev;
    return self;
}
