#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 item, u32 a, u32 b);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);

u32 CM_THISCALL cm_fn_004ae5c0(u32 self, u32 input)
{
    u32 object, node;
    *(volatile u32 *)self = 0;
    *(volatile u32 *)(self + 4) = 0;
    if (input == 0)
        return self;
    object = *(volatile u32 *)input;
    if (object != 0)
        cm_fn_004ae660((void *)self, object);
    object = *(volatile u32 *)(input + 4);
    if (object == 0)
        return self;
    node = *(volatile u32 *)(object + 2);
    *(volatile u32 *)(object + 0xa) = node;
    object = *(volatile u32 *)(input + 4);
    node = cm_fn_0068d1b0(object);
    while (node != 0) {
        u32 b = *(volatile u32 *)(node + 0xcc);
        u32 a = *(volatile u32 *)(node + 0xc8);
        cm_fn_004ae8a0(self, node, a, b);
        object = *(volatile u32 *)(input + 4);
        node = cm_fn_0068d1b0(object);
    }
    return self;
}
