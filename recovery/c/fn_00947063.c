/* WIP: nested callee stack fault address mismatch, case 112. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094c322(u32, u32);
u32 CM_CDECL cm_fn_00947063(u32 value)
{
    u32 table;
    if ((s32)CM_GLOBAL(volatile u32, 0x00ad91dc) > 1)
        return cm_fn_0094c322(value, 0x80);
    table = CM_GLOBAL(volatile u32, 0x00ad8fd0);
    return CM_GLOBAL(volatile u8, table + value * 2u) & 0x80u;
}
