#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_FASTCALL cm_fn_00518640(u32);
extern u32 CM_FASTCALL cm_fn_00518790(u32);
extern u32 CM_THISCALL cm_fn_0051fc00(u32, u32);
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0056f040(u32 self, u16 year, u32 competition)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 local = (u32)scratch;
    u32 table, target, value, name, extension, line, text;
    frame.state = 0xffffffffu;
    frame.handler = 0x00958299;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(local + 4) = self;
    cm_fn_00518640(self);
    D(self + 4) = competition;
    frame.state = 0;
    D(self) = 0x00969b28;
    W(self + 0x40) = year;
    B(self + 0xab) = 0;
    B(self + 0x44) = 0xff;
    D(self + 0x2c) = 0;
    D(self + 0x30) = 0xffffffffu;
    B(self + 0x42) = 1;
    B(self + 0x50) = 9;
    B(self + 0xb1) = 0;
    D(self + 0x45) = 0;
    if (cm_fn_0051fc00(self, 1) != 0)
        goto done;
    B(self + 0xb2) = 0;
    B(self + 0xb4) = 1;
    B(self + 0xb3) = 0;
    W(self + 0x3e) = 0x98;
    value = cm_fn_00945501(0x98, 6);
    D(self + 0xa7) = value;
    if (value == 0) {
        name = local + 0x108;
        extension = local + 8;
        line = 0x63;
        goto error;
    }
    table = D(self);
    D(self + 0x28) = 0x1e8480;
    target = D(table + 0x3c);
    value = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 0xffffffffu, self + 0x3c, self + 0x3a, 0);
    D(self + 0xa3) = value;
    if ((s32)cm_fn_00518790(self) < 0) {
        name = local + 8;
        extension = local + 0x108;
        line = 0x71;
        goto error;
    }
    value = (u32)cm_fn_00944cf1(0x5ce);
    D(local) = value;
    *(volatile u8 *)&frame.state = 1;
    if (value != 0)
        value = cm_fn_0049ee70(value, self);
    *(volatile u8 *)&frame.state = 0;
    D(self + 8) = value;
    if (value != 0) {
        goto done;
    }
    name = local + 8;
    extension = local + 0x108;
    line = 0x79;
error:
    cm_fn_00944cff(0x009c42cc, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return self;
}
