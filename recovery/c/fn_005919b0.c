#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_004ae5c0(u32, u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_005919b0(u32 first, u32 second, u32 third, u32 fourth,
                           u32 fifth, u32 sixth, u32 seventh)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    u32 previous, object, result, text;
    previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00958e8e;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    if (first == 0 || second == 0 || third == 0) {
        cm_fn_00944cff(0x009c7a48, 0, 0, (u32)scratch + 0x104, (u32)scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      (u32)scratch + 0x104, (u32)scratch + 4, 0x611);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    object = (u32)cm_fn_00944cf1(8);
    D((u32)scratch) = object;
    frame.state = 0;
    if (object != 0)
        object = cm_fn_004ae5c0(object, third);
    frame.state = 0xffffffffu;
    result = cm_fn_007fe600(0x00b74450, 0x00591b70, 0x00591dc0, 1, 0, 0);
    if (result != 0) {
        cm_fn_007ff270(0x00b74450, 0, first, 1);
        cm_fn_007ff270(0x00b74450, 1, second, 1);
        cm_fn_007ff270(0x00b74450, 2, 0, 0);
        cm_fn_007ff270(0x00b74450, 3, fourth, 0);
        cm_fn_007ff270(0x00b74450, 4, (u32)(s32)(s16)fifth, 0);
        cm_fn_007ff270(0x00b74450, 5, (u32)(s32)(s16)sixth, 0);
        cm_fn_007ff270(0x00b74450, 6, object, 0);
        result = (u32)cm_fn_007ff270(0x00b74450, 7, seventh, 0);
    }
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
