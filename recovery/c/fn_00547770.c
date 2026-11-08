/* CM0102 3.9.68, preferred-image VA 00547770, base 00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The null path returns EAX=0; the other path forwards full callee EAX.
   Original 00669bd0 remains an explicit boundary, not its unaccepted C draft. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_00669bd0(u32 object, u32 target_a, u32 target_b);

u32 CM_CDECL cm_fn_00547770(u32 object)
{
    if (object == 0)
        return 0;
    return cm_fn_00669bd0(object, 0, 0);
}
