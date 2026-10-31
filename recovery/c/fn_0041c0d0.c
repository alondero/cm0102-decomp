#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007feeb0(u32 self, u32 source);

u32 CM_CDECL cm_fn_0041c0d0(u32 source)
{
    if (CM_GLOBAL(volatile u32, 0x00989858u) == 0 && source != 0)
        source = 0x00989adcu;
    return cm_fn_007feeb0(0x00b74450u, source);
}
