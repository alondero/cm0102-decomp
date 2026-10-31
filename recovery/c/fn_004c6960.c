#include "cmre.h"

u32 CM_THISCALL cm_fn_004c6960(u32 self, u32 source)
{
    u32 value = *(volatile u32 *)source;
    *(volatile u32 *)(self + 0x71u) = value;
    return source;
}
