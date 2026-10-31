#include "cmre.h"

extern u32 CM_CDECL cm_fn_0094c034(u32 stream);
extern u32 CM_CDECL cm_fn_00947c67(u32 stream, u32 format, u32 arguments);
extern void CM_CDECL cm_fn_0094c0c1(u32 mode, u32 stream);

u32 CM_CDECL cm_fn_009464f0(volatile u32 stream, volatile u32 format, ...)
{
    __builtin_va_list arguments;
    u32 state, result, saved_format, saved_stream;
    __builtin_va_start(arguments, format);
    state = cm_fn_0094c034(stream);
    saved_format = format;
    saved_stream = stream;
    result = cm_fn_00947c67(saved_stream, saved_format, (u32)arguments);
    cm_fn_0094c0c1(state, stream);
    __builtin_va_end(arguments);
    return result;
}
