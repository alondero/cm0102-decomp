#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_CDECL cm_fn_00547c50(u32 object, u32 target)
{
    u32 table;
    if (!object) return 0;
    table = D(object + 0x65);
    if (!table || !target) return table;
    if (D(table + 0x28) == target) D(table + 0x28) = 0;
    else if (D(table + 0x2c) == target) D(table + 0x2c) = 0;
    else if (D(table + 0x30) == target) D(table + 0x30) = 0;
    return table;
}

