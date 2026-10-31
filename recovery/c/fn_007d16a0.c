/* Preferred-image VA 0x007D16A0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), plain ret. Stores, in order: word
   +0x3c = 2; byte +0xc2 = 3; word +0xa9 = 0x22; byte +0xc3 = 1; byte +0xc4 = 2;
   byte +0x42 = 1; byte +0xc5 = 4; byte +0xc6 = 1; byte +0xc7 = 2; byte +0xc8 = 3;
   byte +0xbe = 3; byte +0xbf = 0; byte +0xc0 = 0; byte +0xc1 = 3. It reads
   dword[0x009CF60C] into
   +0x1c, then follows the pointer at +0x04 and then the pointer at +0x5d of that
   record. If byte +0x11c of that record has bit 2 set, dword[0x009CF7C0] is
   stored at +0x20; otherwise +0x20 = 0xFFFFFFFF. Then byte +0x4a = 3 and byte
   +0x52 = 2, the vtable pointer at +0x00 is read, byte +0x49 = 7, and vtable
   slot +0x3c is called with ECX = object and stack arguments (-1, object+0xa9,
   object+0x3a, 0); the slot pops its own arguments. The call's EAX is stored at
   the unaligned dword +0xba and 1 is returned.
   Accesses are volatile so -O2 keeps the listing's order at a fault.
   Field and slot meanings remain unknown. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_fn_007d16a0_slot_t)(void *self, s32 arg1, void *arg2, void *arg3, s32 arg4);

u32 CM_THISCALL cm_fn_007d16a0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u8 *vtable;
    u8 *chain;

    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc2] = 3;
    *(volatile u16 *)(base + 0xa9) = 0x22;
    base[0xc3] = 1;
    base[0xc4] = 2;
    base[0x42] = 1;
    base[0xc5] = 4;
    base[0xc6] = 1;
    base[0xc7] = 2;
    base[0xc8] = 3;
    base[0xbe] = 3;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 3;
    *(volatile u32 *)(base + 0x1c) = CM_GLOBAL(volatile u32, 0x009cf60c);
    chain = *(u8 *volatile *)(base + 0x04);
    chain = *(u8 *volatile *)(chain + 0x5d);
    if ((*(volatile u8 *)(chain + 0x11c) & 4) != 0) {
        *(volatile u32 *)(base + 0x20) = CM_GLOBAL(volatile u32, 0x009cf7c0);
    } else {
        *(volatile u32 *)(base + 0x20) = 0xffffffffu;
    }
    base[0x4a] = 3;
    base[0x52] = 2;
    vtable = *(u8 *volatile *)base;
    base[0x49] = 7;
    *(volatile u32 *)(base + 0xba) = (*(cm_fn_007d16a0_slot_t *)(vtable + 0x3c))(
        self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    return 1;
}
