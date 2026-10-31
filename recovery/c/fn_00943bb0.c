/* Preferred-image VA 00943bb0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_009443a0(u32 allocator, u32 size);
extern u32 CM_CDECL cm_fn_00941580(u32 stream);
extern u32 CM_CDECL cm_fn_009415c0(u32 stream);

u32 CM_CDECL cm_fn_00943bb0(u32 allocator, volatile u32 stream)
{
    u32 object, input, value, offset;
    object = cm_fn_009443a0(allocator, 0x28u);
    D(object) = allocator;
    input = stream;
    value = cm_fn_00941580(input);
    D(object + 4u) = value;
    for (offset = 8u; offset < 0x28u; offset += 2u) {
        value = cm_fn_009415c0(input);
        W(object + offset) = (u16)value;
    }
    return object;
}
