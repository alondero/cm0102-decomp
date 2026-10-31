#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
void CM_CDECL cm_fn_00934440(u32 unused, u32 object, u32 flag, u32 index)
{
    u32 table, value;
    (void)unused;
    if ((u16)flag == 0) return;
    table = D(object + 0x18);
    value = D(table + index * 4u);
    D(table + index * 4u) = (value + 0x20u) & 0xffffffc0u;
}
