#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32, u32);
extern u32 CM_THISCALL cm_fn_00401ab0(u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00401000(u32 self, u32 first, u32 second)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 year, result, name, extension, line, text;
    s32 remainder;
    frame.state = 0xffffffffu;
    frame.handler = 0x00953499;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D((u32)scratch + 4) = self;
    cm_fn_00682200(self);
    year = (u16)(first + 1u);
    remainder = ((s32)(s16)year - 1996) % 2;
    D(self + 4) = second;
    frame.state = 0;
    D(self) = 0x00967270;
    B(self + 0x42) = 4;
    D(self + 0x1c) = 0xffffffffu;
    D(self + 0x20) = 0xffffffffu;
    B(self + 0x50) = 0;
    W(self + 0x40) = (u16)year;
    while (remainder != 0) {
        year = W(self + 0x40);
        W(self + 0x40) = (u16)(year + 1u);
        year = W(self + 0x40);
        remainder = ((s32)(s16)year - 1996) % 2;
    }
    B(self + 0x51) = 15;
    if (cm_fn_00687b10(self, 1) != 0)
        goto done;
    W(self + 0xd9) = 0x28;
    D(self + 0x45) = 0;
    B(self + 0x44) = 0xff;
    D(self + 0x30) = 0xffffffffu;
    D(self + 0x2c) = 4;
    result = (u32)cm_fn_00944e46(16);
    D(self + 0xc) = result;
    if (cm_fn_00401ab0(self) == 0) {
        name = (u32)scratch + 0x108;
        extension = (u32)scratch + 8;
        line = 0x58;
        goto error;
    }
    result = (u32)cm_fn_00944cf1(0x5ce);
    D((u32)scratch) = result;
    *(volatile u8 *)&frame.state = 1;
    if (result != 0)
        result = cm_fn_0049ee70(result, self);
    *(volatile u8 *)&frame.state = 0;
    D(self + 8) = result;
    if (result != 0)
        goto done;
    name = (u32)scratch + 8;
    extension = (u32)scratch + 0x108;
    line = 0x5f;
error:
    cm_fn_00944cff(0x009870f8, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return self;
}
