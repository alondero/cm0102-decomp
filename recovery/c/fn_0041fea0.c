/* Preferred-image VA 0x0041FEA0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, esi), plain ret (no stack bytes popped
   by this routine). Stores, in order: bytes +0xc2 = 3, +0xc5 = 3; word +0x3c = 2;
   bytes +0xc3 = 1, +0xc4 = 2, +0x42 = 1, +0xc6 = 1, +0xc7 = 2, +0xc8 = 0,
   +0xbe = 1, +0xbf = 0, +0xc0 = 1, +0xc1 = 2. Then dword +0x1c = dword
   [0x009CF59C] (read at that point), bytes +0xf8, +0xf7, +0xf6 = 0, byte +0x52 = 2,
   dword +0x20 = dword [0x009CF7C4] (read before the +0xf8 stores). It reads the
   vtable pointer at +0x00, stores word +0xd9 = 0x43, and calls vtable slot +0x3c
   with ECX = object and four stack arguments pushed in the order 0, object+0x3a,
   object+0xa9, -1 (so the callee sees -1, object+0xa9, object+0x3a, 0). The slot
   pops its own arguments, as in fn_0042a5a0. The call's EAX is stored at the
   unaligned dword +0xba and 1 is returned.
   Accesses are volatile so -O2 keeps the listing's order at a fault.
   Field, global and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_0041fea0_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_0041fea0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u32 global_a;
    u32 global_b;
    u8 *vtable;

    base[0xc2] = 3;
    base[0xc5] = 3;
    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc6] = 1;
    base[0xc7] = 2;
    base[0xc8] = 0;
    base[0xbe] = 1;
    base[0xbf] = 0;
    base[0xc0] = 1;
    base[0xc1] = 2;
    global_a = CM_GLOBAL(volatile u32, 0x009cf59c);
    *(volatile u32 *)(base + 0x1c) = global_a;
    global_b = CM_GLOBAL(volatile u32, 0x009cf7c4);
    base[0xf8] = 0;
    base[0xf7] = 0;
    base[0xf6] = 0;
    base[0x52] = 2;
    *(volatile u32 *)(base + 0x20) = global_b;
    vtable = *(u8 *volatile *)base;
    *(volatile u16 *)(base + 0xd9) = 0x43;
    *(volatile u32 *)(base + 0xba) = (*(cm_fn_0041fea0_slot_t *)(vtable + 0x3c))(
        self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    return 1;
}
