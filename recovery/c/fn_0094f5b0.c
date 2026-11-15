/* Preferred-image VA0094F5B0, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL2. Native loads the second home before reading the first, masks
   bit19 from the copied second argument, then calls0094F57B. That helper
   and its control-word effects remain an explicit original boundary.
   The volatile result retains a CALL/return rather than rewriting homes
   for a sibling tail call. The wrapper caller forwards the full EAX. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094f57b(u32,u32);
u32 CM_CDECL cm_fn_0094f5b0(volatile u32 first_home,volatile u32 second_home)
{
    u32 second=second_home&0xfff7ffffu;
    u32 first=first_home;
    volatile u32 result=cm_fn_0094f57b(first,second);
    return result;
}
