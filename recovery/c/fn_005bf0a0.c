/* Preferred-image VA 0x005BF0A0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), no stack arguments, ret 0; returns 1
   in EAX. Stores byte/word fields of the object in listing order, reads two
   globals (dword 0x009CF5CC and 0x009CF5D4), then calls the vtable slot at
   [vtable+0x3C] as a thiscall with stack arguments -1, object+0xA9,
   object+0x3A and 0, and stores its result at dword +0xBA. The word at +0x40 is
   compared with 0x7D1 before any store. Object accesses are volatile so -O2
   keeps the listing's store order at a fault. Field meanings remain unknown;
   the vtable slot is an explicit boundary. */
#include "cmre.h"

typedef u32 CM_THISCALL cm_vslot_3c_fn(volatile u8 *self, u32 arg1, volatile u8 *arg2, volatile u8 *arg3, u32 arg4);

u32 CM_THISCALL cm_fn_005bf0a0(u8 *object)
{
    volatile u8 *self = (volatile u8 *)object;
    u32 matched;
    u32 vtable;
    u32 global_a;
    u32 global_b;
    u32 result;
    cm_vslot_3c_fn *slot;

    matched = *(volatile u16 *)(self + 0x40) == 0x7d1u;
    *(volatile u16 *)(self + 0x3c) = 2;
    self[0xc2] = 3;
    self[0xc3] = 1;
    self[0xc4] = 2;
    self[0x42] = 1;
    self[0xc5] = 1;
    self[0xc6] = 2;
    self[0xc7] = 3;
    if (matched) {
        self[0xbe] = 4;
        self[0xc1] = 2;
    } else {
        self[0xbe] = 3;
        self[0xc1] = 3;
    }
    self[0xbf] = 0;
    self[0xc0] = 0;
    global_a = CM_GLOBAL(u32, 0x009cf5cc);
    *(volatile u32 *)(self + 0x1c) = global_a;
    global_b = CM_GLOBAL(u32, 0x009cf5d4);
    self[0x4a] = 3;
    self[0x52] = 2;
    vtable = *(volatile u32 *)self;
    *(volatile u32 *)(self + 0x20) = global_b;
    self[0x49] = 5;
    *(volatile u16 *)(self + 0xd9) = 0x83;
    slot = *(cm_vslot_3c_fn **)(vtable + 0x3c);
    result = slot(self, 0xffffffffu, self + 0xa9, self + 0x3a, 0);
    *(volatile u32 *)(self + 0xba) = result;
    return 1;
}
