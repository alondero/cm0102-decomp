#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_00515de0_slot)(void *, s32, void *, void *, s32);

u32 CM_THISCALL cm_fn_00515de0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u32 first_global;
    u32 second_global;
    u32 result;
    u8 *vtable;

    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    *(volatile u16 *)(base + 0xab) = 0;
    *(volatile u16 *)(base + 0xdb) = 0;
    base[0x42] = 1;
    base[0xc5] = 4;
    base[0xc6] = 1;
    base[0xc7] = 2;
    base[0xc8] = 0;
    base[0xbe] = 0;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 2;
    first_global = (u32)CM_GLOBAL(volatile s32, 0x009cf8a4);
    *(volatile u32 *)(base + 0x1c) = first_global;
    second_global = (u32)CM_GLOBAL(volatile s32, 0x009cf8ac);
    base[0xd9] |= 0x40;
    base[0x52] = 2;
    base[0x4a] = 3;
    vtable = *(u8 *volatile *)base;
    *(volatile u32 *)(base + 0x20) = second_global;
    base[0x49] = 7;
    *(volatile u32 *)(base + 0xe6) = 1;
    result = (*(cm_fn_00515de0_slot volatile *)(vtable + 0x3c))(
        self, -1, (u8 *)self + 0xa9, (u8 *)self + 0x3a, 0);
    *(volatile u32 *)(base + 0xba) = result;
    return 1;
}
