/* Preferred-image VA 0x005ae6a0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_005abf10(u32 self, u32 first, u32 second, u32 third, u32 fourth);
u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_005ae6a0(
    u32 incoming_eax, u32 incoming_edx, u32 incoming_ecx, u32 source, u32 second, u32 third)
{
    u32 first_word = *(volatile u16 *)source;
    u32 second_word = *(volatile u16 *)(source + 2u);
    u32 first = (source & 0xffff0000u) | second_word;
    u32 next = (incoming_ecx & 0xffff0000u) | first_word;
    u32 self = CM_GLOBAL(volatile u32, 0x00ae2a58u);
    (void)incoming_eax; (void)incoming_edx;
    return cm_fn_005abf10(self, first, next, second, third);
}
