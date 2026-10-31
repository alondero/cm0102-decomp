/* Preferred-image VA 0x0077ac00, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_008ae340(volatile u32 *self, u32 index_arg);
u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0077ac00(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 argument)
{
    s16 index = (s16)argument;
    u32 forwarded;
    if (index <= 0 || index > 16) return (incoming_eax & 0xffff0000u) | (u16)argument;
    forwarded = (incoming_edx & 0xffff0000u) | *(volatile u16 *)(self + (u32)index * 2u + 0x4b8u);
    return cm_fn_008ae340((volatile u32 *)self, forwarded);
}
