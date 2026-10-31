#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_FASTCALL cm_fn_00448670(u32);
extern void CM_FASTCALL cm_fn_005a53c0(u32);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00946464(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_004a4850(u32 self, u32 target)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x27c];
    u32 local = (u32)scratch;
    u32 cached, count, array, value, result, text;
    frame.state = 0xffffffffu;
    frame.handler = 0x00956788;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    D(local) = local + 0x1d;
    cm_fn_0054a110(local + 0x21);
    frame.state = 0;
    cm_fn_0054a110(local + 0x29);
    frame.state = 1;
    D(local) = local + 0x31;
    cm_fn_0054a110(local + 0x35);
    *(volatile u8 *)&frame.state = 2;
    cm_fn_0054a110(local + 0x3d);
    *(volatile u8 *)&frame.state = 3;
    cm_fn_00448670(local + 0x45);
    *(volatile u8 *)&frame.state = 4;
    cm_fn_00448670(local + 0x59);
    cached = D(self + 0x566);
    frame.state = 5;
    if (cached != 0) {
        cm_fn_005a53c0(local + 0x1d);
        result = cached;
        goto done;
    }
    if (target == 0) {
        cm_fn_00944cff(0x0099ca94, 0, 0, local + 0x17c, local + 0x7c);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x17c, local + 0x7c, 0xce1);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto empty;
    }
    array = D(self + 0xc);
    if (array == 0)
        goto empty;
    if (B(self + 0x18) != 0xff) {
        B(self + 0x18) = 0xff;
        D(0x00adae10) = self;
        count = D(self + 0x14);
        array = D(self + 0xc);
        cm_fn_00945333(array, count, 0x76, 0x004ad1a0);
    }
    value = D(target);
    D(0x00adae10) = self;
    count = D(self + 0x14);
    array = D(self + 0xc);
    D(local + 4) = value;
    result = cm_fn_00946464(local + 4, array, count, 0x76, 0x004ad1a0);
    cm_fn_005a53c0(local + 0x59);
    cm_fn_005a53c0(local + 0x45);
    cm_fn_005a53c0(local + 0x31);
    cm_fn_005a53c0(local + 0x1d);
    goto done;
empty:
    cm_fn_005a53c0(local + 0x31);
    cm_fn_005a53c0(local + 0x1d);
    result = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
