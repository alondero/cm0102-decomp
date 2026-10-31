/* Preferred-image VA 0x0046ad90, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_00403320(u32 self, u32 argument);
unsigned long long CM_STDCALL __attribute__((regparm(3))) cm_fn_0046ad90(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 index_arg, u32 argument)
{
    u32 index;
    if ((u16)index_arg == 0xffffu)
        return ((unsigned long long)incoming_edx << 32) | (incoming_eax & 0xffff0000u) | 0xffffu;
    index = (u32)(s32)(s16)index_arg;
    return cm_fn_00403320(self + index * 3057u + 4u, argument);
}
