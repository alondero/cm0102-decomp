#include "cmre.h"

typedef u32 (CM_THISCALL *cm_slot_3c)(void *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);

u32 CM_THISCALL cm_fn_0043a600(void *self)
{
    volatile u8 *p = (volatile u8 *)self;
    u8 *vtable = *(u8 * volatile *)p;
    u32 zero = 0;
    u32 two = 2;
    u32 result;
    cm_slot_3c slot;

    *(volatile u16 *)(p + 0xab) = (u16)zero;
    *(volatile u16 *)(p + 0xdb) = (u16)zero;
    p[0xc7] = (u8)zero;
    p[0xbe] = (u8)zero;
    p[0xbf] = (u8)zero;
    p[0xc0] = (u8)zero;
    p[0xc2] = 3;
    p[0xc5] = 3;
    p[0x4a] = 3;
    *(volatile u16 *)(p + 0x3c) = (u16)two;
    p[0xc4] = (u8)two;
    p[0xc1] = (u8)two;
    p[0xc3] = 1;
    p[0x42] = 1;
    p[0xc6] = 1;
    *(volatile u32 *)(p + 0x1c) = 0xffffffff;
    *(volatile u32 *)(p + 0x20) = 0xffffffff;
    p[0x49] = 7;
    *(volatile u16 *)(p + 0xd9) = 8;
    slot = *(cm_slot_3c volatile *)(vtable + 0x3c);
    result = slot(self, 0xffffffff, (u32)(p + 0xa9), (u32)(p + 0x3a), zero);
    *(volatile u32 *)(p + 0xba) = result;
    return 1;
}
