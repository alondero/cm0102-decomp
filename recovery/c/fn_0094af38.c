/* Preferred-image VA 0x0094af38, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);
u32 CM_CDECL cm_fn_0094af38(u32 stream)
{
    u32 flags = CM_GLOBAL(volatile u32, stream + 0xcu);
    if ((flags & 0x83u) && (flags & 8u)) {
        u32 buffer = CM_GLOBAL(volatile u32, stream + 8u);
        cm_fn_009452ca(buffer);
        CM_GLOBAL(volatile u16, stream + 0xcu) &= 0xfbf7u;
        CM_GLOBAL(volatile u32, stream) = 0;
        CM_GLOBAL(volatile u32, stream + 8u) = 0;
        CM_GLOBAL(volatile u32, stream + 4u) = 0;
        return 0;
    }
    return flags;
}
