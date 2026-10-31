#include "cmre.h"

/* Preferred-image VA 0x007d2610, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, read before written), no stack
   arguments, plain `ret`, returns 1 in EAX. It stores word +0x3c = 2 and bytes
   +0xc2..+0xc7 = 3,1,2 / +0x42 = 1 / 1,4,1,2, bytes +0xbe..+0xc1 = 1,0,0,4,
   dword +0x1c = dword[0x009cf610], byte +0x52 = 2, dword +0x20 = -1, bytes
   +0x49 = 7 and +0x4a = 3. Then it calls vtable slot +0x3c with ECX = object and
   stack arguments (-1, object+0xa9, object+0x3a, 0), stores that call's EAX at
   the unaligned dword +0xba, and returns 1. Stores use volatile lvalues so the
   order and the first (faulting) address match the listing. Field and slot
   meanings remain unknown. */

typedef u32 (CM_THISCALL *cm_fn_007d2610_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_007d2610(void *self)
{
    u8 *obj = (u8 *)self;
    u8 *vtable;
    u32 global;
    u32 result;

    *(volatile u16 *)(obj + 0x3c) = 2;
    *(volatile u8 *)(obj + 0xc2) = 3;
    *(volatile u8 *)(obj + 0xc3) = 1;
    *(volatile u8 *)(obj + 0xc4) = 2;
    *(volatile u8 *)(obj + 0x42) = 1;
    *(volatile u8 *)(obj + 0xc5) = 4;
    *(volatile u8 *)(obj + 0xc6) = 1;
    *(volatile u8 *)(obj + 0xc7) = 2;
    *(volatile u8 *)(obj + 0xbe) = 1;
    *(volatile u8 *)(obj + 0xbf) = 0;
    *(volatile u8 *)(obj + 0xc0) = 0;
    *(volatile u8 *)(obj + 0xc1) = 4;
    global = CM_GLOBAL(u32, 0x009cf610);
    *(volatile u32 *)(obj + 0x1c) = global;
    *(volatile u8 *)(obj + 0x52) = 2;
    vtable = *(u8 *volatile *)obj;
    *(volatile u32 *)(obj + 0x20) = 0xffffffff;
    *(volatile u8 *)(obj + 0x49) = 7;
    *(volatile u8 *)(obj + 0x4a) = 3;
    result = (*(cm_fn_007d2610_slot_t *)(vtable + 0x3c))(
        self, -1, obj + 0xa9, obj + 0x3a, 0);
    *(volatile u32 *)(obj + 0xba) = result;
    return 1;
}
