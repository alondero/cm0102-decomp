#include "cmre.h"

u8 CM_THISCALL cm_fn_0070def0(u32 self)
{
    u8 kind = *(volatile u8 *)(self + 0x27u);
    if (kind == 0 && *(volatile s8 *)(self + 0x103u) < 3) return 1;
    if (kind == 1 && *(volatile s8 *)(self + 0x103u) > 8) return 1;
    return 0;
}
