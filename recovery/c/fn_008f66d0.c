#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_00786b70(u32 *self, u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_008f6770(u32 *param1, u32 param2, u32 param3, u32 param4);

u32 CM_CDECL cm_fn_008f66d0(u32 first, u32 second)
{
    volatile struct { u32 prev, handler, state; } frame;
    frame.state = 0xffffffffu;
    frame.handler = 0x00964e3bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    volatile u8 local[0xe0];
    (void)cm_fn_0077bbe0((void *)&local[0], 0, 0);
    u32 arg2 = second;
    u32 arg1 = first;
    u32 global_self = CM_GLOBAL(volatile u32, 0x00b7431cu);
    frame.state = 0;
    (void)cm_fn_00786b70((u32 *)global_self, arg1, arg2, (u32)&local[0]);

    u32 index = (u32)cm_fn_00785830((void *)&local[0], 2u);
    u32 address = CM_GLOBAL(volatile u32, 0x00ae23bcu) + index * 110u;
    u32 result = cm_fn_008f6770((u32 *)address, arg1, arg2, 1u);

    u32 previous = frame.prev;
    CM_FS(u32, 0) = previous;
    return result;
}
