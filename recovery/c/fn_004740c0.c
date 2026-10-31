#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00786b70(u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785830(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005c88f0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_004740c0(u32 first, u32 second)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x2e8];
    u32 result, text, id, low, high, club, context, record;
    frame.state = 0xffffffffu;
    frame.handler = 0x00955d86;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0077bbe0((u32)scratch + 8, 0, 0);
    frame.state = 0;
    context = D(0x00b7431c);
    result = cm_fn_00786b70(context, first, second, (u32)scratch + 8);
    if (result == 0) {
        cm_fn_00944cff(0x0098e598, 0, 0, (u32)scratch + 0x1e8, (u32)scratch + 0xe8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      (u32)scratch + 0x1e8, (u32)scratch + 0xe8, 0x3729);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    id = (u32)cm_fn_00785830((u32)scratch + 8, 2);
    low = (u32)cm_fn_00785830((u32)scratch + 8, 3);
    high = (u32)cm_fn_00785830((u32)scratch + 8, 4);
    if ((s32)id >= 0) {
        club = D(0x00ae23b4);
        club += id * 581u;
    } else {
        club = 0;
    }
    result = (u32)cm_fn_00549ef0((u32)scratch, low, high);
    context = D(0x00ae2c74);
    *(volatile u8 *)&frame.state = 1;
    record = cm_fn_005c88f0(context, result, club);
    *(volatile u8 *)&frame.state = 0;
    result = cm_fn_007fe600(0x00b74450, 0x00474280, 0x004744b0, 1, 0, 0);
    if (result != 0) {
        cm_fn_007ff270(0x00b74450, 0, first, 0);
        cm_fn_007ff270(0x00b74450, 1, second, 0);
        result = (u32)cm_fn_007ff270(0x00b74450, 2, record, 0);
    }
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
