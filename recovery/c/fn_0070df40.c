#include "cmre.h"
u8 CM_THISCALL cm_fn_0070df40(u32 self)
{
    u8 kind = CM_GLOBAL(volatile u8, self + 0x27);
    if (kind == 1 && (s8)CM_GLOBAL(volatile u8, self + 0x103) < 3) return 1;
    if (kind == 0 && (s8)CM_GLOBAL(volatile u8, self + 0x103) > 8) return 1;
    return 0;
}
