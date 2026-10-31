#include "cmre.h"

extern u32 CM_CDECL cm_fn_0090bde0(u32 first, u32 second);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00945b00(u32 first, u32 second);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);

u32 CM_CDECL cm_fn_005f2500(u32 argument)
{
    u32 config = CM_GLOBAL(volatile u32, 0x00ae3138);
    u32 mode = *(volatile u32 *)(config + 8);
    u32 index, path, stream;
    if (mode != 0 && mode != 1)
        return 0;
    index = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a) * 768u;
    if (*(volatile u16 *)(0x00b74c24u + index) != 0)
        return 0;
    cm_fn_00944c9f(0x00b5dedc, 0x009cec50, argument);
    path = cm_fn_0090bde0(0x009cec18, 0x00b5dedc);
    stream = cm_fn_00945b00(path, 0x009899fc);
    if (stream == 0)
        return 0;
    cm_fn_00945a09(stream);
    return 1;
}
