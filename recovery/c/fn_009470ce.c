/* Preferred-image VA 0x009470ce, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_00947c67(u32 stream, u32 format, u32 arguments);
extern u32 CM_CDECL cm_fn_00947b52(u32 value, u32 stream);

u32 CM_CDECL cm_fn_009470ce(u32 output, u32 format, u32 arguments)
{
    volatile u32 stream[8];
    u32 result;
    stream[2] = output;
    stream[0] = output;
    stream[3] = 0x42u;
    stream[1] = 0x7fffffffu;
    result = cm_fn_00947c67((u32)stream, format, arguments);
    --stream[1];
    if ((s32)stream[1] >= 0)
        CM_GLOBAL(volatile u8, stream[0]) &= 0;
    else
        cm_fn_00947b52(0, (u32)stream);
    return result;
}
