/* Instruction-backed C candidate at preferred-image VA007e6350, base00400000.
   TargetSHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Instruction-equivalent wrapper to the already accepted6130 pattern; this
   routine calls opaque child cleanup007e6370. Original allocator00944c94
   remains a boundary. Source acceptance is unproven until cmre publication. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_007e6370(void *);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *);

#define DERIVED_WRAPPER_007E6350_STATE \
    "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(ebx), "+S"(working_esi), "+D"(edi), "+r"(ebp)
#define DERIVED_WRAPPER_007E6350_STEP(text) \
    __asm__ volatile(text : DERIVED_WRAPPER_007E6350_STATE : : "memory", "cc")

/* Register-only C routing plus the original physical stack/call adapter.
   Never reload self or flags from presumed entry slots after the outside call:
   its actual returned ESP/ESI and saved-stack aliases are observable. */
unsigned long long CM_THISCALL
__attribute__((naked, noinline, used, optimize("O2")))
cm_fn_007e6350(void *self_arg __attribute__((unused)),
               unsigned flags_arg __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    register u32 ebx __asm__("ebx"), edi __asm__("edi"), ebp __asm__("ebp");
    register u32 working_esi __asm__("esi");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx), "=b"(ebx),
                         "=S"(working_esi), "=D"(edi), "=r"(ebp));
    DERIVED_WRAPPER_007E6350_STEP("v_007e6350:\n\tpushl %%esi\n"
                                 "v_007e6351:\n\tmovl %%ecx, %%esi\n"
                                 CM_ORIGINAL_CALL(007e6353, 007e6358, 007e6370));

    /* Read only the low flag bit at actual returned ESP+8. The flag output
       drives C routing without changing original TEST flags or scratch GPRs. */
    u8 low_bit_set;
    __asm__ volatile("testb $1, 8(%%esp)\n v_007e635d:\n"
                     : DERIVED_WRAPPER_007E6350_STATE, "=@ccnz"(low_bit_set)
                     : : "memory", "cc");
    if (low_bit_set) {
        DERIVED_WRAPPER_007E6350_STEP("v_007e635f:\n\tpushl %%esi\n"
                                     CM_ORIGINAL_CALL(007e6360, 007e6365, 00944c94)
                                     "addl $4, %%esp");
    }

    DERIVED_WRAPPER_007E6350_STEP("v_007e6368:\n");
    eax = working_esi;
    DERIVED_WRAPPER_007E6350_STEP("v_007e636a:\n\tpopl %%esi\n"
                                 "v_007e636b:\n\tret $4");
    __builtin_unreachable();
}
