/* Preferred-image VA 0x006c38c0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006c38c0(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 object)
{
    s8 index = *(volatile s8 *)(object + 0x19u);
    u32 row;
    s8 group;
    (void)incoming_edx;
    if (index < 0 || index > 10) return incoming_eax & 0xffff0000u;
    group = *(volatile s8 *)(object + 0x27u);
    row = *(volatile u32 *)(self + 0x8ebcu + (u32)(s32)group * 4u);
    return *(volatile u16 *)(row + (u32)(s32)index * 2u);
}
