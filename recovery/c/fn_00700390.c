#include "cmre.h"

void CM_THISCALL cm_fn_00700390(u32 self, u8 value)
{
    *(volatile u8 *)(self + 0x8eaau) = value;
}
