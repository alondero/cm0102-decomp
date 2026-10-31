#include "cmre.h"

/* Preferred-image VA 0x00929d40, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Entry ECX is the receiver. The final vtable call supplies the EAX result.
   Reload the inclusive element count after each element destructor. */
extern u32 CM_THISCALL cm_fn_004a1c50(u32 self, u32 flag);
extern void CM_FASTCALL cm_fn_005fab70(u32 *self);
extern void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);
typedef u32 (CM_THISCALL *destroy_fn)(u32 self, u32 flag);
typedef u32 (CM_THISCALL *refresh_fn)(u32 self);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))
#define U16_AT(base, offset) (*(volatile u16 *)((base) + (offset)))

u32 CM_THISCALL cm_fn_00929d40(u32 self)
{
    u32 allocation, other, count, table, object, vtable, target;
    u32 index = 0;

    U32_AT(self, 0x4c) = 0;
    cm_fn_00687970((u8 *)self, (u8 *)0);
    allocation = U32_AT(self, 0xba);
    if (allocation != 0) {
        cm_fn_009452ca(allocation);
        U32_AT(self, 0xba) = 0;
    }
    other = U32_AT(self, 8);
    if (other != 0)
        cm_fn_004a1c50(other, 1);

    count = U32_AT(self, 0x30);
    if ((s32)count >= 0) {
        do {
            table = U32_AT(self, 0x0c);
            object = U32_AT(table + index * 4u, 0);
            if (object != 0) {
                vtable = U32_AT(object, 0);
                target = U32_AT(vtable, 0);
                ((destroy_fn)target)(object, 1);
            }
            count = U32_AT(self, 0x30);
            ++index;
        } while ((s32)index <= (s32)count);
    }
    U16_AT(self, 0x40) = (u16)(U16_AT(self, 0x40) + 2u);
    U32_AT(self, 0x30) = 0xffffffffu;
    other = CM_GLOBAL(volatile u32, 0x00b5e134);
    cm_fn_005fab70((u32 *)other);
    vtable = U32_AT(self, 0);
    U32_AT(self, 0x45) = 0;
    target = U32_AT(vtable, 0x8c);
    return ((refresh_fn)target)(self);
}
