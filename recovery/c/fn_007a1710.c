#include "cmre.h"

typedef void (CM_STDCALL *memory_status_fn)(volatile u32 *);

u32 CM_CDECL cm_fn_007a1710(void)
{
    volatile u32 status[8];
    u32 available_physical;
    u32 available_page_file;

    status[0] = 0x20u;
    ((memory_status_fn)CM_GLOBAL(volatile u32, 0x009670c4))(status);
    available_physical = status[3];
    available_page_file = status[5];
    return (available_page_file + available_physical) >> 20;
}
