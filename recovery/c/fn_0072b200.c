#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
u32 CM_THISCALL cm_fn_0072b200(u32 self)
{
    u32 pointer = D(self + 0xc);
    u32 result;
    if (pointer) {
        cm_fn_009452ca(pointer);
        D(self + 0xc) = 0;
    }
    pointer = D(self);
    if (!pointer) return 0;
    result = (u32)cm_fn_009452ca(pointer);
    D(self) = 0;
    return result;
}
