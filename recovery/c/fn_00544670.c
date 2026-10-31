#include "cmre.h"

u32 CM_CDECL cm_fn_00544670(u32 record)
{
    u32 id, ptr, first;
    if (CM_GLOBAL(volatile u8, 0x00ae233c) == 1)
        return 1;
    first = CM_GLOBAL(volatile u32, 0x009cf260);
    id = *(volatile u32 *)record;
    if (id == first)
        return 1;
    ptr = *(volatile u32 *)(record + 0x71);
    if (!ptr)
        return 0;
    if (id == CM_GLOBAL(volatile u32, 0x009cf430)) return 0;
    if (id == CM_GLOBAL(volatile u32, 0x009cf4dc)) return 0;
    if (id == CM_GLOBAL(volatile u32, 0x009cf2ac)) return 0;
    if (id == CM_GLOBAL(volatile u32, 0x009cf448)) return 0;
    id = *(volatile u32 *)ptr;
    first = CM_GLOBAL(volatile u32, 0x009cfa10);
    return id == first;
}
