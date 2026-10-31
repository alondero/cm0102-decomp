#include "cmre.h"

extern void CM_FASTCALL cm_fn_00768290(u8 *self);
extern u32 CM_THISCALL cm_fn_0076a280(u8 *self, u32 argument);
extern void CM_THISCALL cm_fn_005c0a20(u8 *self, u32 argument);

u32 CM_THISCALL cm_fn_005c0980(u8 *self, u32 argument)
{
    struct {
        u32 previous;
        u32 handler;
        u32 state;
    } volatile frame;
    u32 index;
    u32 base;
    u32 nine_index;
    u32 argument_value;
    u32 scaled_index;
    s32 ticks;
    s32 sign;
    u32 magnitude;

    frame.state = 0xffffffffu;
    frame.handler = 0x00959b68u;
    frame.previous = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_00768290(self);
    *(volatile u32 *)self = 0x0096ae34u;
    index = CM_GLOBAL(volatile u32, 0x009cf300);
    base = CM_GLOBAL(volatile u32, 0x00ae23a8);
    argument_value = argument;
    nine_index = index + index * 8u;
    frame.state = 0;
    scaled_index = nine_index * 16u;
    scaled_index += index;
    *(volatile u32 *)(self + 0x13) = 9;
    *(volatile u8 *)(self + 0x12) = 1;
    *(volatile u32 *)(self + 0x0e) = base + scaled_index * 2u;
    if (argument_value != 0) {
        ticks = (s32)CM_GLOBAL(volatile u32, 0x00dd7110);
        sign = ticks < 0 ? -1 : 0;
        magnitude = ((u32)ticks ^ (u32)sign) - (u32)sign;
        if ((s32)magnitude >= 0x3e028) {
            if (cm_fn_0076a280(self, argument_value) != 0) {
                CM_FS(u32, 0) = frame.previous;
                return (u32)self;
            }
        }
    }

    cm_fn_005c0a20(self, argument_value);
    CM_FS(u32, 0) = frame.previous;
    return (u32)self;
}
