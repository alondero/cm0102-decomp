/* WIP: one canonical failure. O0 passes; O2 case119 diverges in original
   shutdown file enumeration (FileTime conversions versus FindNextFileA),
   coverage .8468/inc0. Preserve the original helper boundary and scratch. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32, u32);
extern u32 CM_THISCALL cm_fn_00799690(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007988b0(u32 self, u16 year, u32 competition)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 local = (u32)scratch;
    u32 value, name, extension, line, text;
    s32 remainder;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095eb79;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(local + 4) = self;
    cm_fn_00682200(self);
    frame.state = 0;
    D(self) = 0x0096e0cc;
    W(self + 0x40) = year;
    D(self + 4) = competition;
    B(self + 0x42) = 4;
    D(self + 0x1c) = 0xffffffffu;
    D(self + 0x20) = 0xffffffffu;
    B(self + 0x50) = 0;
    if ((s16)year < 2002)
        W(self + 0x40) = 2002;
    remainder = ((s32)(s16)W(self + 0x40) - 1996) % 2;
    while (remainder != 0) {
        W(self + 0x40) = (u16)(W(self + 0x40) + 1u);
        remainder = ((s32)(s16)W(self + 0x40) - 1996) % 2;
    }
    B(self + 0x51) = 15;
    if (cm_fn_00687b10(self, 1) != 0)
        goto done;
    W(self + 0xd9) = 40;
    D(self + 0x45) = 0;
    B(self + 0x44) = 0xff;
    D(self + 0x30) = 0xffffffffu;
    D(self + 0x2c) = 2;
    value = (u32)cm_fn_00944e46(8);
    D(self + 0xc) = value;
    if (cm_fn_00799690(self) == 0) {
        name = local + 0x108;
        extension = local + 8;
        line = 0x57;
        goto error;
    }
    value = (u32)cm_fn_00944cf1(0x5ce);
    D(local) = value;
    *(volatile u8 *)&frame.state = 1;
    if (value != 0)
        value = cm_fn_0049ee70(value, self);
    *(volatile u8 *)&frame.state = 0;
    D(self + 8) = value;
    if (value != 0)
        goto done;
    name = local + 8;
    extension = local + 0x108;
    line = 0x5e;
error:
    cm_fn_00944cff(0x00a6a0ec, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return self;
}
