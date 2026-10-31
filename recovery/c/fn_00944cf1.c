/* Argument bridge at preferred-image VA 00944cf1, image base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original 00944e58 and its dependencies remain explicit boundaries. The
   inventory's operator_new label does not establish allocator semantics.
   EDX:EAX carries actual incidental registers; semantic return is unknown. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_00944e58(u32, u32);

/* Exact physical bridge: after PUSH 1, read the forwarded word at actual
   ESP+8. After the original call, POP twice from actual returned ESP, changing
   ECX without changing flags. Preserve all other actual callee register values.
   No C register temporaries, extra frame, initialization or presumed cleanup. */
unsigned long long CM_CDECL __attribute__((naked, noinline, used))
cm_fn_00944cf1(u32 size __attribute__((unused)))
{
    __asm__ volatile(
        "v_00944cf1:\n\tpushl $1\n"
        "v_00944cf3:\n\tpushl 8(%esp)\n"
        CM_ORIGINAL_CALL(00944cf7, 00944cfc, 00944e58)
        "popl %ecx\n v_00944cfd:\n\tpopl %ecx\n v_00944cfe:\n\tret\n");
    __builtin_unreachable();
}
