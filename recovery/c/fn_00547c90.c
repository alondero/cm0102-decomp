#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u8 CM_CDECL cm_fn_00547c90(u32 object, u32 target)
{
    u32 table, entry;
    u8 count = 0;
    if (!object) return 0;
    table = D(object + 0x65);
    if (!table || !target) return 0;
    entry = D(table + 0x28);
    if (entry && D(entry + 0x39) == target) count = 1;
    entry = D(table + 0x2c);
    if (entry && D(entry + 0x39) == target) ++count;
    entry = D(table + 0x30);
    if (entry && D(entry + 0x39) == target) ++count;
    return count;
}
