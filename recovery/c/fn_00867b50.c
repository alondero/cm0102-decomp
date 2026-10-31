#include "cmre.h"

u32 CM_THISCALL cm_fn_00867b50(u32 self)
{
    if (*(volatile u8 *)(self + 0x1cu) == 2)
        return 1;
    if (*(volatile u8 *)(self + 0x1du) == 1)
        return 1;
    if (*(volatile u8 *)(self + 0x1fu) == 2)
        return 1;
    return 0;
}
