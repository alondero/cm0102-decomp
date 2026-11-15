/* Preferred-image VA0094FA5D, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL2: push fixed configuration00AD9688, second home, first home;
   call original0094F8DB, caller cleanup, full EAX forwarded to0094FAC8.
   The unaccepted callee source is not used as an implementation. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094f8db(u32,u32,u32);
u32 CM_CDECL cm_fn_0094fa5d(volatile u32 first_home,volatile u32 second_home)
{
    u32 second=second_home;
    u32 first=first_home;
    volatile u32 result=cm_fn_0094f8db(first,second,0x00ad9688u);
    return result;
}
