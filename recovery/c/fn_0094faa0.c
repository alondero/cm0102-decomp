#include "cmre.h"
extern u32 CM_CDECL cm_fn_00952128(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0094fa5d(u32, u32);
u32 CM_CDECL cm_fn_0094faa0(u32 destination, u32 input)
{
    u32 scratch[7];
    scratch[6] = input;
    cm_fn_00952128((u32)scratch, (u32)&scratch[6], input, 0, 0, 0, 0);
    return cm_fn_0094fa5d((u32)scratch, destination);
}

