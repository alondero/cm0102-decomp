#include "cmre.h"

typedef u32 (CM_STDCALL *check_fn)(u32, u32);

u32 CM_CDECL cm_fn_0094f4a5(u32 address, u32 size)
{
    check_fn check = (check_fn)CM_GLOBAL(volatile u32, 0x00967104);
    return check(address, size) == 0;
}
