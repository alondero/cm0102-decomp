#include "cmre.h"
extern u32 CM_THISCALL cm_fn_005aff00(u32 self, u32 left);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);

u32 CM_THISCALL cm_fn_005afd70(u32 self, u32 input)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    u32 result, index, value;
    result = CM_FS(u32, 0);
    frame.state = -1;
    frame.handler = 0x0095988b;
    frame.prev = result;
    CM_FS(u32, 0) = (u32)&frame;
    if (input) {
        index = CM_GLOBAL(volatile u32, 0x00ae2364);
        value = *(volatile u32 *)input;
        index = index - value - 1;
        result = (u32)cm_fn_00944cf1(8);
        frame.state = 0;
        if (result)
            result = cm_fn_005aff00(result, input);
        *(volatile u32 *)(self + index * 4) = result;
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
