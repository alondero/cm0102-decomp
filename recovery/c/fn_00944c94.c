/* Argument bridge at preferred-image VA 00944c94, image base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   One caller-cleaned stack input. Original009452ca remains a declared boundary.
   EDX:EAX carries actual incidental registers; semantic return is unknown. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

/* Read the forwarded word before PUSH writes its slot. After the call, POP
   ECX from actual returned ESP without changing flags; then perform plain RET.
   No extra frame, initialization, assumed cleanup or callee-state restoration.
   The original numeric CALL return requires the owned VM binding. */
unsigned long long CM_CDECL __attribute__((naked, noinline, used))
cm_fn_00944c94(void *arg4 __attribute__((unused)))
{
    __asm__ volatile(
        "v_00944c94:\n\tpushl 4(%esp)\n"
        CM_ORIGINAL_CALL(00944c98, 00944c9d, 009452ca)
        "popl %ecx\n v_00944c9e:\n\tret\n");
    __builtin_unreachable();
}
