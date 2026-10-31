#include "cmre.h"

extern void CM_THISCALL cm_fn_005f9070(u32 self, u32 param_2);
extern void CM_FASTCALL cm_fn_00768290(u32 self);
extern u32 CM_THISCALL cm_fn_0076a280(u32 self, u32 param_2);

u32 CM_THISCALL
cm_fn_005f8fe0(u32 self, u32 param_2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 index;
    u32 base;
    u32 field_value;

    seh.state = -1;
    seh.handler = 0x0095ae78u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cm_fn_00768290(self);
    *(volatile u32 *)self = 0x0096ba74u;
    index = CM_GLOBAL(volatile u32, 0x009cf338);
    base = CM_GLOBAL(volatile u32, 0x00ae23a8);
    seh.state = 0;
    field_value = base + index * 0x122u;
    *(volatile u32 *)(self + 0x13u) = 0x0bu;
    *(volatile u8 *)(self + 0x12u) = 1;
    *(volatile u32 *)(self + 0x0eu) = field_value;

    if (param_2 != 0 && cm_fn_0076a280(self, param_2) != 0) {
        CM_FS(u32, 0) = seh.prev;
        return self;
    }
    cm_fn_005f9070(self, param_2);
    CM_FS(u32, 0) = seh.prev;
    return self;
}

