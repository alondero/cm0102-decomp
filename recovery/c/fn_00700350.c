#include "cmre.h"

void CM_THISCALL cm_fn_00700350(u32 self, u8 value)
{
    *(volatile u8 *)(self + 0x8ea9u) = value;
}
