/* Cleanup wrapper at preferred-image VA 007e6130, image base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Object/ownership meanings remain unknown. Original 007e6150 includes dynamic
   calls and FS-chain operations; it and 00944c94 remain explicit boundaries.
   EDX:EAX preserves incidental registers, not an inferred 64-bit return. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_007e6150(void *);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *);

#define CLEANUP_WRAPPER_007E6130_STATE \
    "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(ebx), "+S"(working_esi), "+D"(edi), "+r"(ebp)
#define CLEANUP_WRAPPER_007E6130_STEP(text) \
    __asm__ volatile(text : CLEANUP_WRAPPER_007E6130_STATE : : "memory", "cc")

/* Register-only C routing plus the original physical stack/call adapter.
   Never reload self or flags from presumed entry slots after the outside call:
   its actual returned ESP/ESI and saved-stack aliases are observable. */
unsigned long long CM_THISCALL
__attribute__((naked, noinline, used, optimize("O2")))
cm_fn_007e6130(void *self_arg __attribute__((unused)),
               unsigned flags_arg __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    register u32 ebx __asm__("ebx"), edi __asm__("edi"), ebp __asm__("ebp");
    register u32 working_esi __asm__("esi");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx), "=b"(ebx),
                         "=S"(working_esi), "=D"(edi), "=r"(ebp));
    CLEANUP_WRAPPER_007E6130_STEP("v_007e6130:\n\tpushl %%esi\n"
                                 "v_007e6131:\n\tmovl %%ecx, %%esi\n"
                                 CM_ORIGINAL_CALL(007e6133, 007e6138, 007e6150));

    /* Read only the low flag bit at actual returned ESP+8. The flag output
       drives C routing without changing original TEST flags or scratch GPRs. */
    u8 low_bit_set;
    __asm__ volatile("testb $1, 8(%%esp)\n v_007e613d:\n"
                     : CLEANUP_WRAPPER_007E6130_STATE, "=@ccnz"(low_bit_set)
                     : : "memory", "cc");
    if (low_bit_set) {
        CLEANUP_WRAPPER_007E6130_STEP("v_007e613f:\n\tpushl %%esi\n"
                                     CM_ORIGINAL_CALL(007e6140, 007e6145, 00944c94)
                                     "addl $4, %%esp");
    }

    CLEANUP_WRAPPER_007E6130_STEP("v_007e6148:\n");
    eax = working_esi;
    CLEANUP_WRAPPER_007E6130_STEP("v_007e614a:\n\tpopl %%esi\n"
                                 "v_007e614b:\n\tret $4");
    __builtin_unreachable();
}
