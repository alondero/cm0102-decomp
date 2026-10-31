/* Ignored C draft; preferred-image VA00944e84, image base00400000.
   Target SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Opaque allocator modes, heap ownership and API behavior remain unknown.
   Mode2 already-rounded paths skip the fallback TEST/ADD/AND sequence.
   EDX:EAX transports actual registers, not a semantic return declaration. */
#include "cmre.h"
extern unsigned long long cm_fn_00948ee4(u32);
extern unsigned long long cm_fn_00949987(u32);
typedef u32 allocator_00944e84_word __attribute__((aligned(1),may_alias));
#define ALLOCATOR_00944E84_STATE "+a"(eax),"+c"(ecx),"+d"(edx),"+b"(ebx),"+S"(esi),"+D"(edi),"+r"(ebp)
#define ALLOCATOR_00944E84_STEP(text) __asm__ volatile(text : ALLOCATOR_00944E84_STATE : : "memory","cc")
/* The actual access label lives at the common C return block. */
#define ALLOCATOR_00944E84_INDIRECT \
    "v_00944ef0:\n cmre_indirect_00944ef0_00944ef6:\n" \
    ".byte 0xe9\n.long 0x00944ef0 - . - 4\n" \
    "cmre_return_00944ef0_00944ef6:\n"
unsigned long long __attribute__((naked,noinline,used,optimize("O2")))
cm_fn_00944e84(u32 arg __attribute__((unused))) {
    register u32 eax __asm__("eax"),ecx __asm__("ecx"),edx __asm__("edx");
    register u32 ebx __asm__("ebx"),esi __asm__("esi"),edi __asm__("edi"),ebp __asm__("ebp");
    __asm__ volatile("" : "=a"(eax),"=c"(ecx),"=d"(edx),"=b"(ebx),"=S"(esi),"=D"(edi),"=r"(ebp));
    ALLOCATOR_00944E84_STEP("v_00944e84:\n");
    eax=*(volatile allocator_00944e84_word *)0x00de4404;
    u8 mode3;
    __asm__ volatile("v_00944e89:\n pushl %%esi\n v_00944e8a:\n movl 8(%%esp),%%esi\n"
                     "v_00944e8e:\n cmpl $3,%%eax\n v_00944e91:\n"
        : ALLOCATOR_00944E84_STATE,"=@ccz"(mode3) : : "memory","cc");
    if (mode3) {
        u8 above;
        __asm__ volatile("v_00944e93:\n cmpl 0x00de43fc,%%esi\n v_00944e99:\n"
            : ALLOCATOR_00944E84_STATE,"=@cca"(above) : : "memory","cc");
        if (!above) {
            ALLOCATOR_00944E84_STEP("v_00944e9b:\n pushl %%esi\n"
                CM_ORIGINAL_CALL(00944e9c,00944ea1,00948ee4));
            u8 allocated;
            __asm__ volatile("testl %%eax,%%eax\n v_00944ea3:\n popl %%ecx\n v_00944ea4:\n"
                : ALLOCATOR_00944E84_STATE,"=@ccnz"(allocated) : : "memory","cc");
            if (allocated) {
                ALLOCATOR_00944E84_STEP("v_00944ea6:\n popl %%esi\n v_00944ea7:\n ret");
                __builtin_unreachable();
            }
        }
        goto fallback_round;
    }
    u8 mode2;
    __asm__ volatile("v_00944ea8:\n cmpl $2,%%eax\n v_00944eab:\n"
        : ALLOCATOR_00944E84_STATE,"=@ccz"(mode2) : : "memory","cc");
    if (mode2) {
        u8 nonzero;
        __asm__ volatile("v_00944ead:\n movl 8(%%esp),%%eax\n v_00944eb1:\n testl %%eax,%%eax\n v_00944eb3:\n"
            : ALLOCATOR_00944E84_STATE,"=@ccnz"(nonzero) : : "memory","cc");
        if (nonzero) {
            ALLOCATOR_00944E84_STEP("v_00944eb5:\n leal 15(%%eax),%%esi\n"
                                  "v_00944eb8:\n andl $-16,%%esi\n");
        } else {
            ALLOCATOR_00944E84_STEP("v_00944ebd:\n pushl $16\n v_00944ebf:\n popl %%esi\n");
        }
        u8 above;
        __asm__ volatile("v_00944ec0:\n cmpl 0x00ad8d7c,%%esi\n v_00944ec6:\n"
            : ALLOCATOR_00944E84_STATE,"=@cca"(above) : : "memory","cc");
        if (!above) {
            ALLOCATOR_00944E84_STEP("v_00944ec8:\n movl %%esi,%%eax\n v_00944eca:\n shrl $4,%%eax\n"
                "v_00944ecd:\n pushl %%eax\n" CM_ORIGINAL_CALL(00944ece,00944ed3,00949987));
            u8 allocated;
            __asm__ volatile("testl %%eax,%%eax\n v_00944ed5:\n popl %%ecx\n v_00944ed6:\n"
                : ALLOCATOR_00944E84_STATE,"=@ccnz"(allocated) : : "memory","cc");
            if (allocated) goto return_actual;
        }
        goto heap_call;
    }
fallback_round:
    {
        u8 nonzero;
        __asm__ volatile("v_00944eda:\n testl %%esi,%%esi\n v_00944edc:\n"
            : ALLOCATOR_00944E84_STATE,"=@ccnz"(nonzero) : : "memory","cc");
        if (!nonzero) {
            ALLOCATOR_00944E84_STEP("v_00944ede:\n pushl $1\n v_00944ee0:\n popl %%esi\n");
        }
        ALLOCATOR_00944E84_STEP("v_00944ee1:\n addl $15,%%esi\n v_00944ee4:\n andl $-16,%%esi\n");
    }
heap_call:
    ALLOCATOR_00944E84_STEP("v_00944ee7:\n pushl %%esi\n v_00944ee8:\n pushl $0\n"
        "v_00944eea:\n pushl 0x00de4400\n" ALLOCATOR_00944E84_INDIRECT);
return_actual:
    ALLOCATOR_00944E84_STEP("v_00944ef6:\n popl %%esi\n v_00944ef7:\n ret");
    __builtin_unreachable();
}
