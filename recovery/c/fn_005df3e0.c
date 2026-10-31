/* Preferred-image VA 0x005DF3E0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), plain ret. Stores, in order: word
   +0x3c = 2; bytes +0xc2..+0xc7 = 3,1,2 / +0x42 = 1 / 1,2,3; bytes +0xbe..+0xc1
   = 3,0,0,4; dword +0x1c = dword[0x009CF54C]. It then follows dword +0x04 to a
   record, that record's dword +0x5d to a second record, and tests bit 2 (mask
   4) of the second record's byte +0x11c: when set, dword +0x20 =
   dword[0x009CF564], otherwise -1. It stores byte +0x52 = 2 and +0x4a = 3,
   reads the vtable pointer at +0x00, stores byte +0x49 = 6 and calls vtable
   slot +0x3c with ECX = object and stack arguments (-1, object+0xa9,
   object+0x3a, 0); the slot pops its own arguments. The call's EAX is stored at
   the unaligned dword +0xba and 1 is returned.
   Accesses are volatile so -O2 keeps the listing's order at a fault.
   Field and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_005df3e0_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_005df3e0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile u8 *first;
    volatile u8 *second;
    u8 *vtable;

    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 1;
    base[0xc6] = 2;
    base[0xc7] = 3;
    base[0xbe] = 3;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 4;
    *(volatile u32 *)(base + 0x1c) = CM_GLOBAL(volatile u32, 0x009cf54c);
    first = *(u8 *volatile *)(base + 0x04);
    second = *(u8 *volatile *)(first + 0x5d);
    if ((second[0x11c] & 4) != 0) {
        *(volatile u32 *)(base + 0x20) = CM_GLOBAL(volatile u32, 0x009cf564);
    } else {
        *(volatile u32 *)(base + 0x20) = 0xffffffff;
    }
    base[0x52] = 2;
    base[0x4a] = 3;
    vtable = *(u8 *volatile *)base;
    base[0x49] = 6;
    *(volatile u32 *)(base + 0xba) = (*(cm_fn_005df3e0_slot_t *)(vtable + 0x3c))(
        self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    return 1;
}
