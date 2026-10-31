#include "cmre.h"

u32 CM_THISCALL cm_fn_007feb70(u32 self)
{
    u32 index = (u32)(s32)*(volatile s16 *)(self + 0x382au);
    s32 count = *(volatile s16 *)(self + index * 2u + 0x380au);
    u32 address, result = 0;
    if (count <= 0)
        return 0;
    address = self + index * 0x300u + 0x7f2u;
    while (count-- > 0) {
        u32 first = *(volatile u32 *)(address - 4u);
        if (first == 0 && *(volatile u32 *)address == 0)
            ++result;
        address += 0x30u;
    }
    return result;
}
