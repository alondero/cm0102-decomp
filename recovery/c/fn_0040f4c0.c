#include "cmre.h"

extern short *CM_THISCALL cm_fn_00549f70(short *, short, char, u16, u32);
typedef u32 (CM_THISCALL *cm_fn_0040f4c0_slot)(void *, s32, void *, void *, s32);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

u32 CM_THISCALL cm_fn_0040f4c0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    volatile u8 scratch[8];
    void *entry_prev;
    void *restore_prev;
    u32 packed;
    u32 result;
    u8 *vtable;

    seh.state = -1;
    seh.handler = (void *)0x009538e8;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    base[0x43] = 2;
    base[0xc6] = 2;
    base[0xbe] = 2;
    packed = ((u32)entry_prev & 0xffff0000u) | *(volatile u16 *)(base + 0x40);
    base[0xc2] = 3;
    base[0xc7] = 3;
    base[0x4a] = 3;
    *(volatile u16 *)(base + 0x3c) = 1;
    base[0xc3] = 1;
    base[0xc4] = 0x0a;
    base[0xc5] = 1;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 0;
    base[0x49] = 7;
    cm_fn_00549f70((short *)scratch, 5, 5, (u16)packed, 5);

    {
        u16 scratch_result = *(volatile u16 *)scratch;
        vtable = *(u8 *volatile *)base;
        *(volatile u16 *)(base + 0xb6) = scratch_result;
    }
    seh.state = 0;
    *(volatile u16 *)(base + 0xb8) = 0;
    result = (*(cm_fn_0040f4c0_slot volatile *)(vtable + 0x3c))(
        self, -1, (u8 *)self + 0xa9, (u8 *)self + 0x3a, 0);
    restore_prev = seh.prev;
    *(volatile u32 *)(base + 0xba) = result;
    CM_FS(void *, 0) = restore_prev;
    return 1;
}
