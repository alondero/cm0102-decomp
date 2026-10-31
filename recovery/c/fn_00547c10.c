#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_CDECL cm_fn_00547c10(u32 object, u32 target)
{
    u32 table;
    if (!object) return 0;
    table = D(object + 0x65);
    if (!table || !target) return table;
    if (D(table + 0x1c) == target) D(table + 0x1c) = 0;
    else if (D(table + 0x20) == target) D(table + 0x20) = 0;
    else if (D(table + 0x24) == target) D(table + 0x24) = 0;
    return table;
}
