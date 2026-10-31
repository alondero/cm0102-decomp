/* Preferred-image VA 0x004196a0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_004196a0(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 argument)
{
    u32 eax = incoming_eax, base, address, container, count = 0, found, wanted;
    u8 value;
    (void)incoming_edx;
    if (!argument) return eax;
    base = *(volatile u32 *)(self + 0xcu);
    eax = base;
    if (!base) return eax;
    address = *(volatile u32 *)argument + base;
    value = *(volatile u8 *)address;
    eax = (eax & 0xffffff00u) | value;
    if (!(value & 1u)) return eax;
    *(volatile u8 *)address = value ^ 1u;
    container = *(volatile u32 *)self;
    eax = container;
    if (!container) return eax;
    *(volatile u32 *)(container + 0xau) = *(volatile u32 *)(container + 2u);
    container = *(volatile u32 *)self;
    if ((s16)*(volatile u16 *)container <= 0) return eax;
    for (;;) {
        eax = cm_fn_0068d1b0(container);
        if (!eax) return eax;
        found = *(volatile u32 *)eax;
        wanted = *(volatile u32 *)argument;
        if (found == wanted) {
            container = *(volatile u32 *)self;
            return cm_fn_0068d1f0((u8 *)container, (const u8 *)eax);
        }
        container = *(volatile u32 *)self;
        ++count;
        eax = (u32)(s32)(s16)*(volatile u16 *)container;
        if ((s32)count >= (s32)eax) return eax;
    }
}
