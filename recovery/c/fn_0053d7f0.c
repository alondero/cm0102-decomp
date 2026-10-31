#include "cmre.h"

u32 CM_CDECL cm_fn_0053d7f0(volatile u32 *object)
{
    u32 first;
    u32 value;
    volatile u32 *field;

    if (object == 0)
        return 0;
    field = (volatile u32 *)((volatile u8 *)object + 0x53);
    field = (volatile u32 *)(*field);
    if (field == 0)
        return 0;
    first = *field;
    if (first != CM_GLOBAL(volatile u32, 0x009cf488))
        return 0;

    value = *object;
    if (value == CM_GLOBAL(volatile u32, 0x009d0120)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0134)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d02e4)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d02e8)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d02ec)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d02f0)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d02f4)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d02f8)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d02fc)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0304)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0308)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d030c)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0310)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0314)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0318)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d031c)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0320)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0324)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0328)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d032c)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0330)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0334)) return 1;
    if (value == CM_GLOBAL(volatile u32, 0x009d0338)) return 1;
    return 0;
}
