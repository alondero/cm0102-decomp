/* Preferred-image VA 00922c40, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole indexed caller00922ADF tests AL at00922AE4. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0090cba0(u32 stream, u32 output);
extern u32 CM_CDECL cm_fn_00946fd8(u32 source, u32 format, ...);
/* Full439B original writes ECX before every call; incoming ECX is unused. */
extern u32 CM_STDCALL cm_fn_00922d20(u32 stream, u32 output);

u8 CM_THISCALL cm_fn_00922c40(u32 self, u32 stream, volatile u32 output)
{
    u8 line[2000];
    u32 target, result;
    (void)self;
    result = cm_fn_0090cba0(stream, (u32)line);
    if (result == 0) return 0;
    target = output;
    result = cm_fn_00946fd8((u32)line, 0x00ad4e24u, target, target + 4u);
    if (result != 2) return 0;
    result = cm_fn_00922d20(stream, target + 0x1eu);
    if ((u8)result == 0) return 0;
    result = cm_fn_00922d20(stream, target + 0x31u);
    if ((u8)result == 0) return 0;
    result = cm_fn_00922d20(stream, target + 0x44u);
    if ((u8)result == 0) return 0;
    result = cm_fn_00922d20(stream, target + 0x57u);
    return (u8)result != 0;
}
