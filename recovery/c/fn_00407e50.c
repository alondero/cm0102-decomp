#include "cmre.h"

typedef u32 (CM_THISCALL *cm_slot_3c)(void *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);

u32 CM_THISCALL cm_fn_00407e50(void *self)
{
    volatile u8 *p = (volatile u8 *)self;
    u32 global_value;
    u8 *vtable;
    cm_slot_3c slot;
    u32 result;

    *(volatile u16 *)(p + 0x3c) = 1;
    p[0xc2] = 3;
    p[0xc3] = 1;
    p[0xc4] = 2;
    *(volatile u16 *)(p + 0xab) = 0;
    *(volatile u16 *)(p + 0xdb) = 0;
    p[0x42] = 1;
    p[0xc5] = 1;
    p[0xc6] = 2;
    p[0xc7] = 3;
    p[0xc8] = 0;
    p[0xbe] = 1;
    p[0xbf] = 0;
    p[0xc0] = 0;
    p[0xc1] = 0;
    global_value = (u32)CM_GLOBAL(volatile s32, 0x009cf654);
    *(volatile u16 *)(p + 0xd9) = 3;
    p[0x4a] = 3;
    *(volatile u32 *)(p + 0x1c) = global_value;
    vtable = *(u8 * volatile *)p;
    *(volatile u32 *)(p + 0x20) = 0xffffffff;
    p[0x49] = 5;
    slot = *(cm_slot_3c volatile *)(vtable + 0x3c);
    result = slot(self, 0xffffffff, (u32)(p + 0xa9), (u32)(p + 0x3a), 0);
    *(volatile u32 *)(p + 0xba) = result;
    return 1;
}
