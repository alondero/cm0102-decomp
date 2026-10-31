#include "cmre.h"

u32 CM_THISCALL cm_fn_008bba10(u32 self)
{
    *(volatile u32 *)self = 0x1011;
    *(volatile u8 *)(self + 4) = 0xff;
    *(volatile u32 *)(self + 5) = 0;
    *(volatile u32 *)(self + 9) = 0;
    *(volatile u32 *)(self + 0xd) = 0x21;
    return self;
}
