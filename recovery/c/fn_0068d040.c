#include "cmre.h"

extern void CM_THISCALL cm_fn_0068d010(void *self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);

void CM_THISCALL cm_fn_0068d040(u32 self)
{
    u32 node = *(volatile u32 *)(self + 2u);
    if (node != 0) {
        u32 next;
        do {
            node = *(volatile u32 *)(self + 2u);
            next = *(volatile u32 *)(node + 6u);
            if (node != 0) {
                cm_fn_0068d010((void *)node);
                cm_fn_00944c94((void *)node);
            }
            *(volatile u32 *)(self + 2u) = next;
        } while (next != 0);
    }
}
