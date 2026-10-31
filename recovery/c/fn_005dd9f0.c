/* Preferred-image VA 0x005DD9F0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: push esi; mov esi, ecx (object pointer, thiscall); no stack arguments,
   ret 0. Reads the word at obj+0x40 first and compares it with 0x7d0 (the flags are
   used later by the jne). Stores word 2 at +0x3c, bytes 3 at +0xc2, 1 at +0xc3,
   2 at +0xc4, 1 at +0x42, 1 at +0xc5, 2 at +0xc6, 0 at +0xc7, 2 at +0xbe, 0 at
   +0xbf and +0xc0; +0xc1 = 5 when the word at +0x40 equals 0x7d0, else 4.
   Then dword global 0x009CF550 is stored at obj+0x1c. The vtable pointer is read from
   obj+0 and the slot at +0x3c is called with ECX = object and four stack arguments
   pushed in the order 0, object+0x3a, object+0xa9, -1 (so arguments are -1,
   object+0xa9, object+0x3a, 0). No ESP fix-up follows, so the slot pops its own
   arguments and is modelled as thiscall. Dword -1 is stored at obj+0x20 and word
   0x42 at obj+0xd9 before the call. The call result is stored at obj+0xba; the
   routine returns 1 in EAX. Field meanings remain unknown. Object accesses are
   volatile so -O2 keeps the listing's store order and first fault address. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_005dd9f0_slot)(void *, u32, void *, void *, u32);

u32 CM_THISCALL cm_fn_005dd9f0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u32 flag;
    u32 global_value;
    u32 vtable;
    cm_fn_005dd9f0_slot slot;
    u32 result;

    flag = *(volatile u16 *)(base + 0x40);
    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 1;
    base[0xc6] = 2;
    base[0xc7] = 0;
    base[0xbe] = 2;
    base[0xbf] = 0;
    base[0xc0] = 0;
    if (flag == 0x7d0u) {
        base[0xc1] = 5;
    } else {
        base[0xc1] = 4;
    }
    global_value = CM_GLOBAL(u32, 0x009cf550);
    vtable = *(volatile u32 *)base;
    *(volatile u32 *)(base + 0x1c) = global_value;
    *(volatile u32 *)(base + 0x20) = 0xffffffffu;
    *(volatile u16 *)(base + 0xd9) = 0x42;
    slot = (cm_fn_005dd9f0_slot)(*(volatile u32 *)(vtable + 0x3c));
    result = slot(self, 0xffffffffu, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    *(volatile u32 *)(base + 0xba) = result;
    return 1;
}
