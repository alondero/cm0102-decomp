/* Preferred-image VA 0x0063B4C0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: ECX is the object pointer (thiscall). The routine stores constant bytes
   and words into fields of the object, then calls the vtable slot at +0x3c with
   ECX = object and four stack arguments pushed in the order 0, object+0x3a,
   object+0xa9, -1 (so arguments are -1, object+0xa9, object+0x3a, 0). The original
   does not clean the stack after the call, so the vtable callee pops its four
   stack arguments. The call result is stored at object+0xba and the routine returns 1
   in EAX. The vtable callee's behaviour is unknown and stays an explicit boundary.
   The object accesses are volatile so the original store and load order (a fault on
   the first store at object+0xc4 is observable) is kept at every optimisation level. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_0063b4c0_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_0063b4c0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    void *vtable;
    cm_fn_0063b4c0_slot_t slot;
    u32 result;

    base[0xc4] = 2;
    base[0xc7] = 2;
    vtable = *(void * volatile *)self;
    base[0xbe] = 0;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 0;
    base[0xc2] = 3;
    base[0xc8] = 3;
    *(volatile s32 *)(base + 0x1c) = -1;
    *(volatile s32 *)(base + 0x20) = -1;
    *(volatile u16 *)(base + 0x3c) = 1;
    base[0xc3] = 1;
    base[0x42] = 1;
    base[0xc5] = 4;
    base[0xc6] = 1;
    slot = *(cm_fn_0063b4c0_slot_t *)((u8 *)vtable + 0x3c);
    result = slot(self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    *(volatile u32 *)(base + 0xba) = result;
    return 1;
}
