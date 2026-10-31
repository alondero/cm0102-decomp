#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
u32 CM_THISCALL cm_fn_009227d0(u32 self)
{
    u32 pointer = D(self);
    u32 result;
    if (pointer) {
        cm_fn_009452ca(pointer);
        D(self) = 0;
    }
    pointer = D(self + 4);
    if (!pointer) return 0;
    result = (u32)cm_fn_009452ca(pointer);
    D(self + 4) = 0;
    return result;
}
