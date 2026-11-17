#include "cmre.h"

/* Preferred-image VAs below refer to CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   007e8516..007e8519 forwards incoming ECX and pushes two DWORDs.
   007e8569..007e8571 loads ECX and pushes key and the complete EDX.
   007e857d/007e858b pop eight bytes. Preserve observable EAX, including
   the partial AX replacement at 007e8580. */
extern u32 CM_THISCALL cm_fn_007e82e0(u32 self, u32 key, u32 create);
extern u32 CM_THISCALL cm_fn_00556d00(u32 self, u32 key, u32 old);

u32 CM_THISCALL cm_fn_007e8510(u32 self, u32 key, u32 replacement)
{
    u32 result = cm_fn_007e82e0(self, key, 1);
    u32 record = result;
    s16 old;
    if (!record)
        return result;
    old = *(volatile s16 *)(record + 6u);
    if (old == -1) {
        result = (result & 0xffff0000u) | (u16)replacement;
        *(volatile u16 *)(record + 6u) = (u16)result;
        return result;
    } else {
        u32 global = CM_GLOBAL(volatile u32, 0x00ae28f0);
        u16 next = (u16)replacement;
        u32 base = *(volatile u32 *)(global + 4u);
        u32 source = base + (u32)(s32)old * 20u;
        u32 target = base + (u32)(s32)(s16)next * 20u;
        s16 amount = *(volatile s16 *)(source + 2u);
        u32 argument;
        u32 receiver;
        if (amount > 0) {
            u16 value = *(volatile u16 *)(target + 2u);
            *(volatile u16 *)(target + 2u) = (u16)(value + (u16)amount);
        } else {
            s8 amount8 = *(volatile s8 *)source;
            if (amount8 > 0) {
                u8 value = *(volatile u8 *)target;
                *(volatile u8 *)target = (u8)(value + (u8)amount8);
            }
        }
        argument = (source & 0xffff0000u) | *(volatile u16 *)(record + 6u);
        receiver = CM_GLOBAL(volatile u32, 0x00ae28f0);
        result = cm_fn_00556d00(receiver, key, argument);
        *(volatile u16 *)(record + 6u) = next;
        return result;
    }
}
