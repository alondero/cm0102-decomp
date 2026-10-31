#include "cmre.h"

typedef struct { void *prev; void *handler; s32 state; } cm_8b5930_seh;
extern u32 CM_THISCALL cm_fn_007fe600(void *self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5);
extern void CM_THISCALL cm_fn_007ff270(void *self, u32 index, u32 value, u32 zero);
extern u32 CM_THISCALL cm_fn_008b0a00(void *self, u32 arg);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);

void CM_CDECL cm_fn_008b5930(u32 argument)
{
    volatile cm_8b5930_seh frame;
    volatile u32 allocation_slot __attribute__((unused));
    unsigned long long allocation_result;
    u32 allocation;
    u32 value;
    u32 initialized;

    frame.state = -1;
    frame.handler = (void *)0x0096387bu;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    initialized = cm_fn_007fe600((void *)0x00b74450u, 0x008b5a10u,
                                  0x008b83f0u, 1u, 0u, 0x008b97a0u);
    if (initialized != 0u) {
        allocation_result = cm_fn_00944cf1(0x819u);
        allocation = (u32)allocation_result;
        allocation_slot = allocation;
        frame.state = 0;
        if (allocation != 0u)
            value = cm_fn_008b0a00((void *)allocation, argument);
        else
            value = 0u;

        frame.state = -1;
        cm_fn_007ff270((void *)0x00b74450u, 0u, value, 0u);
        cm_fn_007ff270((void *)0x00b74450u, 1u, 0u, 0u);
        cm_fn_007ff270((void *)0x00b74450u, 2u, 0u, 0u);
        cm_fn_007ff270((void *)0x00b74450u, 3u, 0u, 0u);
        cm_fn_007ff270((void *)0x00b74450u, 4u, 1u, 0u);
        cm_fn_007ff270((void *)0x00b74450u, 5u, 0xffffffffu, 0u);
    }

    CM_FS(void *, 0) = frame.prev;
}
