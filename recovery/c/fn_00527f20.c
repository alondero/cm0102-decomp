#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
u32 CM_CDECL cm_fn_00527f20(void)
{
    u32 handle = D(0x00ae23e4), mode, result;
    if (!handle) return 0;
    mode = D(0x00ae2340);
    if (mode) {
        u32 target = D(0x009670f4);
        result = ((u32 (CM_THISCALL *)(u32, u32))target)(mode, handle);
    } else {
        result = (u32)cm_fn_009452ca(handle);
    }
    D(0x00ae23e4) = 0;
    D(0x00ae238c) = 0;
    return result;
}

