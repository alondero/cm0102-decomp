/* Preferred-image VA 0x007F3F80, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: ECX is the object (read before written), no stack arguments of its own,
   ret 0, dword result in EAX (the function itself returns 1).
   - Word +0x3c = 4; bytes +0xc2 = 3, +0xc3 = 1, +0xc4 = 2, +0x42 = 1,
     +0xc5 = 1, +0xc6 = 2, +0xc7 = 3, +0xbe = 2.
   - Word +0x40 compared with 0x07cf: equal -> bytes +0xbf, +0xc0, +0xc1 = 0;
     not equal -> byte +0xc1 = 2, then +0xbf = 0, +0xc0 = 0.
   - Dwords +0x1c and +0x20 = globals [0x009CF5E0] and [0x009CF5E8].
   - Byte +0x4a = 3, byte +0x49 = 5.
   - Indirect call through the vtable slot at +0x3c: a thiscall with the object as
     its first parameter and four stack arguments, pushed in this order by the
     listing: (-1, object+0xa9, object+0x3a, 0). The callee pops its own arguments
     (ret 0x10 is assumed by the listing's stack layout; the gate observes it).
   - Dword +0xba = the call's EAX result; return 1.
   Field meanings remain unknown. Global meanings remain unknown.
   Object accesses are volatile so -O2 keeps the listing's store order. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_slot_3c_t)(u8 *self, s32 arg1, u8 *arg2, u8 *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_007f3f80(u8 *object)
{
    volatile u8 *self = (volatile u8 *)object;
    cm_slot_3c_t slot;
    u32 result;
    u32 vtable;
    s32 word40_is_cf;

    /* The listing compares word +0x40 before any store; keep that read first. */
    word40_is_cf = (*(volatile u16 *)(self + 0x40) == 0x07cf);
    *(volatile u16 *)(self + 0x3c) = 4;
    self[0xc2] = 3;
    self[0xc3] = 1;
    self[0xc4] = 2;
    self[0x42] = 1;
    self[0xc5] = 1;
    self[0xc6] = 2;
    self[0xc7] = 3;
    self[0xbe] = 2;
    if (word40_is_cf) {
        self[0xbf] = 0;
        self[0xc0] = 0;
        self[0xc1] = 0;
    } else {
        self[0xc1] = 2;
        self[0xbf] = 0;
        self[0xc0] = 0;
    }
    *(volatile u32 *)(self + 0x1c) = CM_GLOBAL(u32, 0x009cf5e0);
    *(volatile u32 *)(self + 0x20) = CM_GLOBAL(u32, 0x009cf5e8);
    self[0x4a] = 3;
    vtable = *(volatile u32 *)object;
    self[0x49] = 5;
    /* The slot read at vtable+0x3c happens at the call, after the byte +0x49 store. */
    slot = (cm_slot_3c_t)*(volatile u32 *)(vtable + 0x3c);
    result = slot(object, -1, object + 0xa9, object + 0x3a, 0);
    *(volatile u32 *)(self + 0xba) = result;
    return 1;
}
