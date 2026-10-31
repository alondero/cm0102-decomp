/* Preferred-image VA 0x00635aa0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, read before any write), no stack
   arguments, plain ret. Stores word +0x3c = 4, bytes +0xc2..+0xc7 = 3,1,2,1,1,2,
   bytes +0xbe..+0xc1 = 1,1,0,0, byte +0x52 = 2, byte +0x4a = 3, dword +0x1c =
   dword[0x009CF80C], dword +0x20 = -1 and byte +0x49 = 5. It then loads the
   vtable pointer from +0x00 and calls vtable slot +0x3c with ECX = object and
   stack arguments (-1, object+0xa9, object+0x3a, 0). The callee's cleanup is not
   visible in the caller (no add esp), so the slot is modelled as CM_THISCALL with
   four stack arguments. The call's EAX is stored at the unaligned dword +0xba and
   the function returns 1 in EAX. No direct callers read EDX; field meanings remain
   unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_00635aa0_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

/* The original stores before it loads the vtable pointer, so the store order is
   kept under -O2 to match the listing. */
u32 CM_THISCALL __attribute__((optimize("no-schedule-insns2"))) cm_fn_00635aa0(void *self)
{
    u8 *base = (u8 *)self;
    u8 *vtable;
    u32 global_value;

    *(u16 *)(base + 0x3c) = 4;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 1;
    base[0xc6] = 2;
    base[0xc7] = 0;
    base[0xbe] = 1;
    base[0xbf] = 1;
    base[0xc0] = 0;
    base[0xc1] = 0;
    global_value = CM_GLOBAL(u32, 0x009cf80c);
    base[0x52] = 2;
    base[0x4a] = 3;
    vtable = *(u8 **)base;
    *(u32 *)(base + 0x1c) = global_value;
    *(u32 *)(base + 0x20) = 0xffffffff;
    base[0x49] = 5;
    *(u32 *)(base + 0xba) = (*(cm_fn_00635aa0_slot_t *)(vtable + 0x3c))(
        self, -1, base + 0xa9, base + 0x3a, 0);
    return 1;
}
