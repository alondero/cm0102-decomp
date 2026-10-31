/* Preferred-image VA 0x0066C870, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: ECX is the object pointer (thiscall); push esi; mov esi, ecx.
   Stores constant bytes and words into fields of the object, stores -1 to dwords
   at +0x1c and +0x20, stores 100000 at +0x28, then calls the vtable slot at +0x3c
   (vtable pointer read from obj+0) with ECX = object and four stack arguments pushed
   in the order 0, object+0x3a, object+0xa9, -1 (so arguments are -1, object+0xa9,
   object+0x3a, 0). No ESP fix-up follows the call, so the vtable callee pops its four
   stack arguments. The call result is stored at object+0xba; the routine returns 1
   in EAX (mov eax, 1) with ret 0. The vtable callee's behaviour is unknown and stays
   an explicit boundary. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_0066c870_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_0066c870(void *self)
{
    u8 *base = (u8 *)self;
    void *vtable;
    cm_fn_0066c870_slot_t slot;
    u32 result;

    vtable = *(void **)self;
    base[0xc7] = 0;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 0;
    *(u16 *)(base + 0x3c) = 2;
    base[0xc6] = 2;
    base[0xbe] = 2;
    *(s32 *)(base + 0x1c) = -1;
    *(s32 *)(base + 0x20) = -1;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 4;
    base[0xc5] = 1;
    base[0x42] = 1;
    *(u16 *)(base + 0xab) = 0x83;
    *(s32 *)(base + 0x28) = 100000;
    slot = (cm_fn_0066c870_slot_t)(*(void **)((u8 *)vtable + 0x3c));
    result = slot(self, -1, base + 0xa9, base + 0x3a, 0);
    *(u32 *)(base + 0xba) = result;
    return 1;
}
