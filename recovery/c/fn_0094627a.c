/* Preferred-image VA 0x0094627a, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094620d(u32 value);
u32 CM_CDECL cm_fn_0094627a(u32 value)
{
    return cm_fn_0094620d(value) ? 0u : 0xffffffffu;
}
