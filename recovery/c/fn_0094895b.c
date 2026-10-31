/* Ignored C draft; preferred-image VA0094895b, image base00400000.
   Target SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Opaque callback policy and non-RET unwinds remain unknown.
   EDX:EAX transports actual registers, not a semantic return declaration. */
#include "cmre.h"
typedef u32 handler_0094895b_word __attribute__((aligned(1),may_alias));
#define HANDLER_0094895B_STATE "+a"(eax),"+c"(ecx),"+d"(edx),"+b"(ebx),"+S"(esi),"+D"(edi),"+r"(ebp)
#define HANDLER_0094895B_STEP(text) __asm__ volatile(text : HANDLER_0094895B_STATE : : "memory","cc")
unsigned long long __attribute__((naked,noinline,used,optimize("O2")))
cm_fn_0094895b(u32 arg __attribute__((unused))) {
    register u32 eax __asm__("eax"),ecx __asm__("ecx"),edx __asm__("edx");
    register u32 ebx __asm__("ebx"),esi __asm__("esi"),edi __asm__("edi"),ebp __asm__("ebp");
    __asm__ volatile("" : "=a"(eax),"=c"(ecx),"=d"(edx),"=b"(ebx),"=S"(esi),"=D"(edi),"=r"(ebp));
    HANDLER_0094895B_STEP("v_0094895b:\n");
    eax=*(volatile handler_0094895b_word *)0x00de300c;
    u8 present;
    __asm__ volatile("v_00948960:\n testl %%eax,%%eax\n v_00948962:\n"
        : HANDLER_0094895B_STATE,"=@ccnz"(present) : : "memory","cc");
    if (present) {
        HANDLER_0094895B_STEP("v_00948964:\n pushl 4(%%esp)\n"
                             CM_ORIGINAL_INDIRECT_CALL(00948968,0094896a));
        u8 nonzero;
        __asm__ volatile("testl %%eax,%%eax\n v_0094896c:\n popl %%ecx\n v_0094896d:\n"
            : HANDLER_0094895B_STATE,"=@ccnz"(nonzero) : : "memory","cc");
        if (nonzero) {
            HANDLER_0094895B_STEP("v_0094896f:\n pushl $1\n v_00948971:\n popl %%eax\n v_00948972:\n ret");
            __builtin_unreachable();
        }
    }
    HANDLER_0094895B_STEP("v_00948973:\n xorl %%eax,%%eax\n v_00948975:\n ret");
    __builtin_unreachable();
}
