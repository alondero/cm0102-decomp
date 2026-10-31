/* Preferred-image VA 0x00926800, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: ECX is the object pointer (thiscall); push ebx; push esi; mov esi, ecx.
   Stores constant bytes and words into fields of the object, stores -1 to dwords
   at +0x1c and +0x20, stores 100000 at +0x28, then calls the vtable slot at +0x3c
   (vtable pointer read from obj+0) with ECX = object and four stack arguments pushed
   in the order 0, object+0x3a, object+0xa9, -1 (so arguments are -1, object+0xa9,
   object+0x3a, 0). No ESP fix-up follows the call, so the vtable callee pops its four
   stack arguments. The vtable pointer is read after the stores to +0xc4 and +0xc5 and
   before the store to +0xc8, so a bad object pointer faults in the same place as the
   original. Accesses are volatile to keep that order under optimisation.
   The call result is stored at object+0xba; the routine returns 1 in EAX with ret 0.
   The vtable callee's behaviour is unknown and stays an explicit boundary. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_00926800_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_00926800(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    void *vtable;
    cm_fn_00926800_slot_t slot;
    u32 result;

    base[0xc4] = 4;
    base[0xc5] = 4;
    vtable = *(void * volatile *)self;
    base[0xc8] = 0;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 0;
    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc7] = 2;
    *(volatile s32 *)(base + 0x1c) = -1;
    *(volatile s32 *)(base + 0x20) = -1;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc6] = 1;
    base[0xbe] = 1;
    base[0x42] = 1;
    *(volatile s32 *)(base + 0x28) = 100000;
    slot = (cm_fn_00926800_slot_t)(*(void * volatile *)((u8 *)vtable + 0x3c));
    result = slot(self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    *(volatile u32 *)(base + 0xba) = result;
    return 1;
}
