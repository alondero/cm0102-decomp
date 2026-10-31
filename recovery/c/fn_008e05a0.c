#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
/* At the indirect call EAX holds the vtable, EDX the second stack
   argument, and ECX the receiver; the target pops three stack words. */
typedef u32 (__attribute__((stdcall, regparm(3))) *dispatch_fn)(
    u32 eax_in, u32 edx_in, u32 ecx_in, u32 record, u32 second, u32 third);

u32 CM_THISCALL cm_fn_008e05a0(u32 self, u32 record, u32 second, u32 third)
{
    u32 index = (u32)cm_fn_00785830((void *)record, 0);
    u32 object = *(volatile u32 *)(self + 0x8acu + index * 4u);
    u32 vtable = *(volatile u32 *)object;
    return ((dispatch_fn)*(volatile u32 *)(vtable + 0x30u))(
        vtable, second, object, record, second, third);
}
