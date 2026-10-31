#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_00948b90(u32 arg);
extern unsigned long long CM_CDECL cm_fn_009498eb(u32 arg4, u32 arg8, u32 argC);
typedef u32 (CM_STDCALL *size_fn)(u32, u32, u32);

u32 CM_CDECL cm_fn_0094bef1(u32 p)
{
    u32 mode = CM_GLOBAL(volatile u32, 0x00de4404);
    u32 result, scratch[2];
    if (mode == 3) {
        result = (u32)cm_fn_00948b90(p);
        if (result)
            return *(volatile u32 *)(p - 4) - 9;
    } else if (mode == 2) {
        result = (u32)cm_fn_009498eb(p, (u32)&scratch[0], (u32)&scratch[1]);
        if (result)
            return (u32)*(volatile u8 *)result << 4;
    }
    return ((size_fn)CM_GLOBAL(volatile u32, 0x009670dc))(
        CM_GLOBAL(volatile u32, 0x00de4400), 0, p);
}
