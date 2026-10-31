/* Preferred-image VA 0x00934460, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00934460(
    u32 incoming_eax, u32 unused, u32 object, u32 flag, u32 index)
{
    u32 table, address, value;
    (void)unused;
    if ((u16)flag == 0) return incoming_eax;
    table = *(volatile u32 *)(object + 0x1cu);
    address = table + index * 4u;
    value = *(volatile u32 *)address;
    *(volatile u32 *)address = (value + 0x20u) & 0xffffffc0u;
    return table;
}
