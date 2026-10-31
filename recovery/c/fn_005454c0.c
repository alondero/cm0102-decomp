#include "cmre.h"

extern u32 CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_FASTCALL cm_fn_00544fc0(u32 *allocation);

u32 CM_CDECL cm_fn_005454c0(const u32 *arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095797bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    s32 mode = CM_GLOBAL(volatile s32, 0x009b5984u);
    if (mode == 0 || mode == 1) {
        u32 result = (u32)arg + 0x53u;
        u32 restore_prev = frame.prev;
        CM_FS(u32, 0) = restore_prev;
        return result;
    }

    u32 manager = CM_GLOBAL(volatile u32, 0x00ae2408u);
    if (manager == 0) {
        u32 allocation = cm_fn_00944cf1(0x40u);
        frame.state = 0;
        if (allocation != 0) {
            CM_GLOBAL(volatile u32, allocation + 0x20u) = 0;
            CM_GLOBAL(volatile u32, allocation + 0x24u) = 0;
            CM_GLOBAL(volatile u32, allocation + 0x28u) = 0;
            CM_GLOBAL(volatile u32, allocation + 0x2cu) = 0;
            CM_GLOBAL(volatile u32, allocation + 0x30u) = 0;
            CM_GLOBAL(volatile u32, allocation + 0x34u) = 0;
            CM_GLOBAL(volatile u32, allocation + 0x38u) = 0;
            CM_GLOBAL(volatile u32, allocation + 0x3cu) = 0;
            (void)cm_fn_00544fc0((u32 *)allocation);
        }
        manager = allocation;
        CM_GLOBAL(volatile u32, 0x00ae2408u) = manager;
        if (manager == 0) {
            u32 restore_prev = frame.prev;
            CM_FS(u32, 0) = restore_prev;
            return 0;
        }
    }

    u32 index = CM_GLOBAL(volatile u32, (u32)arg);
    u32 base = CM_GLOBAL(volatile u32, manager + 0x2cu);
    u32 result = base + index * 0x6du + 0x4fu;
    u32 restore_prev = frame.prev;
    CM_FS(u32, 0) = restore_prev;
    return result;
}