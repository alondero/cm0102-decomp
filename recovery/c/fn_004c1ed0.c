#include "cmre.h"

extern short *CM_THISCALL cm_fn_00549f70(short *, short, char, u16, u32);
typedef u32 (CM_THISCALL *cm_fn_004c1ed0_slot)(void *, s32, void *, void *, s32);
typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

u32 CM_THISCALL cm_fn_004c1ed0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    volatile u8 scratch[8];
    void *restore_prev;
    u16 version;
    u32 result;
    u8 *vtable;

    seh.state = -1;
    seh.handler = (void *)0x009569c8;
    seh.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&seh;

    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc6] = 2;
    base[0x42] = 2;
    version = *(volatile u16 *)(base + 0x40);

    base[0xc2] = 3;
    base[0xc7] = 3;
    base[0xc3] = 1;
    base[0xc4] = 7;
    base[0xc5] = 1;
    base[0xbe] = 1;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 0;
    *(volatile u32 *)(base + 0x1c) = 0xffffffffu;
    *(volatile u32 *)(base + 0x20) = 0xffffffffu;
    cm_fn_00549f70((short *)scratch, 1, 7, version, 0xffffffffu);

    {
        u16 scratch_result = *(volatile u16 *)scratch;
        vtable = *(u8 *volatile *)base;
        *(volatile u16 *)(base + 0xb6) = scratch_result;
    }
    seh.state = 0;
    *(volatile u16 *)(base + 0xb8) = 0;
    *(volatile u32 *)(base + 0x28) = 2000000;
    result = (*(cm_fn_004c1ed0_slot volatile *)(vtable + 0x3c))(
        self, -1, (u8 *)self + 0xa9, (u8 *)self + 0x3a, 0);
    restore_prev = seh.prev;
    *(volatile u32 *)(base + 0xba) = result;
    CM_FS(void *, 0) = restore_prev;
    return 1;
}
