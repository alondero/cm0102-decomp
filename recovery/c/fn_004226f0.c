/* Preferred-image VA 0x004226F0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), no stack arguments, ret 0; returns 1 in EAX
   (held in EBX until the end). Stores constant bytes and words into the object,
   copies DAT_009cf5a0 to +0x1c, zeroes bytes +0xfe..+0x103, then calls the object's
   vtable slot +0x3c with (this, -1, this+0xa9, this+0x3a, 0) and stores the result
   at +0xba. The callee pops its own four stack arguments (no caller cleanup follows
   the call). Object accesses are volatile so -O2 keeps the listing's store order
   at a fault. Field meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_slot_3c_fn)(u8 *self, s32 a1, u8 *a2, u8 *a3, u32 a4);

u32 CM_THISCALL cm_fn_004226f0(u8 *object)
{
    volatile u8 *self = (volatile u8 *)object;
    u8 *vtable;
    cm_slot_3c_fn slot;
    u32 global_val;
    u32 result;

    *(volatile u16 *)(self + 0x3c) = 2;
    self[0xc2] = 3;
    self[0xc3] = 1;
    self[0xc4] = 2;
    self[0x42] = 1;
    self[0xc5] = 3;
    self[0xc6] = 1;
    self[0xc7] = 2;
    self[0xc8] = 0;
    self[0xbe] = 1;
    self[0xbf] = 0;
    self[0xc0] = 1;
    self[0xc1] = 2;
    global_val = CM_GLOBAL(u32, 0x009cf5a0);
    self[0x102] = 0;
    self[0x100] = 0;
    self[0xfe] = 0;
    self[0x103] = 0;
    self[0x101] = 0;
    self[0xff] = 0;
    *(volatile u32 *)(self + 0x1c) = global_val;
    self[0x4a] = 3;
    vtable = *(u8 *volatile *)self;
    *(volatile u32 *)(self + 0x20) = 0xffffffffu;
    self[0x49] = 4;
    *(volatile u16 *)(self + 0xd9) = 0x43;
    slot = *(cm_slot_3c_fn volatile *)(vtable + 0x3c);
    result = slot(object, -1, (u8 *)(self + 0xa9), (u8 *)(self + 0x3a), 0);
    *(volatile u32 *)(self + 0xba) = result;
    return 1;
}
