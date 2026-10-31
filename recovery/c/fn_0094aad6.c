/* Preferred-image VA 0x0094aad6, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094a80f(u32 value, u32 output, u32 width, u32 uppercase);
extern u32 CM_CDECL cm_fn_0094a913(u32 value, u32 output, u32 width);
extern u32 CM_CDECL cm_fn_0094a9f1(u32 value, u32 output, u32 width, u32 uppercase);

u32 CM_CDECL cm_fn_0094aad6(u32 value, u32 output, u32 format, u32 width, u32 uppercase)
{
    if (format == 0x65u || format == 0x45u)
        return cm_fn_0094a80f(value, output, width, uppercase);
    if (format == 0x66u)
        return cm_fn_0094a913(value, output, width);
    return cm_fn_0094a9f1(value, output, width, uppercase);
}
