/* Instruction-backed C candidate; no publication/acceptance claim. */
/* Base cleanup at preferred-image VA 0048c600, image base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Fields and ownership meanings remain unknown. Original 009452ca remains an
   explicit boundary. EDX:EAX records incidental registers, not a semantic
   64-bit return. Self, members and the saved ESI stack slot may alias. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(void *);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *);

typedef u32 base_cleanup_0048c600_word __attribute__((aligned(1), may_alias));
#define BASE_DELETING_0048C600_STATE \
    "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(ebx), "+S"(working_self), "+D"(edi), "+r"(ebp)
#define BASE_DELETING_0048C600_STEP(text) \
    __asm__ volatile(text : BASE_DELETING_0048C600_STATE : : "memory", "cc")

/* The original physical adapter saves only ESI. Ordered C field accesses use
   fixed registers; inspect both emitted builds for spills or helper calls. */
unsigned long long CM_THISCALL
__attribute__((naked, noinline, used, optimize("O2")))
cm_fn_0048c600(void *self_arg __attribute__((unused)), unsigned flags_arg __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    register u32 ebx __asm__("ebx"), edi __asm__("edi"), ebp __asm__("ebp");
    register u8 *working_self __asm__("esi");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx), "=b"(ebx),
                         "=S"(working_self), "=D"(edi), "=r"(ebp));
    BASE_DELETING_0048C600_STEP("v_0048c600:\n\tpushl %%esi\n"
                              "v_0048c601:\n\tmovl %%ecx, %%esi\n"
                              "v_0048c603:\n");

    /* Read +14 before installing the original vtable, including fault order. */
    eax = *(volatile base_cleanup_0048c600_word *)(working_self + 0x14);
    /* Pin the store: generic C stores spill live registers into saved slots. */
    BASE_DELETING_0048C600_STEP("v_0048c606:\n\tmovl $0x009689e8, (%%esi)");

    u8 first_nonnull;
    __asm__ volatile("v_0048c60c:\n\ttestl %%eax, %%eax\n v_0048c60e:\n"
                     : BASE_DELETING_0048C600_STATE, "=@ccnz"(first_nonnull)
                     : : "memory", "cc");
    if (first_nonnull) {
        BASE_DELETING_0048C600_STEP("v_0048c610:\n\tpushl %%eax\n"
                                  CM_ORIGINAL_CALL(0048c611, 0048c616, 009452ca)
                                  "addl $4, %%esp");
    }

    /* +18 is loaded after the first outside call and its observable effects.
       The working ESI value is retained; no cached C self pointer is restored. */
    BASE_DELETING_0048C600_STEP("v_0048c619:\n");
    eax = *(volatile base_cleanup_0048c600_word *)(working_self + 0x18);
    u8 second_nonnull;
    __asm__ volatile("v_0048c61c:\n\ttestl %%eax, %%eax\n v_0048c61e:\n"
                     : BASE_DELETING_0048C600_STATE, "=@ccnz"(second_nonnull)
                     : : "memory", "cc");
    if (second_nonnull) {
        BASE_DELETING_0048C600_STEP("v_0048c620:\n\tpushl %%eax\n"
                                  CM_ORIGINAL_CALL(0048c621, 0048c626, 009452ca)
                                  "addl $4, %%esp\n v_0048c629:\n"
                                  "movl $0, 0x18(%%esi)");
    }

    BASE_DELETING_0048C600_STEP("v_0048c630:\n");
    /* Read only the low flag bit at actual returned ESP+8. The flag output
       drives C routing without changing original TEST flags or scratch GPRs. */
    u8 low_bit_set;
    __asm__ volatile("testb $1, 8(%%esp)\n v_0048c635:\n"
                     : BASE_DELETING_0048C600_STATE, "=@ccnz"(low_bit_set)
                     : : "memory", "cc");
    if (low_bit_set) {
        BASE_DELETING_0048C600_STEP("v_0048c637:\n\tpushl %%esi\n"
                                     CM_ORIGINAL_CALL(0048c638, 0048c63d, 00944c94)
                                     "addl $4, %%esp");
    }

    BASE_DELETING_0048C600_STEP("v_0048c640:\n");
    eax = (u32)working_self;
    BASE_DELETING_0048C600_STEP("v_0048c642:\n\tpopl %%esi\n"
                                 "v_0048c643:\n\tret $4");
    __builtin_unreachable();
}
