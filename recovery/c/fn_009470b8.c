/* Preferred-image VA009470B8, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL variadic wrapper: original LEA ESP+12 addresses the actual first
   variadic home. Read second then first home before calling0094D006 with
   those words and that pointer. EAX also retains that LEA result at CALL;
   preserve it explicitly across the scanner's initial AL-only load.
   The original scanner remains a boundary. */
#include "cmre.h"
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_0094d006(u32,u32,u32,u32);
u32 CM_CDECL cm_fn_009470b8(volatile u32 first_home,volatile u32 second_home,...)
{
    __builtin_va_list arguments;
    __builtin_va_start(arguments,second_home);
    u32 second=second_home;
    u32 first=first_home;
    volatile u32 result=cm_fn_0094d006((u32)arguments,first,second,(u32)arguments);
    __builtin_va_end(arguments);
    return result;
}
