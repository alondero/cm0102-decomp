/* Preferred-image VA 0x005cbdd0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_0054a130(u32 self, u32 first, u32 second, u32 third, u32 fourth);
unsigned long long CM_STDCALL __attribute__((regparm(3))) cm_fn_005cbdd0(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 argument)
{
    u32 eax = (incoming_eax & 0xffff0000u) | (u16)(s16)*(volatile s8 *)self;
    u32 fourth = eax;
    u32 third_byte = *(volatile u8 *)(self + 2u);
    u32 second = (eax & 0xffffff00u) | third_byte;
    u32 first = (argument & 0xffff0000u) | (u16)(s16)*(volatile s8 *)(self + 1u);
    (void)incoming_edx;
    return cm_fn_0054a130(self + 3u, first, second, argument, fourth);
}
