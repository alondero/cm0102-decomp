/* Preferred-image VA 0x005d7b70, image base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

/* The object in ECX gates all cleanup.  After the first cleanup, reload the
   global: the callee may have changed it.  The inner cleanup and free use that
   reloaded pointer; its saved ESI slot, call-return words, and the ordering of
   the final global clear are observable through aliases and faults.  A tail
   transfer forwards the actual register and flag state to 0066e9c0.  This
   compiler-specific adapter uses the VM's owned near-call continuations. */
#define CLEAN_STATE "+a"(eax), "+b"(ebx), "+c"(ecx), "+d"(edx), "+S"(esi), "+D"(edi)
#define CLEAN_STEP(text) __asm__ volatile(text : CLEAN_STATE : : "memory", "cc")
#define CLEAN_ZERO(text) __asm__ volatile(text : CLEAN_STATE, "=@ccz"(zero) : : "memory", "cc")

void CM_THISCALL __attribute__((naked, noinline, used,
    optimize("O2", "no-tree-ch", "align-loops=1", "align-jumps=1", "align-labels=1")))
cm_fn_005d7b70(void *self __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ebx __asm__("ebx"), ecx __asm__("ecx");
    register u32 edx __asm__("edx"), esi __asm__("esi"), edi __asm__("edi");
    u8 zero;
    __asm__ volatile("" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx), "=S"(esi), "=D"(edi));

    CLEAN_ZERO("v_005d7b70: cmpl $0, (%%ecx)\n");
    if (zero) {
        CLEAN_STEP(CM_ORIGINAL_CALL(005d7b75, 005d7b7a, 0041bad0));
        CLEAN_ZERO("movl 0xdd7eec, %%ecx\n testl %%ecx, %%ecx\n");
        if (!zero) {
            CLEAN_STEP("pushl %%esi\n" CM_ORIGINAL_CALL(005d7b85, 005d7b8a, 0084cb10));
            CLEAN_ZERO("movl 0xdd7eec, %%esi\n testl %%esi, %%esi\n");
            if (!zero) {
                CLEAN_STEP("movl %%esi, %%ecx\n" CM_ORIGINAL_CALL(005d7b96, 005d7b9b, 0084caf0)
                    "pushl %%esi\n" CM_ORIGINAL_CALL(005d7b9c, 005d7ba1, 00944c94)
                    "addl $4, %%esp\n");
            }
            CLEAN_STEP("movl $0, 0xdd7eec\n popl %%esi\n");
        }
        CLEAN_STEP(CM_ORIGINAL_CALL(005d7baf, 005d7bb4, 005e2cc0)
            "jmp _cm_fn_0066e9c0\n");
    }
    CLEAN_STEP("ret\n");
    __builtin_unreachable();
}
