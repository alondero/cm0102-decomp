#include "cmre.h"

u32 CM_THISCALL cm_fn_00789280(u32 self, u32 destination)
{
    u32 first = *(volatile u32 *)(self + 0xcdu);
    u32 second;
    *(volatile u32 *)destination = first;
    second = *(volatile u32 *)(self + 0xd1u);
    *(volatile u32 *)(destination + 4u) = second;
    return destination;
}
