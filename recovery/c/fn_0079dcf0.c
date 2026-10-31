#include "cmre.h"

extern void CM_FASTCALL cm_fn_00768290(u32 *self);
extern u32 CM_THISCALL cm_fn_0076a280(u32 *self, u32 argument);
extern void CM_THISCALL cm_fn_0079dd80(u32 *self, u32 argument);

u32 *CM_THISCALL cm_fn_0079dcf0(u32 *self, u32 argument)
{
    struct {
        void *prev;
        void *handler;
        u32 state;
    } volatile frame;

    frame.state = 0xffffffffu;
    frame.handler = (void *)0x0095ed18u;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    cm_fn_00768290(self);
    *(volatile u32 *)self = 0x0096e194u;

    u32 counter = CM_GLOBAL(volatile u32, 0x009cf300u);
    u32 base = CM_GLOBAL(volatile u32, 0x00ae23a8u);
    u32 field_e = base + counter * 290u;
    frame.state = 0;
    *(volatile u32 *)((u8 *)self + 0x13) = 9;
    *(volatile u8 *)((u8 *)self + 0x12) = 1;
    *(volatile u32 *)((u8 *)self + 0x0e) = field_e;

    if (argument == 0 || cm_fn_0076a280(self, argument) == 0) {
        cm_fn_0079dd80(self, argument);
    }

    CM_FS(void *, 0) = frame.prev;
    return self;
}
