/* Preferred-image VA 0x00939f20, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

typedef u32 (__attribute__((cdecl, regparm(3))) *callback_939f20)(
    u32, u32, u32, u32, u32);

u32 CM_CDECL __attribute__((regparm(3))) cm_fn_00939f20(
    u32 incoming_eax, u32 incoming_edx, u32 incoming_ecx, u32 self)
{
    u32 cursor = *(volatile u32 *)(self + 0x20) - 4u;
    u32 value, result;
    (void)incoming_eax;
    (void)incoming_edx;
    *(volatile u32 *)(self + 0x20) = cursor;
    value = *(volatile u32 *)cursor;
    result = ((callback_939f20)*(volatile u32 *)(self + 0x52))(
        value, cursor, incoming_ecx, self, value);
    cursor = *(volatile u32 *)(self + 0x20);
    *(volatile u32 *)cursor = result;
    cursor = *(volatile u32 *)(self + 0x20) + 4u;
    *(volatile u32 *)(self + 0x20) = cursor;
    return cursor;
}
