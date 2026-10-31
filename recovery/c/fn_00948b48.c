/* Preferred-image VA 0x00948b48, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

typedef u32 (CM_STDCALL *heap_alloc_fn)(u32, u32, u32);

u32 CM_CDECL cm_fn_00948b48(u32 arg)
{
    u32 block = ((heap_alloc_fn)CM_GLOBAL(u32, 0x00967154))(
        CM_GLOBAL(u32, 0x00de4400), 0, 0x140);
    CM_GLOBAL(volatile u32, 0x00de43f8) = block;
    if (!block)
        return 0;
    CM_GLOBAL(volatile u32, 0x00de43f0) &= 0;
    CM_GLOBAL(volatile u32, 0x00de43f4) &= 0;
    CM_GLOBAL(volatile u32, 0x00de43ec) = block;
    CM_GLOBAL(volatile u32, 0x00de43fc) = arg;
    CM_GLOBAL(volatile u32, 0x00de43e4) = 0x10;
    return 1;
}
