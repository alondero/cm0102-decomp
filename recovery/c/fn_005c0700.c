/* Preferred-image VA 0x005C0700, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), no stack arguments, ret 0; returns 1
   in EAX. Stores constants into the object and copies the global dword at
   0x009CF5D0 to object +0x1c. The word at +0x40 selects the byte pair at +0xbe
   and +0xc1 (0x7D1 gives 4 and 4, otherwise 3 and 4). Then it calls the vtable
   slot at [object] + 0x3C with ECX = object and four stack arguments:
   0xFFFFFFFF, object +0xA9, object +0x3A, 0. The slot pops its own arguments
   (no caller cleanup follows the call). Its result is stored at dword +0xba.
   Field meanings remain unknown; the vtable slot is an explicit boundary. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_slot_fn)(void *self, u32 a1, u32 a2, u32 a3, u32 a4);

u32 CM_THISCALL cm_fn_005c0700(u8 *self)
{
    cm_slot_fn slot;
    u32 result;
    u16 word40;

    word40 = *(u16 *)(self + 0x40);
    *(u16 *)(self + 0x3c) = 2;
    self[0xc2] = 3;
    self[0xc3] = 1;
    self[0xc4] = 2;
    self[0x42] = 1;
    self[0xc5] = 1;
    self[0xc6] = 2;
    self[0xc7] = 3;
    if (word40 == 0x7d1) {
        self[0xbe] = 4;
        self[0xc1] = 4;
    } else {
        self[0xbe] = 3;
        self[0xc1] = 4;
    }
    self[0xbf] = 0;
    self[0xc0] = 0;
    *(s32 *)(self + 0x1c) = CM_GLOBAL(s32, 0x009cf5d0);
    self[0x49] = 3;
    self[0x4a] = 3;
    *(u32 *)(self + 0x20) = 0xffffffffu;
    *(u16 *)(self + 0xa7) = 0xffff;
    *(u16 *)(self + 0xd9) = 0x83;
    slot = *(cm_slot_fn *)(*(u8 **)self + 0x3c);
    result = slot(self, 0xffffffffu, (u32)(self + 0xa9), (u32)(self + 0x3a), 0);
    *(u32 *)(self + 0xba) = result;
    return 1;
}
