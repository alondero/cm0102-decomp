/* Preferred-image VA 0x0078D0D0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: push ebx; push esi; mov esi, ecx (object pointer). Stores constant
   bytes and words into the object, pushes 0, &obj+0x3a, &obj+0xa9 and -1, then
   calls the object's vtable slot at +0x3c (vtable pointer read from obj+0).
   The callee's result is stored as a dword at obj+0xba. Returns EAX = 1.
   Sibling of 0x005F3E70 with a different set of constant stores and dword
   of 0x186A0 at obj+0x28. The vtable callee's identity and cleanup are not known
   from this function; the four pushed arguments and the absence of an ESP fix-up
   after the call imply the slot pops its own 16 bytes; ECX = object at the call, so the slot is
   modelled as thiscall.
   Field meanings remain unknown. Ret 0 stack bytes.
   Object stores and the vtable read are volatile so that -O2 keeps the listing's
   store order: a fault at the first store must be the same access as original. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_0078d0d0_slot)(void *, u32, void *, void *, u32);

u32 CM_THISCALL cm_fn_0078d0d0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u32 vtable;
    cm_fn_0078d0d0_slot slot;
    u32 result;

    *(volatile u8 *)(base + 0xc4) = 4;
    *(volatile u8 *)(base + 0xc7) = 4;
    *(volatile u8 *)(base + 0xc8) = 0;
    *(volatile u8 *)(base + 0xbf) = 0;
    *(volatile u8 *)(base + 0xc0) = 0;
    *(volatile u8 *)(base + 0xc1) = 0;
    *(volatile u8 *)(base + 0xc6) = 2;
    *(volatile u8 *)(base + 0xbe) = 2;
    *(volatile s32 *)(base + 0x1c) = -1;
    *(volatile s32 *)(base + 0x20) = -1;
    *(volatile u16 *)(base + 0x3c) = 1;
    *(volatile u8 *)(base + 0xc2) = 3;
    *(volatile u8 *)(base + 0xc3) = 1;
    *(volatile u8 *)(base + 0xc5) = 1;
    *(volatile u8 *)(base + 0x42) = 1;
    *(volatile u32 *)(base + 0x28) = 100000;

    vtable = *(volatile u32 *)base;
    slot = (cm_fn_0078d0d0_slot)(*(volatile u32 *)(vtable + 0x3c));
    result = slot(self, 0xffffffffu, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    *(volatile u32 *)(base + 0xba) = result;
    return 1;
}
