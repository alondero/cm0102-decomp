#include "cmre.h"

/* Preferred-image VA 005e8170, base 00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep the first indirect target cached across both calls and read each
   cursor and second target after the initial call, as in the original. */
u32 CM_CDECL cm_fn_005e8170(u32 mode)
{
    typedef u32 (CM_STDCALL *boundary)(u32);
    u32 previous = CM_GLOBAL(volatile u32, 0x009cd804u);
    u32 cached_target, cursor;
    if (previous == mode)
        return previous;
    cached_target = CM_GLOBAL(volatile u32, 0x009671c0u);
    CM_GLOBAL(volatile u32, 0x009cd804u) = mode;
    ((boundary)cached_target)(0);
    switch (mode) {
    case 0:
        cursor = CM_GLOBAL(volatile u32, 0x00aebe24u);
        ((boundary)CM_GLOBAL(volatile u32, 0x009671bcu))(cursor);
        break;
    case 1:
        cursor = CM_GLOBAL(volatile u32, 0x00aebe1cu);
        ((boundary)CM_GLOBAL(volatile u32, 0x009671bcu))(cursor);
        break;
    case 2:
        cursor = CM_GLOBAL(volatile u32, 0x00aebe20u);
        ((boundary)CM_GLOBAL(volatile u32, 0x009671bcu))(cursor);
        break;
    }
    return ((boundary)cached_target)(1);
}
