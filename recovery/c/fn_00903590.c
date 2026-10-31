#include "cmre.h"

u32 CM_THISCALL cm_fn_00903590(u32 self, u32 destination)
{
    u8 value;
    *(volatile u32 *)destination = self;
    value = *(volatile u8 *)(self + 0x438);
    return (destination & 0xffffff00u) | value;
}
