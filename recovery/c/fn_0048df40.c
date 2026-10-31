/* Preferred-image VA 0x0048df40, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u8 CM_CDECL cm_fn_0048df40(u32 object)
{
    u32 nested, target, entry;
    u32 index;
    if (!object)
        return 3;
    nested = CM_GLOBAL(volatile u32, object + 0x38u);
    if (!nested)
        return 3;
    target = CM_GLOBAL(volatile u32, nested + 0x1fu);
    if (!target)
        return 3;
    for (index = 0; index < 0x23; ++index) {
        entry = CM_GLOBAL(volatile u32, 0x00b63d60u + index * 72u);
        if (entry && CM_GLOBAL(volatile u8, entry + 0x11cu) && entry == target)
            return CM_GLOBAL(volatile u8, 0x00b63d74u + index * 72u);
    }
    return 3;
}
