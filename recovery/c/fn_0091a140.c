#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 *CM_THISCALL cm_fn_0068b100(
    u32 *allocation, u32 original_self, u16 count, int zero1, u16 zero2,
    u32 field4, u32 zero3, u32 zero4, u8 byte_c2, u8 byte_c3, u8 byte_c4,
    u8 *locals, u8 *flag, u16 field40, u8 index, u32 constant3fc, u32 zero5,
    u32 four, u32 zero6, u32 two1, u32 minus_one, u32 zero7, u32 two2);
extern void CM_FASTCALL cm_fn_0091a240(u32 self);

void CM_THISCALL cm_fn_0091a140(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 locals[8];
    s8 initial_count;
    u8 index;
    u16 field40;
    u8 byte_c2;
    u8 byte_c3;
    u8 byte_c4;
    u8 byte_100;
    u32 field4;
    u32 allocation;
    u32 result;
    u32 array;
    u32 slot;

    frame.state = 0xffffffffu;
    frame.handler = 0x00965a1bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;

    locals[0] = 1u;
    initial_count = *(volatile s8 *)(self + 0xffu);
    locals[1] = 2u;
    locals[2] = 5u;
    locals[3] = 0u;
    *(volatile u8 *)(self + 0xbeu) = 0u;
    allocation = (u32)(unsigned long)cm_fn_00944cf1(0xeeu);
    *(volatile u32 *)&locals[4] = allocation;
    frame.state = 0u;

    if (allocation != 0u) {
        byte_100 = *(volatile u8 *)(self + 0x100u);
        field40 = *(volatile u16 *)(self + 0x40u);
        byte_c4 = *(volatile u8 *)(self + 0xc4u);
        byte_c3 = *(volatile u8 *)(self + 0xc3u);
        byte_c2 = *(volatile u8 *)(self + 0xc2u);
        field4 = *(volatile u32 *)(self + 4u);

        result = (u32)(unsigned long)cm_fn_0068b100(
            (u32 *)allocation, self,
            (u16)((s16)initial_count + 2), 0, 0, field4, 0, 0,
            byte_c2, byte_c3, byte_c4,
            (u8 *)&locals[0], (u8 *)(unsigned long)(self + 0xbeu),
            field40, byte_100, 0x3fcu, 0, 4, 0, 2, 0xffffffffu, 0, 2);
    } else {
        result = 0u;
    }

    index = *(volatile u8 *)(self + 0x100u);
    array = *(volatile u32 *)(self + 0x0cu);
    slot = array + (u32)((s32)(s8)index * 4);
    frame.state = 0xffffffffu;
    *(volatile u32 *)slot = result;
    index = *(volatile u8 *)(self + 0x100u);
    *(volatile u8 *)(self + 0xbeu) = 1u;
    *(volatile u32 *)(self + 0x30u) = (u32)(s32)(s8)index;
    cm_fn_0091a240(self);

    CM_FS(u32, 0) = frame.prev;
}
