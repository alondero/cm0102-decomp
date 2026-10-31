/* Preferred-image VA 0x006f4600, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_005b57c0(u32 self, u32 index);
u32 CM_CDECL __attribute__((regparm(3))) cm_fn_006f4600(
    u32 incoming_eax, u32 incoming_edx, u32 self)
{
    u32 argument = (incoming_eax & 0xffffff00u) | *(volatile u8 *)(self + 0x19u);
    u32 group = (u32)(s32)*(volatile s8 *)(self + 0x27u);
    u32 base = *(volatile u32 *)(self + 0x19eu);
    u32 result = cm_fn_005b57c0(base + group * 0x18e3u + 0x91e2u, argument);
    u32 offset = (result & 2u) ? 0x10au : 0x108u;
    (void)incoming_edx;
    return (result & 0xffffff00u) | *(volatile u8 *)(self + offset);
}
