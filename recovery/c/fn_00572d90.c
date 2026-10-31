/* Preferred-image VA 0x00572d90, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, read before any write), no stack
   arguments, plain ret. Stores word +0x3c = 2, bytes +0xc2 = 3, +0xc3 = 1,
   +0xc4 = 2, +0x42 = 1, +0xc5 = 1, +0xc6 = 2, +0xc7 = 3, +0xbe = 2, +0xbf = 4,
   +0xc0 = 0, +0xc1 = 3. It copies dword[0x009CF5BC] to +0x1c and dword[0x009CF5C4]
   to +0x20, stores byte +0x4a = 3 and byte +0x52 = 2. It then loads the vtable
   pointer from +0x00 and calls vtable slot +0x3c with ECX = object and stack
   arguments (-1, object+0xa9, object+0x3a, 0). The callee's cleanup is not visible
   in the caller (no add esp), so the slot is modelled as CM_THISCALL with four stack
   arguments. The call's EAX is stored at the unaligned dword +0xba, byte +0x49 = 5
   is stored before the call, and the function returns 1 in EAX. No direct callers
   read EDX; field meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_00572d90_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

/* The original stores before it loads the vtable pointer, so the store order is
   kept under -O2 to match the listing. */
u32 CM_THISCALL __attribute__((optimize("no-schedule-insns2"))) cm_fn_00572d90(void *self)
{
    u8 *base = (u8 *)self;
    u8 *vtable;
    u32 global_a;
    u32 global_b;

    *(u16 *)(base + 0x3c) = 2;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 1;
    base[0xc6] = 2;
    base[0xc7] = 3;
    base[0xbe] = 2;
    base[0xbf] = 4;
    base[0xc0] = 0;
    base[0xc1] = 3;
    global_a = CM_GLOBAL(u32, 0x009cf5bc);
    *(u32 *)(base + 0x1c) = global_a;
    global_b = CM_GLOBAL(u32, 0x009cf5c4);
    base[0x4a] = 3;
    *(u32 *)(base + 0x20) = global_b;
    base[0x52] = 2;
    vtable = *(u8 **)base;
    base[0x49] = 5;
    *(u32 *)(base + 0xba) = (*(cm_fn_00572d90_slot_t *)(vtable + 0x3c))(
        self, -1, base + 0xa9, base + 0x3a, 0);
    return 1;
}
