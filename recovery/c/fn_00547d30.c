#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
u32 CM_CDECL cm_fn_00547d30(u32 object, u32 target)
{
    u32 table;
    if (!object) return 0;
    table = D(object + 0x65);
    if (!table || !target) return 0;
    if (D(table + 0x10) == target || D(table + 0x14) == target ||
        D(table + 0x18) == target) return 1;
    return 0;
}
