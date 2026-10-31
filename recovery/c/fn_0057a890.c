#include "cmre.h"

/* Preferred-image VA 0x0057a890, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   WORD year increment wraps. The first diagnostic exits construction;
   the second diagnostic continues to the original trailing stores. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32, u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0057bd80(u32);
extern u32 CM_THISCALL cm_fn_0057b630(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

static void construction_error(u32 name, u32 ext, u32 line)
{
    u32 text;
    cm_fn_00944cff(0x009c4638, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}

u32 CM_THISCALL cm_fn_0057a890(u32 self, u32 first, u32 second)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 local = (u32)scratch, prev, value;
    u16 year;
    s32 difference;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00958789;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    D(local + 4) = self;
    cm_fn_00682200(self);
    D(self + 4) = second;
    year = (u16)(first + 1u);
    frame.state = 0;
    D(self) = 0x0096a020;
    B(self + 0x42) = 4;
    D(self + 0x1c) = 0xffffffffu;
    D(self + 0x20) = 0xffffffffu;
    B(self + 0x4b) = 1;
    B(self + 0x50) = 0;
    W(self + 0x40) = year;
    while ((year & 3u) != 0) {
        year = W(self + 0x40);
        W(self + 0x40) = (u16)(year + 1u);
        year = W(self + 0x40);
    }
    W(self + 0xd9) = 40;
    B(self + 0x51) = 15;
    value = cm_fn_00687b10(self, 1);
    if (value != 0) goto done;
    B(self + 0x44) = 0xff;
    D(self + 0x30) = 0xffffffffu;
    D(self + 0x2c) = 4;
    value = cm_fn_00944e46(16);
    D(self + 12) = value;
    cm_fn_0057bd80(self);
    value = cm_fn_0057b630(self);
    if (value == 0) {
        construction_error(local + 0x108, local + 8, 0x5f);
        goto done;
    }
    value = cm_fn_00944cf1(0x5ce);
    D(local) = value;
    *(volatile u8 *)&frame.state = 1;
    if (value != 0) value = cm_fn_0049ee70(value, self);
    *(volatile u8 *)&frame.state = 0;
    D(self + 8) = value;
    if (value == 0) construction_error(local + 8, local + 0x108, 0x66);
    year = W(self + 0x40);
    difference = (s32)(s16)year - (s32)(s16)first;
    if (difference < 1) W(self + 0x36) = 16;
    else D(self + 0x45) = 0;
    W(self + 0x3e) = 0;
    D(self + 0xb1) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return self;
}
