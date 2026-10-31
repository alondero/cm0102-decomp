#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_THISCALL cm_fn_00682200(u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0068b4f0(u32 self, u32 count, u32 rounds)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    u32 previous, allocated, value, other, table, name, extension, line, text;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095cb0b;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    D((u32)scratch) = self;
    cm_fn_00682200(self);
    count = (u32)(s32)(s16)count;
    D(self) = 0x0096d0f0;
    frame.state = 0;
    allocated = (u32)cm_fn_00944e46(count * 59u);
    D(self + 0xb1) = allocated;
    if (allocated == 0) {
        name = (u32)scratch + 0x104;
        extension = (u32)scratch + 4;
        line = 0xa6;
        goto error;
    }
    if ((s16)rounds > 0) {
        allocated = (u32)cm_fn_00944e46((u32)(s32)(s16)rounds * 65u);
        D(self + 0xba) = allocated;
        if (allocated != 0)
            goto done;
        name = (u32)scratch + 4;
        extension = (u32)scratch + 0x104;
        line = 0xaf;
        goto error;
    }
    allocated = (u32)cm_fn_00944e46(65);
    D(self + 0xba) = allocated;
    if (allocated == 0) {
        name = (u32)scratch + 4;
        extension = (u32)scratch + 0x104;
        line = 0xba;
        goto error;
    }
    value = W(0x00ae2c90);
    W(allocated) = (u16)value;
    value = W(0x00ae2c92);
    other = W(self + 0x40);
    value = (u16)(value - other);
    table = D(self + 0xba);
    W(table + 2) = (u16)value;
    table = D(self + 0xba);
    B(table + 4) = 0;
    goto done;
error:
    cm_fn_00944cff(0x009f1630, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return self;
}
