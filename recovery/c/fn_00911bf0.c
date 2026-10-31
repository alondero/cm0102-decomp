#include "cmre.h"

/* Preferred-image VA 0x00911bf0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32, u32);
extern u32 CM_FASTCALL cm_fn_00913f60(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_00912eb0(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00911bf0(u32 self, volatile u32 first, volatile u32 second)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 local = (u32)scratch, year, owner, value, name, extension, line, text;
    s32 difference;
    frame.state = 0xffffffffu;
    frame.handler = 0x00965709;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(local + 4) = self;
    cm_fn_00682200(self);
    year = first;
    owner = second;
    D(self) = 0x00970b70;
    W(self + 0x40) = (u16)year;
    D(self + 4) = owner;
    B(self + 0x42) = 4;
    value = D(0x009cf770);
    D(self + 0x1c) = value;
    value = D(0x009cf76c);
    difference = (s16)year - 2000;
    frame.state = 0;
    D(self + 0x20) = value;
    B(self + 0x49) = 7;
    B(self + 0x4a) = 3;
    D(self + 0xf7) = 0;
    B(self + 0x50) = 0;
    if (difference % 4 == 1) {
        --year;
        W(self + 0x40) = (u16)year;
    }
    difference = CM_GLOBAL(volatile s16, self + 0x40) - 2000;
    while (difference % 4 != 0) {
        W(self + 0x40) = (u16)(W(self + 0x40) + 1);
        difference = CM_GLOBAL(volatile s16, self + 0x40) - 2000;
    }
    if (CM_GLOBAL(volatile s16, self + 0x40) < 2004) W(self + 0x40) = 2004;
    B(self + 0x51) = 10;
    if (cm_fn_00687b10(self, 1) != 0) goto done;
    W(self + 0xd9) = 0x28;
    D(self + 0x45) = 0;
    cm_fn_00913f60(self);
    B(self + 0x44) = 0xff;
    D(self + 0x2c) = 13;
    D(self + 0x30) = 0xffffffffu;
    value = cm_fn_00944e46(0x34);
    D(self + 0xc) = value;
    if (cm_fn_00912eb0(self) == 0) {
        name = local + 0x108; extension = local + 8; line = 0x6f;
        goto error;
    }
    value = cm_fn_00944cf1(0x5ce);
    D(local) = value;
    *(volatile u8 *)&frame.state = 1;
    if (value != 0) value = cm_fn_0049ee70(value, self);
    else value = 0;
    *(volatile u8 *)&frame.state = 0;
    D(self + 8) = value;
    if (value != 0) goto done;
    name = local + 8; extension = local + 0x108; line = 0x77;
error:
    cm_fn_00944cff(0x00ad4680, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    value = frame.prev;
    CM_FS(u32, 0) = value;
    return self;
}
