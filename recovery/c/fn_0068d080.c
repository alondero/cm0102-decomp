#include "cmre.h"
extern void CM_THISCALL cm_fn_0068d010(void *self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_0068d080(u32 self)
{
    u32 eax = *(volatile u32 *)(self + 2);
    if (eax) {
        u32 next;
        do {
            u32 item = *(volatile u32 *)(self + 2);
            next = *(volatile u32 *)(item + 6);
            eax = item;
            if (item) {
                cm_fn_0068d010((void *)item);
                eax = (u32)cm_fn_00944c94((void *)item);
            }
            *(volatile u32 *)(self + 2) = next;
        } while (next);
    }
    *(volatile u16 *)self = 0;
    *(volatile u32 *)(self + 2) = 0;
    *(volatile u32 *)(self + 6) = 0;
    *(volatile u32 *)(self + 10) = 0;
    return eax;
}
