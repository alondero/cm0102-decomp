#include "cmre.h"

extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);

void CM_THISCALL cm_fn_004ae630(u32 self)
{
    u32 pointer = *(volatile u32 *)self;
    if (pointer != 0) cm_fn_00944c94((void *)pointer);
    pointer = *(volatile u32 *)(self + 4u);
    if (pointer != 0) {
        cm_fn_0068d040(pointer);
        cm_fn_00944c94((void *)pointer);
    }
}
