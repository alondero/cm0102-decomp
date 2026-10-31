#include "cmre.h"

void CM_THISCALL cm_fn_007160d0(u32 self, u8 value)
{
    volatile u8 *field = (volatile u8 *)(self + 0x2bu);
    *field = (u8)(*field + value);
}
