#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u8 CM_CDECL cm_fn_00547ce0(u32 object, u32 target)
{
    u32 table, entry;
    u8 count = 0;
    if (!object) return 0;
    table = D(object + 0x65);
    if (!table || !target) return 0;
    entry = D(table + 0x1c);
    if (entry && D(entry + 0x39) == target) count = 1;
    entry = D(table + 0x20);
    if (entry && D(entry + 0x39) == target) ++count;
    entry = D(table + 0x24);
    if (entry && D(entry + 0x39) == target) ++count;
    return count;
}

