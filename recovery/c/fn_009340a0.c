#include "cmre.h"

u32 CM_CDECL cm_fn_009340a0(u32 object)
{
    *(volatile u32 *)(object + 0x426) = 0x1000;
    *(volatile u32 *)(object + 0x422) = 0x800;
    *(volatile u32 *)(object + 0x41e) = 0x2000;
    *(volatile u32 *)(object + 0x42a) = 0;
    return object;
}
