#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_THISCALL cm_fn_0068d1b0(u32 self)
{
    u32 node = D(self + 0xa);
    u32 value, next;
    if (!node) return 0;
    value = D(node);
    next = D(node + 6);
    D(self + 0xa) = next;
    return value;
}
