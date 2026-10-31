/* Preferred-image VA 0x00892380, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), plain ret. Reads the vtable pointer at
   +0x00, then stores word +0x3c = 2 and bytes +0xc2..+0xc7 = 3,1,2 / +0x42 = 1 /
   1,2,0, bytes +0xbe..+0xc1 = 0,1,1,2, dword +0x1c = dword[0x009CF5F0],
   dword +0x20 = -1 and word +0xd9 = 0x42. It then calls vtable slot +0x3c with
   ECX = object and stack arguments (-1, object+0xa9, object+0x3a, 0), stores
   that call's EAX at the unaligned dword +0xba, and returns 1 in EAX.
   No direct callers; field and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_00892380_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

/* The original loads the vtable before any store, so a bad object faults on that
   read. Volatile alone did not stop -O2 scheduling the +0x3c store first. */
u32 CM_THISCALL __attribute__((optimize("no-schedule-insns2"))) cm_fn_00892380(void *self)
{
    u8 *base = (u8 *)self;
    u8 *vtable = *(u8 *volatile *)base;

    *(u16 *)(base + 0x3c) = 2;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 1;
    base[0xc6] = 2;
    base[0xc7] = 0;
    base[0xbe] = 0;
    base[0xbf] = 1;
    base[0xc0] = 1;
    base[0xc1] = 2;
    *(u32 *)(base + 0x1c) = CM_GLOBAL(u32, 0x009cf5f0);
    *(u32 *)(base + 0x20) = 0xffffffff;
    *(u16 *)(base + 0xd9) = 0x42;
    *(u32 *)(base + 0xba) = (*(cm_fn_00892380_slot_t *)(vtable + 0x3c))(
        self, -1, base + 0xa9, base + 0x3a, 0);
    return 1;
}
