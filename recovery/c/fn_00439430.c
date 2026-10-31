#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_00439430_slot)(void *, s32, void *, void *, s32);

u32 CM_THISCALL cm_fn_00439430(void *self)
{
    u32 result;
    volatile u8 *base = (volatile u8 *)self;
    u8 *vtable = *(u8 *volatile *)base;

    *(volatile u16 *)(base + 0xab) = 0;
    *(volatile u16 *)(base + 0xdb) = 0;
    base[0xc7] = 0;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc2] = 3;
    base[0xc5] = 3;
    base[0x4a] = 3;
    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc4] = 2;
    base[0xc1] = 2;
    base[0xc3] = 1;
    base[0x42] = 1;
    base[0xc6] = 1;
    base[0xbe] = 4;
    *(volatile u32 *)(base + 0x1c) = 0xffffffff;
    *(volatile u32 *)(base + 0x20) = 0xffffffff;
    base[0x49] = 7;
    *(volatile u16 *)(base + 0xd9) = 8;
    result = (*(cm_fn_00439430_slot volatile *)(vtable + 0x3c))(
        self, -1, (u8 *)self + 0xa9, (u8 *)self + 0x3a, 0);
    *(volatile u32 *)(base + 0xba) = result;
    return 1;
}
