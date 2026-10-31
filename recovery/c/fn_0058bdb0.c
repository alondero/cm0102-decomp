/* Preferred-image VA 0x0058BDB0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), no stack arguments, ret 0; returns 1
   in EAX. Stores byte and word fields of the object in listing order, calls the
   vtable slot at [vtable+0x3C] as a thiscall with stack arguments -1,
   object+0xA9, object+0x3A and 0 (result stored at dword +0xBA), then calls
   00549F70 as a thiscall with ECX pointing at an 8-byte local, and stack
   arguments 0x14, 1, (result high word | word +0x40) and 5. The callee pops its
   own arguments. Finally it stores 0 at word +0xB8 and the first word of the
   local at word +0xB6. Field meanings remain unknown; both calls are explicit
   boundaries. Object accesses are volatile. */
#include "cmre.h"

typedef u32 CM_THISCALL cm_vslot_3c_fn(volatile u8 *self, u32 arg1, volatile u8 *arg2, volatile u8 *arg3, u32 arg4);

extern /* WARNING: Removing unreachable block (ram,0x0054a080) */ /* WARNING: Removing unreachable block (ram,0x0054a093) */ short * CM_THISCALL cm_fn_00549f70(short *param_1,short param_2,char param_3,u32 param_4,u32 param_5);

u32 CM_THISCALL cm_fn_0058bdb0(u8 *object)
{
    volatile u8 *self = (volatile u8 *)object;
    u16 buffer[4];
    cm_vslot_3c_fn *slot;
    u32 vtable;
    u32 result;
    u16 word40;
    u16 first_word;

    self[0xc2] = 3;
    self[0x4a] = 3;
    self[0x43] = 2;
    self[0xc7] = 2;
    self[0xbe] = 2;
    vtable = *(volatile u32 *)self;
    *(volatile u16 *)(self + 0x3c) = 1;
    self[0xc3] = 1;
    self[0xc4] = 0x0a;
    self[0xc5] = 4;
    self[0xc6] = 1;
    self[0xbf] = 0;
    self[0xc0] = 0;
    self[0xc1] = 0;
    self[0x49] = 7;
    *(volatile u16 *)(self + 0xd9) = 0x28;
    slot = *(cm_vslot_3c_fn **)(vtable + 0x3c);
    result = slot(self, 0xffffffffu, self + 0xa9, self + 0x3a, 0);
    *(volatile u32 *)(self + 0xba) = result;
    word40 = *(volatile u16 *)(self + 0x40);
    cm_fn_00549f70((short *)buffer, 0x14, 1, (result & 0xffff0000u) | word40, 5);
    first_word = buffer[0];
    *(volatile u16 *)(self + 0xb8) = 0;
    *(volatile u16 *)(self + 0xb6) = first_word;
    return 1;
}
