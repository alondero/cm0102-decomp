#include "cmre.h"

extern void *CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_THISCALL cm_fn_0068b100(
    u32 receiver, u32 stack_self, u32 arg3, u32 arg4, u32 arg5, u32 arg6,
    u32 arg7, u32 arg8, u32 arg9, u32 arg10, u32 arg11, u32 arg12,
    u32 arg13, u32 arg14, u32 arg15, u32 arg16, u32 arg17, u32 arg18,
    u32 arg19, u32 arg20, u32 arg21, u32 arg22, u32 arg23);
extern void CM_THISCALL cm_fn_0057fa10(void *self);

void CM_THISCALL cm_fn_0057f920(void *self)
{
    volatile struct { u32 previous, handler, state; } frame;
    u32 self_address = (u32)self;
    volatile u8 local_area[8];
    u8 *descriptor = (u8 *)(self_address + 0xbeu);
    u32 allocation;
    u32 result;
    s32 index;
    u32 array;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095891bu;
    frame.previous = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    local_area[0] = 1u;
    local_area[1] = 2u;
    local_area[2] = 5u;
    local_area[3] = 0u;
    * (volatile u8 *)descriptor = 0u;
    allocation = (u32)cm_fn_00944cf1(0xeeu);
    *(volatile u32 *)&local_area[4] = allocation;
    frame.state = 0u;

    if (allocation == 0u) {
        result = 0u;
    } else {
        u8 value100 = *(volatile u8 *)(self_address + 0x100u);
        u16 value40 = *(volatile u16 *)(self_address + 0x40u);
        u8 valuec4 = *(volatile u8 *)(self_address + 0xc4u);
        u32 local_pointer = (u32)&local_area[0];
        u8 valuec3 = *(volatile u8 *)(self_address + 0xc3u);
        u8 valuec2 = *(volatile u8 *)(self_address + 0xc2u);
        u32 value4 = *(volatile u32 *)(self_address + 4u);

        result = cm_fn_0068b100(
            allocation, self_address, 9u, 0u, 0u, value4, 0u, 0u,
            valuec2, valuec3, valuec4, local_pointer, self_address + 0xbeu,
            value40, value100, 0x3fcu, 0u, 1u, 0u, 2u, 0xffffffffu,
            0u, 2u);
    }

    index = (s32)(s8)*(volatile u8 *)(self_address + 0x100u);
    array = *(volatile u32 *)(self_address + 0x0cu);
    frame.state = 0xffffffffu;
    *(volatile u32 *)(array + (u32)index * 4u) = result;
    index = (s32)(s8)*(volatile u8 *)(self_address + 0x100u);
    * (volatile u8 *)descriptor = 1u;
    *(volatile u32 *)(self_address + 0x30u) = (u32)index;
    cm_fn_0057fa10(self);
    CM_FS(u32, 0) = frame.previous;
}
