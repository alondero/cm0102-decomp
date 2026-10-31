#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_008ae2a0(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 address);

unsigned long long CM_THISCALL cm_fn_0077a870(u32 self)
{
    unsigned long long result = cm_fn_008ae2a0(self);
    u32 address = *(volatile u32 *)self;
    if (address != 0u)
        result = cm_fn_00944c94(address);
    return result;
}
