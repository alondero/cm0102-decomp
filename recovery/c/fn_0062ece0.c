/* Preferred-image VA 0x0062ece0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0062ece0(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 object)
{
    u32 index, address;
    s8 value;
    (void)incoming_edx;
    if (!object) return incoming_eax & 0xffffff00u;
    index = *(volatile u32 *)object;
    if ((s32)index >= CM_GLOBAL(volatile s32, 0x00ae2364u)) return index & 0xffffff00u;
    if (!*(volatile u32 *)(object + 0x61u)) return index & 0xffffff00u;
    address = *(volatile u32 *)self + index * 31u;
    if (*(volatile s8 *)(address + 0x12u) <= -1) return address & 0xffffff00u;
    value = *(volatile s8 *)(address + 0x13u);
    return value > 1 ? (u32)(s32)value : 1u;
}
