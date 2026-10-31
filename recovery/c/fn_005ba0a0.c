/* Preferred-image VA 0x005ba0a0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall with no stack parameters (ECX is the object, plain ret 0).
   Stores word +0x3c = 2, bytes +0xc4 = 2, +0xc7 = 2, +0xc2 = 3, +0xc3 = 1,
   +0x42 = 1, +0xc5 = 4, +0xc6 = 1, bytes +0xbe..+0xc1 = 0. Then loads the
   vtable pointer from +0x00, stores dword +0x1c = dword[0x009CF624], dword
   +0x20 = -1 and word +0xd9 = 0x43. It calls vtable slot +0x3c with ECX =
   object and stack arguments (-1, object+0xa9, object+0x3a, 0), stores that
   call's EAX at the unaligned dword +0xba, and returns 1 in EAX.
   No EDX read by callers. Field and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_005ba0a0_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_005ba0a0(void *self)
{
    /* Volatile byte stores: -O2 merged the +0xc6 and +0xc7 byte stores into one
       word store, which moved the fault address. */
    u8 volatile *base = (u8 volatile *)self;
    u8 *vtable;
    u32 call_result;

    *(u16 volatile *)(base + 0x3c) = 2;
    base[0xc4] = 2;
    base[0xc7] = 2;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0x42] = 1;
    base[0xc5] = 4;
    base[0xc6] = 1;
    base[0xbe] = 0;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 0;
    vtable = *(u8 **)base;
    *(u32 *)(base + 0x1c) = CM_GLOBAL(u32, 0x009cf624);
    *(u32 *)(base + 0x20) = 0xffffffff;
    *(u16 *)(base + 0xd9) = 0x43;
    call_result = (*(cm_fn_005ba0a0_slot_t *)(vtable + 0x3c))(
        self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    *(u32 *)(base + 0xba) = call_result;
    return 1;
}
