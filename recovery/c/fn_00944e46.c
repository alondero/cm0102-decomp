/* Preferred-image VA00944e46, base00400000.
   Target SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The original helper remains an explicit boundary; global DE3008 and the
   argument have no inferred heap semantics. Physical EDX:EAX transport only.
   A single physical adapter preserves ordered reads, original return data,
   actual callee ESP/registers/flags, and the two real POP ECX instructions. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_00944e58(u32,u32);

unsigned long long CM_CDECL __attribute__((naked,noinline,used))
cm_fn_00944e46(u32 size __attribute__((unused)))
{
    __asm__ volatile(
        "v_00944e46:\n pushl 0x00de3008\n"
        "v_00944e4c:\n pushl 8(%esp)\n"
        CM_ORIGINAL_CALL(00944e50,00944e55,00944e58)
        "popl %ecx\n"
        "v_00944e56:\n popl %ecx\n"
        "v_00944e57:\n ret\n");
    __builtin_unreachable();
}
