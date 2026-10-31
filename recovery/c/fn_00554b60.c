#include "cmre.h"

typedef u32 (CM_THISCALL *cm_slot_3c)(void *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);

u32 CM_THISCALL cm_fn_00554b60(void *self)
{
    volatile u8 *p = (volatile u8 *)self;
    u32 two = 2;
    u32 three = 3;
    u32 four = 4;
    u32 zero = 0;
    u32 global_value;
    u8 *vtable;
    cm_slot_3c slot;
    u32 result;

    *(volatile u16 *)(p + 0x3c) = (u16)two;
    p[0xc4] = (u8)two;
    p[0xc6] = (u8)two;
    p[0xc2] = (u8)three;
    p[0xc3] = 1;
    *(volatile u16 *)(p + 0xab) = (u16)zero;
    *(volatile u16 *)(p + 0xdb) = (u16)zero;
    p[0x42] = 1;
    p[0xc5] = 1;
    p[0xc7] = (u8)zero;
    p[0xbe] = (u8)three;
    p[0xbf] = (u8)zero;
    p[0xc0] = (u8)zero;
    p[0xc1] = (u8)four;
    global_value = (u32)CM_GLOBAL(volatile s32, 0x009cf5b0);
    p[0x4a] = (u8)three;
    p[0x49] = (u8)four;
    vtable = *(u8 * volatile *)p;
    *(volatile u32 *)(p + 0x1c) = global_value;
    *(volatile u32 *)(p + 0x20) = 0xffffffff;
    slot = *(cm_slot_3c volatile *)(vtable + 0x3c);
    result = slot(self, 0xffffffff, (u32)(p + 0xa9), (u32)(p + 0x3a), zero);
    *(volatile u32 *)(p + 0xba) = result;
    return 1;
}
