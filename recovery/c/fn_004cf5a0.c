#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument)
{
    u32 index, limit, table, entry, base;
    if (!argument) return 0;
    index = D(argument);
    limit = D(0x00ae2364);
    if ((s32)index >= (s32)limit) return 0;
    table = D(0x00ae2d0c);
    entry = D(table + index * 79u);
    if ((s32)entry < 0) return 0;
    base = D(self);
    return base + entry * 80u;
}
