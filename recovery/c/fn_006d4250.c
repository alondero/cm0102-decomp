#include "cmre.h"

u32 CM_THISCALL cm_fn_006d4250(u32 self)
{
    u16 value = *(volatile u16 *)(self + 0x8ed4u);
    u32 pointer;
    if (value == 0x1efu || value == 0x3deu)
        return 0;
    pointer = *(volatile u32 *)(self + 0x4792u);
    return (*(volatile u8 *)(pointer + 0x38u) & 0x80u) != 0;
}
