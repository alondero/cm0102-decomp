/* Preferred-image VA 0x004A1FB0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object) with one stack argument (a source
   record pointer), ret 4; returns the object in EAX. Three calls to the helper at
   0x00417C10 take the object (or sub-object +0x18 / +0x20) in ECX, a scratch
   8-byte local on the stack and a source pointer (source, source+0x18,
   source+0x20). Between them the listing copies dwords, words and bytes from the
   source into the object at +0x08..+0x14 and +0x28..+0x3b. Accesses are volatile
   so the order matches the listing at a fault. Field meanings remain unknown. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);

u8 *CM_THISCALL cm_fn_004a1fb0(u8 *object, u8 *source)
{
    u8 local_8[8];
    u8 *self = object;
    u8 *src = source;

    cm_fn_00417c10((u32 *)self, (u32 *)local_8, (u16 *)src);
    *(volatile u32 *)(self + 0x08) = *(volatile u32 *)(src + 0x08);
    *(volatile u32 *)(self + 0x0c) = *(volatile u32 *)(src + 0x0c);
    *(volatile u32 *)(self + 0x10) = *(volatile u32 *)(src + 0x10);
    *(volatile u32 *)(self + 0x14) = *(volatile u32 *)(src + 0x14);
    cm_fn_00417c10((u32 *)(self + 0x18), (u32 *)local_8, (u16 *)(src + 0x18));
    src = src + 0x20;
    cm_fn_00417c10((u32 *)(self + 0x20), (u32 *)local_8, (u16 *)src);
    *(volatile u32 *)(self + 0x28) = *(volatile u32 *)(src + 0x08);
    *(volatile u16 *)(self + 0x2c) = *(volatile u16 *)(src + 0x0c);
    *(volatile u16 *)(self + 0x2e) = *(volatile u16 *)(src + 0x0e);
    *(volatile u8 *)(self + 0x30) = *(volatile u8 *)(src + 0x10);
    *(volatile u32 *)(self + 0x31) = *(volatile u32 *)(src + 0x11);
    *(volatile u32 *)(self + 0x35) = *(volatile u32 *)(src + 0x15);
    *(volatile u8 *)(self + 0x39) = *(volatile u8 *)(src + 0x19);
    *(volatile u8 *)(self + 0x3a) = *(volatile u8 *)(src + 0x1a);
    *(volatile u32 *)(self + 0x3b) = *(volatile u32 *)(src + 0x1b);
    return object;
}
