#include "cmre.h"

typedef void (CM_STDCALL *memory_status_fn)(volatile u32 *);

s32 CM_CDECL cm_fn_007a16d0(void)
{
    volatile u32 status[8];
    s32 total_physical;

    status[0] = 0x20u;
    ((memory_status_fn)CM_GLOBAL(volatile u32, 0x009670c4))(status);
    total_physical = (s32)status[2];
    return total_physical / 0x100000;
}
