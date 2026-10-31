/* Preferred-image VA 0x00945ae0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094b1e0(void);
extern volatile u32 *CM_CDECL cm_fn_0094b070(u32 path, volatile s8 *mode_text, u32 share_arg, volatile u32 *out);
volatile u32 *CM_CDECL cm_fn_00945ae0(u32 path, volatile s8 *mode_text, u32 share_arg)
{
    u32 output = cm_fn_0094b1e0();
    if (!output)
        return (volatile u32 *)0;
    return cm_fn_0094b070(path, mode_text, share_arg, (volatile u32 *)output);
}
