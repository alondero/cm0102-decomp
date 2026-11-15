/* Preferred-image VA 0094DA93, base 00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL2. Cache only the counter pointer. Each loop reloads the caller's
   second home before incrementing the DWORD counter. Both native callees
   remain explicit boundaries. Return the full last reader result. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_0094da62(u32);
extern u32 CM_CDECL cm_fn_00947090(u32);
u32 CM_CDECL cm_fn_0094da93(volatile u32 counter_home,
                           volatile u32 stream_home)
{
    u32 counter=counter_home;
    u32 value;
    do {
        u32 stream=stream_home;
        u32 count=CM_GLOBAL(volatile u32,counter);
        CM_GLOBAL(volatile u32,counter)=count+1u;
        value=cm_fn_0094da62(stream);
    } while (cm_fn_00947090(value)!=0u);
    return value;
}
