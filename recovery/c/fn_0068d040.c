/* Private proposal: preferred-image VA 0x0068d040, base 0x00400000.
   CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0 RET0. Empty head returns zero; nonempty return keeps actual low EAX
   of the final original node-free call. EDX is not part of this return. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_0068d010(void *self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);

u32 CM_THISCALL cm_fn_0068d040(u32 self)
{
    u32 node = *(volatile u32 *)(self + 2u);
    u32 result = node;
    if (node != 0) {
        u32 next;
        do {
            node = *(volatile u32 *)(self + 2u);
            next = *(volatile u32 *)(node + 6u);
            if (node != 0) {
                cm_fn_0068d010((void *)node);
                result = (u32)cm_fn_00944c94((void *)node);
            }
            *(volatile u32 *)(self + 2u) = next;
        } while (next != 0);
    }
    return result;
}
