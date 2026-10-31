#include "cmre.h"

void CM_THISCALL cm_fn_007003a0(u32 self, u16 value)
{
    *(volatile u16 *)(self + 0x8ed8u) = value;
}
