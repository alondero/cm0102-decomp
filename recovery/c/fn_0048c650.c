/* Base cleanup at preferred-image VA 0048c650, image base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Fields and ownership meanings remain unknown. Original 009452ca remains an
   explicit boundary. EDX:EAX records incidental registers, not a semantic
   64-bit return. Self, members and the saved ESI stack slot may alias. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(void *);

typedef u32 base_cleanup_0048c650_word __attribute__((aligned(1), may_alias));
#define BASE_CLEANUP_0048C650_STATE \
    "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(ebx), "+S"(working_self), "+D"(edi), "+r"(ebp)
#define BASE_CLEANUP_0048C650_STEP(text) \
    __asm__ volatile(text : BASE_CLEANUP_0048C650_STATE : : "memory", "cc")

/* The original physical adapter saves only ESI. Ordered C field accesses use
   fixed registers; inspect both emitted builds for spills or helper calls. */
unsigned long long CM_THISCALL
__attribute__((naked, noinline, used, optimize("O2")))
cm_fn_0048c650(void *self_arg __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    register u32 ebx __asm__("ebx"), edi __asm__("edi"), ebp __asm__("ebp");
    register u8 *working_self __asm__("esi");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx), "=b"(ebx),
                         "=S"(working_self), "=D"(edi), "=r"(ebp));
    BASE_CLEANUP_0048C650_STEP("v_0048c650:\n\tpushl %%esi\n"
                              "v_0048c651:\n\tmovl %%ecx, %%esi\n"
                              "v_0048c653:\n");

    /* Read +14 before installing the original vtable, including fault order. */
    eax = *(volatile base_cleanup_0048c650_word *)(working_self + 0x14);
    /* Pin the store: generic C stores spill live registers into saved slots. */
    BASE_CLEANUP_0048C650_STEP("v_0048c656:\n\tmovl $0x009689e8, (%%esi)");

    u8 first_nonnull;
    __asm__ volatile("v_0048c65c:\n\ttestl %%eax, %%eax\n v_0048c65e:\n"
                     : BASE_CLEANUP_0048C650_STATE, "=@ccnz"(first_nonnull)
                     : : "memory", "cc");
    if (first_nonnull) {
        BASE_CLEANUP_0048C650_STEP("v_0048c660:\n\tpushl %%eax\n"
                                  CM_ORIGINAL_CALL(0048c661, 0048c666, 009452ca)
                                  "addl $4, %%esp");
    }

    /* +18 is loaded after the first outside call and its observable effects.
       The working ESI value is retained; no cached C self pointer is restored. */
    BASE_CLEANUP_0048C650_STEP("v_0048c669:\n");
    eax = *(volatile base_cleanup_0048c650_word *)(working_self + 0x18);
    u8 second_nonnull;
    __asm__ volatile("v_0048c66c:\n\ttestl %%eax, %%eax\n v_0048c66e:\n"
                     : BASE_CLEANUP_0048C650_STATE, "=@ccnz"(second_nonnull)
                     : : "memory", "cc");
    if (second_nonnull) {
        BASE_CLEANUP_0048C650_STEP("v_0048c670:\n\tpushl %%eax\n"
                                  CM_ORIGINAL_CALL(0048c671, 0048c676, 009452ca)
                                  "addl $4, %%esp\n v_0048c679:\n"
                                  "movl $0, 0x18(%%esi)");
    }

    /* +14 is never cleared. +18 is cleared only following its nonnull call.
       POP may read a modified saved slot; RET keeps EAX/EDX and final flags. */
    BASE_CLEANUP_0048C650_STEP("v_0048c680:\n\tpopl %%esi\n v_0048c681:\n\tret");
    __builtin_unreachable();
}
