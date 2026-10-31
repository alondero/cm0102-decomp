/* Preferred-image VA 0x0069b780, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern void CM_CDECL cm_fn_006b1d10(u32 value);
void CM_STDCALL cm_fn_0069b780(u32 value)
{
    cm_fn_006b1d10(value);
}
