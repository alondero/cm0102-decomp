/* Preferred-image VA 0x0062ed30, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0062ed30(
    u32 incoming_eax, u32 incoming_edx, u32 self, u32 object)
{
    u32 index, base;
    (void)incoming_edx;
    if (!object) return incoming_eax | 0xffu;
    index = *(volatile u32 *)object;
    if ((s32)index >= CM_GLOBAL(volatile s32, 0x00ae2364u)) return index | 0xffu;
    if (!*(volatile u32 *)(object + 0x61u)) return index | 0xffu;
    base = *(volatile u32 *)self;
    return (base & 0xffffff00u) | *(volatile u8 *)(base + index * 31u + 0x12u);
}
