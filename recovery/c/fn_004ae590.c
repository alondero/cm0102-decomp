#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg);

u32 CM_THISCALL cm_fn_004ae590(u32 self, u32 arg)
{
    *(volatile u32 *)self = 0;
    *(volatile u32 *)(self + 4u) = 0;
    if (arg != 0)
        cm_fn_004ae660((void *)self, arg);
    return self;
}
