/* Preferred-image VA 0054daf0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00932e10(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00945ce6(u32 buffer, u32 size, u32 count, u32 stream);

u32 CM_CDECL cm_fn_0054daf0(u32 stream, u32 mode, u32 buffer, u32 count)
{
    u32 result;
    if (mode != 0)
        result = cm_fn_00932e10(buffer, 0x4eu, count, stream);
    else
        result = cm_fn_00945ce6(buffer, 0x4eu, count, stream);
    return result == count;
}
