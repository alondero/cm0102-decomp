#include "cmre.h"

typedef u32 (CM_THISCALL *cm_slot_3c)(void *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);

u32 CM_THISCALL cm_fn_00411ce0(void *self)
{
    volatile u8 *p = (volatile u8 *)self;
    u32 zero = 0;
    u32 three = 3;
    u8 *vtable;
    cm_slot_3c slot;
    u32 result;

    *(volatile u16 *)(p + 0xab) = (u16)zero;
    p[0xc8] = (u8)zero;
    p[0xbf] = (u8)zero;
    p[0xc0] = (u8)zero;
    p[0xc1] = (u8)zero;
    p[0xc4] = 2;
    p[0xc6] = 2;
    p[0x52] = 2;
    *(volatile u16 *)(p + 0x3c) = (u16)three;
    p[0xc2] = (u8)three;
    p[0x4a] = (u8)three;
    vtable = *(u8 * volatile *)p;
    p[0xc3] = 1;
    p[0x42] = 1;
    p[0xc5] = 1;
    p[0xc7] = 4;
    p[0xbe] = 6;
    *(volatile u32 *)(p + 0x1c) = 0xffffffff;
    *(volatile u32 *)(p + 0x20) = 0xffffffff;
    *(volatile u16 *)(p + 0xd9) = 0x28;
    p[0x49] = 4;
    slot = *(cm_slot_3c volatile *)(vtable + 0x3c);
    result = slot(self, 0xffffffff, (u32)(p + 0xa9), (u32)(p + 0x3a), zero);
    *(volatile u32 *)(p + 0xba) = result;
    return 1;
}
