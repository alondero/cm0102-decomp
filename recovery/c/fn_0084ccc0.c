/* Preferred-image VA 0x0084ccc0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (__attribute__((stdcall, regparm(3))) *callback_84ccc0)(u32, u32, u32, u32, u32);
u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0084ccc0(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 index_arg, u32 volume)
{
    u32 eax = *(volatile u32 *)(self + 0x28u), index, element, vtable, scaled;
    (void)incoming_eax; (void)incoming_edx;
    if (!eax) return eax;
    index = (u8)index_arg;
    eax = (eax & 0xffffff00u) | index;
    if ((s8)index < 0 || (s8)index >= 10) return eax;
    if ((s32)volume < 0 || (s32)volume > 100) return eax;
    element = *(volatile u32 *)(self + index * 4u);
    if (!element) return element;
    scaled = (volume * 5u - 500u) * 20u;
    vtable = *(volatile u32 *)element;
    return ((callback_84ccc0)*(volatile u32 *)(vtable + 0x3cu))(
        element, volume, scaled, element, scaled);
}
