/* Preferred-image VA 0x0078BD70, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, saved in ESI), no stack arguments, returns
   1 in EAX from EBX. Stores word +0x3c = 4, byte +0xc7 = 4, +0xc2 = 3, +0xc3 = 1,
   +0xc4 = 2, +0x42 = 1, +0xc5 = 1, +0xc6 = 2, +0xbe = 1, +0xbf = 1, +0xc0 = 0,
   +0xc1 = 0, +0x49 = 3, +0x4a = 3, +0x52 = 2, dword +0x1c = dword[0x009CF8B0]
   and dword +0x20 = -1. It then loads the vtable pointer at +0x00 and calls vtable
   slot +0x3c with ECX = object and stack arguments (-1, object+0xa9,
   object+0x3a, 0), stores that call's EAX at the unaligned dword +0xba.
   Field and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_0078bd70_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

/* The original stores into the object before loading the vtable, so a bad object
   faults on the first store. The vtable load is volatile and scheduling is off to
   keep the original store and load order (see fn_00892380.c). */
u32 CM_THISCALL __attribute__((optimize("no-schedule-insns2"))) cm_fn_0078bd70(void *self)
{
    u8 *base = (u8 *)self;
    u8 *vtable;

    *(u16 *)(base + 0x3c) = 4;
    base[0xc7] = 4;
    base[0xc2] = 3;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 1;
    base[0xc6] = 2;
    base[0xbe] = 1;
    base[0xbf] = 1;
    base[0xc0] = 0;
    base[0xc1] = 0;
    base[0x49] = 3;
    base[0x4a] = 3;
    *(u32 *)(base + 0x1c) = CM_GLOBAL(u32, 0x009cf8b0);
    *(u32 *)(base + 0x20) = 0xffffffff;
    base[0x52] = 2;
    vtable = *(u8 *volatile *)base;
    *(u32 *)(base + 0xba) = (*(cm_fn_0078bd70_slot_t *)(vtable + 0x3c))(
        self, -1, base + 0xa9, base + 0x3a, 0);
    return 1;
}
