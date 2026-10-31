/* Preferred-image VA 0x007EEE50, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, esi), plain ret. The word at +0x40 is
   compared with 0x07CF before any store, and that flag picks one of two store
   sets. Stores, in order: word +0x3c = 4; bytes +0xc2 = 3, +0xc3 = 1, +0xc4 = 2,
   +0x42 = 1, +0xc5 = 1, +0xc6 = 2, +0xc7 = 3, +0xbe = 1. Then:
   - if the flag equals 0x07CF: byte +0xbf = 2, +0xc0 = 0, +0xc1 = 0;
   - otherwise: byte +0xc1 = 2, +0xbf = 0, +0xc0 = 0.
   Then dword +0x1c = dword [0x009CF5DC] (read at that point), dword +0x20 =
   dword [0x009CF5E4], byte +0x4a = 3. It reads the vtable pointer at +0x00,
   stores byte +0x49 = 5, and calls vtable slot +0x3c with ECX = object and four
   stack arguments pushed in the order 0, object+0x3a, object+0xa9, -1 (so the
   callee sees -1, object+0xa9, object+0x3a, 0). The slot pops its own arguments,
   as in fn_0042a5a0. The call's EAX is stored at the unaligned dword +0xba and
   1 is returned.
   Accesses are volatile so -O2 keeps the listing's order at a fault.
   Field, global and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_007eee50_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_007eee50(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u16 flag;
    u32 global_a;
    u32 global_b;
    u8 *vtable;

    flag = *(volatile u16 *)(base + 0x40);
    *(volatile u16 *)(base + 0x3c) = 4;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 1;
    base[0xc6] = 2;
    base[0xc7] = 3;
    base[0xbe] = 1;
    if (flag == 0x07cf) {
        base[0xbf] = 2;
        base[0xc0] = 0;
        base[0xc1] = 0;
    } else {
        base[0xc1] = 2;
        base[0xbf] = 0;
        base[0xc0] = 0;
    }
    global_a = CM_GLOBAL(volatile u32, 0x009cf5dc);
    *(volatile u32 *)(base + 0x1c) = global_a;
    global_b = CM_GLOBAL(volatile u32, 0x009cf5e4);
    *(volatile u32 *)(base + 0x20) = global_b;
    base[0x4a] = 3;
    vtable = *(u8 *volatile *)base;
    base[0x49] = 5;
    *(volatile u32 *)(base + 0xba) = (*(cm_fn_007eee50_slot_t *)(vtable + 0x3c))(
        self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    return 1;
}
