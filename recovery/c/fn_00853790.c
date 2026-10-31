/* Preferred-image VA 0x00853790, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, copied to ESI on entry), plain ret.
   Stores word +0x3c = 2, bytes +0xc2 = 3, +0xc3 = 1, +0xc4 = 2, +0x42 = 1,
   +0xc5 = 4, +0xc6 = 1, +0xc7 = 2, +0xbe = 2, +0xbf = 2, +0xc0 = 0, +0xc1 = 4.
   Reads dword global 0x009CF624 and stores it at +0x1c, then -1 at +0x20.
   Stores byte +0x4a = 3, word +0xd9 = 0x43 and byte +0x49 = 7.
   Calls vtable slot +0x3c with ECX = object and stack arguments
   (-1, object+0xa9, object+0x3a, 0), stores that call's EAX at the
   unaligned dword +0xba, and returns 1 in EAX. Single direct caller
   0x008510a2; field and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_00853790_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

/* The first store is at object+0x3c, so a bad object faults there. The no-schedule
   attribute keeps -O2 from moving the vtable load ahead of the stores. */
u32 CM_THISCALL __attribute__((optimize("no-schedule-insns2"))) cm_fn_00853790(void *self)
{
    u8 *base = (u8 *)self;
    u32 global_value;
    u32 call_result;

    *(u16 *)(base + 0x3c) = 2;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 4;
    base[0xc6] = 1;
    base[0xc7] = 2;
    base[0xbe] = 2;
    base[0xbf] = 2;
    base[0xc0] = 0;
    base[0xc1] = 4;
    global_value = CM_GLOBAL(u32, 0x009cf624);
    *(u32 *)(base + 0x1c) = global_value;
    base[0x4a] = 3;
    *(u32 *)(base + 0x20) = 0xffffffff;
    *(u16 *)(base + 0xd9) = 0x43;
    base[0x49] = 7;
    call_result = (*(cm_fn_00853790_slot_t *)(*(u8 **)self + 0x3c))(
        self, -1, base + 0xa9, base + 0x3a, 0);
    *(u32 *)(base + 0xba) = call_result;
    return 1;
}
