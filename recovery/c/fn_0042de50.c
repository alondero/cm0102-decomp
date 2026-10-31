/* Preferred-image VA 0x0042DE50, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), plain ret. Stores, in order: word
   +0x3c = 2; bytes +0xc2..+0xc7 = 3,1,2 / +0x42 = 1 / 1,2,4; bytes +0xbe..+0xc1
   = 2,0,0,2. It reads dword[0x009CF9E0], stores byte +0x4a = 3 and byte +0x52
   = 2, reads the vtable pointer at +0x00, then stores dword +0x1c = the global
   value, dword +0x20 = -1, word +0xd9 = 8 and byte +0x49 = 7. It calls vtable
   slot +0x3c with ECX = object and stack arguments (-1, object+0xa9,
   object+0x3a, 0); the slot pops its own arguments. The call's EAX is stored at
   the unaligned dword +0xba and 1 is returned.
   Accesses are volatile so -O2 keeps the listing's order at a fault.
   Field and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_0042de50_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_0042de50(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u8 *vtable;
    u32 global;

    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 1;
    base[0xc6] = 2;
    base[0xc7] = 4;
    base[0xbe] = 2;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 2;
    global = CM_GLOBAL(volatile u32, 0x009cf9e0);
    base[0x4a] = 3;
    base[0x52] = 2;
    vtable = *(u8 *volatile *)base;
    *(volatile u32 *)(base + 0x1c) = global;
    *(volatile u32 *)(base + 0x20) = 0xffffffff;
    *(volatile u16 *)(base + 0xd9) = 8;
    base[0x49] = 7;
    *(volatile u32 *)(base + 0xba) = (*(cm_fn_0042de50_slot_t *)(vtable + 0x3c))(
        self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    return 1;
}
