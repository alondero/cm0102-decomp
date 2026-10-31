#include "cmre.h"

/* Preferred-image VA 009202b0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep the signed year remainder, WORD wrapping, and BYTE SEH states.
   Original constructors and competition helpers remain explicit boundaries. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_00682200(u32);
extern u32 CM_THISCALL cm_fn_00687b10(u32,u32);
extern u32 CM_THISCALL cm_fn_00920d10(u32);
extern u32 CM_THISCALL cm_fn_00921040(u32);
extern u32 CM_THISCALL cm_fn_00920eb0(u32);
extern u32 CM_THISCALL cm_fn_0049ee70(u32,u32);
extern u32 CM_THISCALL cm_fn_009216d0(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static void diagnostic(u32 name, u32 extension, u32 line)
{
    u32 version;
    cm_fn_00944cff(0x00ad4ce0,0,0,name,extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,extension,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34) = 0;
}

u32 CM_THISCALL cm_fn_009202b0(u32 self, volatile u32 year_arg,
                              volatile u32 competition_arg)
{
    volatile struct {u32 prev, handler, state;} frame;
    u8 scratch[0x208];
    u32 local = (u32)scratch, competition, year, value, first, second, result;
    frame.state = 0xffffffffu;
    frame.handler = 0x00965ba9;
    frame.prev = CM_FS(u32,0);
    CM_FS(u32,0) = (u32)&frame;
    D(local + 4) = self;
    cm_fn_00682200(self);
    competition = competition_arg;
    year = year_arg;
    D(self + 4) = competition;
    D(self) = 0x00970e2c;
    W(self + 0x40) = (u16)year;
    first = D(0x009cf76c);
    D(self + 0x1c) = first;
    second = D(0x009cf77c);
    value = ((u32)(s32)(s16)year - 0x7d0u) & 0x80000003u;
    frame.state = 0;
    D(self + 0x20) = second;
    B(self + 0x42) = 4;
    B(self + 0x50) = 0;
    D(self + 0x45) = 0;
    if ((s32)value < 0) value = ((value - 1) | 0xfffffffcu) + 1;
    if (value == 1) W(self + 0x40) = (u16)(year - 1);
    for (;;) {
        value = ((u32)(s32)(s16)W(self + 0x40) - 0x7d0u) & 0x80000003u;
        if ((s32)value < 0) value = ((value - 1) | 0xfffffffcu) + 1;
        if (value == 0) break;
        W(self + 0x40) = (u16)(W(self + 0x40) + 1);
    }
    if ((s16)W(self + 0x40) < 0x7d4) W(self + 0x40) = 0x7d4;
    result = cm_fn_00687b10(self,1);
    if (result != 0) goto done;
    W(self + 0xd9) = 0x24;
    B(self + 0x44) = 0xff;
    D(self + 0x30) = 0xffffffffu;
    D(self + 0x2c) = 0;
    D(self + 0xc) = 0;
    cm_fn_00920d10(self);
    result = cm_fn_00921040(self);
    if (result == 0) {
        diagnostic(local + 0x108,local + 8,0x65);
        goto done;
    }
    result = cm_fn_00920eb0(self);
    if (result == 0) {
        diagnostic(local + 8,local + 0x108,0x6c);
        goto done;
    }
    result = cm_fn_00944cf1(0x5ce);
    D(local) = result;
    B((u32)&frame.state) = 1;
    if (result != 0) result = cm_fn_0049ee70(result,self);
    B((u32)&frame.state) = 0;
    D(self + 8) = result;
    if (result == 0) {
        diagnostic(local + 8,local + 0x108,0x73);
        goto done;
    }
    if ((s16)year >= (s16)W(self + 0x40)) cm_fn_009216d0(self);
done:
    CM_FS(u32,0) = frame.prev;
    return self;
}
