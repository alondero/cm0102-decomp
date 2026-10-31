/* Preferred-image VA 0x0093a7a0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (CM_CDECL *callback_fn)(u32 self, u32 value, u32 zero);
u32 CM_CDECL cm_fn_0093a7a0(u32 self)
{
    u32 cursor = CM_GLOBAL(volatile u32, self + 0x20u);
    u32 table = CM_GLOBAL(volatile u32, self + 0x28u);
    u32 value, callback, result;
    cursor -= 4u;
    CM_GLOBAL(volatile u32, self + 0x20u) = cursor;
    value = CM_GLOBAL(volatile u32, cursor);
    callback = CM_GLOBAL(volatile u32, table + 0x70u);
    result = ((callback_fn)callback)(self, value, 0);
    cursor = CM_GLOBAL(volatile u32, self + 0x20u);
    CM_GLOBAL(volatile u32, cursor) = result;
    cursor = CM_GLOBAL(volatile u32, self + 0x20u) + 4u;
    CM_GLOBAL(volatile u32, self + 0x20u) = cursor;
    return cursor;
}
