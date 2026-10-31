#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_005523b0_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, u32 arg4);

u32 CM_THISCALL cm_fn_005523b0(void *object)
{
    volatile u8 *self = (volatile u8 *)object;
    u32 vtable;
    cm_fn_005523b0_slot_t slot;
    u32 result;

    *(volatile u16 *)(self + 0x3c) = 2;
    self[0xc2] = 3;
    self[0xc3] = 1;
    self[0xc4] = 2;
    *(volatile u16 *)(self + 0xab) = 0;
    *(volatile u16 *)(self + 0xdb) = 0;
    self[0x42] = 1;
    self[0xc5] = 1;
    self[0xc6] = 2;
    self[0xc7] = 0;
    self[0xbe] = 2;
    self[0xbf] = 0;
    self[0xc0] = 0;
    self[0xc1] = 3;
    *(volatile u32 *)(self + 0x1c) = CM_GLOBAL(volatile u32, 0x009cf5ac);
    *(volatile u32 *)(self + 0x20) = CM_GLOBAL(volatile u32, 0x009cf5b4);
    self[0x4a] = 3;
    vtable = *(volatile u32 *)self;
    self[0x49] = 4;
    slot = *(cm_fn_005523b0_slot_t volatile *)(vtable + 0x3c);
    result = slot(object, -1, (void *)(self + 0xa9), (void *)(self + 0x3a), 0);
    *(volatile u32 *)(self + 0xba) = result;
    return 1;
}

