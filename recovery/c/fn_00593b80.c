/* Preferred-image VA 0x00593B80, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: ECX is the object pointer (thiscall); push ebx; push esi; mov esi, ecx.
   Stores constant bytes and a word into fields of the object, copies the dword
   global at 0x009CF7E8 into +0x1c, stores -1 to the dword at +0x20, then calls the
   vtable slot at +0x3c (vtable pointer read from obj+0 after the store to +0x52)
   with ECX = object and four stack arguments pushed in the order 0, object+0x3a,
   object+0xa9, -1 (so arguments are -1, object+0xa9, object+0x3a, 0).
   No ESP fix-up follows the call, so the vtable callee pops its four stack arguments.
   The call result is stored at object+0xba; the routine returns 1 in EAX with ret 0.
   Accesses are volatile to keep the listing's store order under optimisation.
   The vtable callee's behaviour is unknown and stays an explicit boundary. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_00593b80_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_00593b80(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    void *vtable;
    cm_fn_00593b80_slot_t slot;
    s32 global_value;
    u32 result;

    *(volatile u16 *)(base + 0x3c) = 3;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 1;
    base[0xc6] = 2;
    base[0xc7] = 3;
    base[0xbe] = 2;
    base[0xbf] = 0;
    base[0xc0] = 1;
    base[0xc1] = 2;
    global_value = CM_GLOBAL(s32, 0x009cf7e8);
    base[0x4a] = 3;
    *(volatile s32 *)(base + 0x1c) = global_value;
    base[0x52] = 2;
    vtable = *(void * volatile *)self;
    *(volatile s32 *)(base + 0x20) = -1;
    *(volatile u16 *)(base + 0xd9) = 8;
    base[0x49] = 7;
    slot = (cm_fn_00593b80_slot_t)(*(void * volatile *)((u8 *)vtable + 0x3c));
    result = slot(self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    *(volatile u32 *)(base + 0xba) = result;
    return 1;
}
