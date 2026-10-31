#include "cmre.h"

extern void CM_CDECL cm_fn_004dd0f0(u32 player, u32 result, u32 mode, u32 flags);
extern void CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_00785830(void *self, u8 value);
extern u32 CM_THISCALL cm_fn_00786b70(u32 self, u32 arg1, u32 arg2, void *buffer);
extern u32 CM_THISCALL cm_fn_008c61e0(u32 self, u32 arg1);

void CM_CDECL cm_fn_004e5770(u32 arg1, u32 arg2)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 local_area[0xe0];
    u32 buffer = (u32)&local_area[0];
    u32 arg2_copy;
    u32 arg1_copy;
    u32 status;
    u32 result;
    u32 player;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095732bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_0077bbe0((void *)buffer, 0u, 0u);
    arg2_copy = *(volatile u32 *)((u32)&arg2);
    arg1_copy = *(volatile u32 *)((u32)&arg1);
    frame.state = 0u;
    cm_fn_00786b70(CM_GLOBAL(volatile u32, 0x00b7431cu), arg1_copy, arg2_copy,
                   (void *)buffer);

    status = cm_fn_00785830((void *)buffer, 0u);
    if (*(volatile u32 *)buffer == 0x0fa4u) {
        result = cm_fn_00785830((void *)buffer, 7u);
        result = cm_fn_008c61e0(CM_GLOBAL(volatile u32, 0x00de1f54u), result);
        player = CM_GLOBAL(volatile u32, 0x00ae23bcu) + status * 110u;
        cm_fn_004dd0f0(player, result, 1u, 0u);
    } else {
        player = CM_GLOBAL(volatile u32, 0x00ae23bcu) + status * 110u;
        cm_fn_004dd0f0(player, 0u, 0u, 0u);
    }

    CM_FS(u32, 0) = frame.prev;
}
