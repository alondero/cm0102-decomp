#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0094b307(u32);
u32 CM_CDECL cm_fn_0094da62(u32 stream)
{
    u32 count = D(stream + 4) - 1u;
    u32 pointer, result;
    D(stream + 4) = count;
    if ((s32)count < 0) return cm_fn_0094b307(stream);
    pointer = D(stream);
    result = CM_GLOBAL(volatile u8, pointer);
    D(stream) = pointer + 1u;
    return result;
}

