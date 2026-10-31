#include "cmre.h"

u32 CM_THISCALL cm_fn_007fee80(u32 self)
{
    u32 index = (u32)(s32)*(volatile s16 *)(self + 0x382au);
    u32 pointer = *(volatile u32 *)(self + index * 0x300u + 0x7e2u);
    if (pointer != 0)
        return pointer + 0x1fcu;
    return 0x00ad9c64u;
}
