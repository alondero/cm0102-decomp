/* Preferred-image VA 0094AAB3, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3. Third home precedes the flag store; second and first homes follow
   it. Preserve the final byte read/clear and the original callee's full EAX.
   Original 0094A913 remains an explicit execution boundary. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094a913(u32,u32,u32);
u32 CM_CDECL cm_fn_0094aab3(volatile u32 first_home,
                           volatile u32 second_home,
                           volatile u32 third_home)
{
    u32 third=third_home;
    u32 second,first,result;
    CM_GLOBAL(volatile u8,0x00de3028u)=1u;
    second=second_home;
    first=first_home;
    result=cm_fn_0094a913(first,second,third);
    CM_GLOBAL(volatile u8,0x00de3028u)&=0u;
    return result;
}
