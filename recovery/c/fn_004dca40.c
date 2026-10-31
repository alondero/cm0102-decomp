#include "cmre.h"

extern void CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_00786b70(u32 self, u32 arg1, u32 arg2,
                                     void *output);
extern u32 CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2,
                                      u32 arg3, u32 arg4, u32 arg5);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 arg1, u32 arg2,
                                       u32 arg3);

void CM_CDECL cm_fn_004dca40(u32 arg1, u32 arg2)
{
    volatile struct { u32 previous, handler, state; } frame;
    volatile u8 local_area[0xe0];
    u32 first_value;
    u32 second_value;
    u32 choice0;
    u32 choice2;
    u32 menu_result;

    frame.state = 0xffffffffu;
    frame.handler = 0x009570bbu;
    frame.previous = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_0077bbe0((void *)&local_area[0], 0u, 0u);
    second_value = arg2;
    first_value = arg1;
    menu_result = CM_GLOBAL(volatile u32, 0x00b7431cu);
    frame.state = 0u;
    (void)cm_fn_00786b70(menu_result, first_value, second_value,
                         (void *)&local_area[0]);
    choice0 = cm_fn_00785830((void *)&local_area[0], 0u);
    choice2 = cm_fn_00785830((void *)&local_area[0], 2u);
    menu_result = cm_fn_007fe600(0x00b74450u, 0x004dcb30u, 0x004dcd70u,
                                 1u, 0u, 0u);
    if (menu_result != 0u) {
        cm_fn_007ff270(0x00b74450u, 0u, first_value, 0u);
        cm_fn_007ff270(0x00b74450u, 1u, second_value, 0u);
        cm_fn_007ff270(0x00b74450u, 2u, choice0, 0u);
        cm_fn_007ff270(0x00b74450u, 3u, choice2, 0u);
    }
    CM_FS(u32, 0) = frame.previous;
}
