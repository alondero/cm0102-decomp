/* Preferred-image VA 0x00574B70, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, copied to ESI on entry), plain ret.
   Stores word +0x3c = 2, bytes +0xc2 = 3, +0xc3 = 1, +0xc4 = 2, word +0xab = 0,
   word +0xdb = 0, bytes +0x42 = 1, +0xc5 = 1, +0xc6 = 2, +0xc7 = 3,
   +0xbe = 0, +0xbf = 0, +0xc0 = 0, +0xc1 = 3, dword +0x1c = -1,
   byte +0x52 = 2. Reads dword global 0x009CF5C0 and stores it at +0x20.
   Calls vtable slot +0x3c with ECX = object and stack arguments
   (-1, object+0xa9, object+0x3a, 0), stores that call's EAX at the
   unaligned dword +0xba, and returns 1 in EAX. Single direct caller
   0x00573406; field and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_00574b70_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

/* The first store is at object+0x3c, so a bad object faults there. The no-schedule
   attribute keeps -O2 from moving the vtable load ahead of the stores. */
u32 CM_THISCALL __attribute__((optimize("no-schedule-insns2"))) cm_fn_00574b70(void *self)
{
    u8 *base = (u8 *)self;
    u32 global_value;
    u32 call_result;

    *(u16 *)(base + 0x3c) = 2;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    *(u16 *)(base + 0xab) = 0;
    *(u16 *)(base + 0xdb) = 0;
    base[0x42] = 1;
    base[0xc5] = 1;
    base[0xc6] = 2;
    base[0xc7] = 3;
    base[0xbe] = 0;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 3;
    *(u32 *)(base + 0x1c) = 0xffffffff;
    global_value = CM_GLOBAL(u32, 0x009cf5c0);
    base[0x52] = 2;
    *(u32 *)(base + 0x20) = global_value;
    call_result = (*(cm_fn_00574b70_slot_t *)(*(u8 **)self + 0x3c))(
        self, -1, base + 0xa9, base + 0x3a, 0);
    *(u32 *)(base + 0xba) = call_result;
    return 1;
}
