/* Preferred-image VA 0x00855CC0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), no stack arguments, ret 0; returns 1 in EAX.
   Stores constant bytes and words into the object, copies DAT_009cf624 to +0x1c,
   tests byte [*(this+4)+0x5d)+0x11c] & 4 to choose between DAT_009cf7b8 and -1 for
   +0x20, then calls the object's vtable slot +0x3c with (this, -1, this+0xa9,
   this+0x3a, 0) and stores the result at +0xba. The callee pops its own four
   stack arguments (no caller cleanup follows the call). Object accesses are
   volatile so -O2 keeps the listing's store order at a fault. Field meanings
   remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_slot_3c_fn)(u8 *self, s32 a1, u8 *a2, u8 *a3, u32 a4);

u32 CM_THISCALL cm_fn_00855cc0(u8 *object)
{
    volatile u8 *self = (volatile u8 *)object;
    u8 *ptr;
    u8 *vtable;
    cm_slot_3c_fn slot;
    u32 result;

    *(volatile u16 *)(self + 0x3c) = 2;
    self[0xc2] = 3;
    self[0xc3] = 1;
    self[0xc4] = 2;
    self[0x42] = 1;
    self[0xc5] = 4;
    self[0xc6] = 1;
    self[0xc7] = 2;
    self[0xc8] = 3;
    self[0xbe] = 3;
    self[0xbf] = 0;
    self[0xc0] = 0;
    self[0xc1] = 4;
    *(volatile u32 *)(self + 0x1c) = CM_GLOBAL(u32, 0x009cf624);

    ptr = *(u8 *volatile *)(self + 0x04);
    ptr = *(u8 *volatile *)(ptr + 0x5d);
    if ((*(volatile u8 *)(ptr + 0x11c) & 4) == 0) {
        *(volatile u32 *)(self + 0x20) = 0xffffffffu;
    } else {
        *(volatile u32 *)(self + 0x20) = CM_GLOBAL(u32, 0x009cf7b8);
    }

    self[0x52] = 2;
    self[0x4a] = 3;
    vtable = *(u8 *volatile *)self;
    *(volatile u16 *)(self + 0xd9) = 0x43;
    self[0x49] = 7;
    slot = *(cm_slot_3c_fn volatile *)(vtable + 0x3c);
    result = slot(object, -1, (u8 *)(self + 0xa9), (u8 *)(self + 0x3a), 0);
    *(volatile u32 *)(self + 0xba) = result;
    return 1;
}
