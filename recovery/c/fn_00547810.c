#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_CDECL cm_fn_00547810(u32 target, u32 object)
{
    u32 table;
    if (!target || !object) return 0;
    table = D(object + 0x65);
    if (!table) return 0;
    if (D(table + 0x1c) == target || D(table + 0x20) == target ||
        D(table + 0x24) == target) return 1;
    return 0;
}
