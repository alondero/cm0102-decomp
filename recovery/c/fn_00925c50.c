/* Preferred-image VA 0x00925C50, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), plain ret. Reads the vtable pointer at
   +0x00 first, then stores, in order: words +0xab and +0xdb = 0; bytes +0xbe,
   +0xbf, +0xc0, +0xc1, +0x52 = 0; bytes +0xc2, +0xc7, +0x4a = 3; word +0x3c = 2;
   bytes +0xc4, +0xc6 = 2; bytes +0xc3, +0x42, +0xc5 = 1; dwords +0x1c and +0x20
   = -1; byte +0x49 = 5. It calls vtable slot +0x3c with ECX = object and stack
   arguments (-1, object+0xa9, object+0x3a, 0); the slot pops its own arguments.
   The call's EAX is stored at the unaligned dword +0xba and 1 is returned.
   Accesses are volatile so -O2 keeps the listing's order at a fault.
   Field and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_00925c50_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_00925c50(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u8 *vtable = *(u8 *volatile *)base;

    *(volatile u16 *)(base + 0xab) = 0;
    *(volatile u16 *)(base + 0xdb) = 0;
    base[0xbe] = 0;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 0;
    base[0x52] = 0;
    base[0xc2] = 3;
    base[0xc7] = 3;
    base[0x4a] = 3;
    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc4] = 2;
    base[0xc6] = 2;
    base[0xc3] = 1;
    base[0x42] = 1;
    base[0xc5] = 1;
    *(volatile u32 *)(base + 0x1c) = 0xffffffff;
    *(volatile u32 *)(base + 0x20) = 0xffffffff;
    base[0x49] = 5;
    *(volatile u32 *)(base + 0xba) = (*(cm_fn_00925c50_slot_t *)(vtable + 0x3c))(
        self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    return 1;
}
