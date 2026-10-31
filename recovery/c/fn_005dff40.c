/* Preferred-image VA 0x005dff40, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_STDCALL __attribute__((regparm(3))) cm_fn_00767150(
    u32 eax, u32 edx, u32 ecx, u32 object, u32 mode);
unsigned long long CM_CDECL __attribute__((regparm(3))) cm_fn_005dff40(
    u32 incoming_eax, u32 incoming_edx, u32 self)
{
    u32 container = *(volatile u32 *)(self + 4u);
    u32 object = *(volatile u32 *)(self + 0xau);
    u32 receiver = *(volatile u32 *)(container + 0x1cu);
    (void)incoming_eax;
    cm_fn_00767150(object, incoming_edx, receiver, object, 0xffffffffu);
    container = *(volatile u32 *)(self + 4u);
    object = *(volatile u32 *)(self + 0xau);
    receiver = *(volatile u32 *)(container + 0x2cu);
    return cm_fn_00767150(container, object, receiver, object, 0x14u);
}
