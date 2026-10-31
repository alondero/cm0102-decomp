#include "cmre.h"

extern void CM_CDECL cm_fn_004ad9c0(u32 target, u32 field, u32 index,
                                    void *buffer);
extern void CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern void CM_THISCALL cm_fn_00786860(void *self, void *arg1, u32 arg2);

u32 CM_THISCALL cm_fn_004a2450(volatile u8 *self)
{
    volatile struct { u32 previous, handler, state; } frame;
    volatile u8 local_area[0xe0];
    volatile u32 *slot;
    u32 index;
    u32 saved_previous = CM_FS(u32, 0);

    frame.state = 0xffffffffu;
    frame.handler = 0x0095672bu;
    frame.previous = saved_previous;
    CM_FS(u32, 0) = (u32)&frame;

    if (*(volatile u32 *)(self + 0x562) == 0) {
        CM_FS(u32, 0) = frame.previous;
        return 0;
    }

    index = 1;
    slot = (volatile u32 *)(self + 0x50a);
    do {
        if (*slot != 0) {
            cm_fn_0077bbe0((void *)local_area, 0x3e9u, 0u);
            u32 manager = *(volatile u32 *)(self + 0x502);
            u32 manager_field = *(volatile u32 *)(manager + 4u);
            frame.state = 0u;
            u32 target = *(volatile u32 *)manager_field;
            cm_fn_004ad9c0(target, (u32)(self + 0x26), index,
                           (void *)local_area);

            manager = *(volatile u32 *)(self + 0x502);
            manager_field = *(volatile u32 *)(manager + 4u);
            cm_fn_00786860((void *)CM_GLOBAL(volatile u32, 0x00b7431cu),
                           (void *)local_area, manager_field);
            *slot = 0;
            frame.state = 0xffffffffu;
        }
        ++index;
        slot = (volatile u32 *)((u32)slot + 4u);
    } while (index <= 0x16u);

    {
        u32 restore_previous = frame.previous;
        *(volatile u32 *)(self + 0x562) = 0;
        CM_FS(u32, 0) = restore_previous;
    }
    return 1;
}
