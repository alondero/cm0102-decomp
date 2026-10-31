/* Preferred-image VA00944e58, base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Opaque size/flag allocation retry bridge. Callees remain explicit original
   boundaries. EDX:EAX transports incidental state, not a semantic return type.
   Actual returned ESP is used at each argument read and POP; no retry limit. */
#include "cmre.h"
extern unsigned long long cm_fn_00944e84(u32);
extern unsigned long long cm_fn_0094895b(u32);
#define ALLOC_00944E58_STATE "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(ebx), "+S"(esi), "+D"(edi), "+r"(ebp)
#define ALLOC_00944E58_STEP(text) __asm__ volatile(text : ALLOC_00944E58_STATE : : "memory", "cc")
unsigned long long __attribute__((naked,noinline,used,optimize("O2")))
cm_fn_00944e58(u32 size __attribute__((unused)),u32 flag __attribute__((unused))) {
    register u32 eax __asm__("eax"),ecx __asm__("ecx"),edx __asm__("edx");
    register u32 ebx __asm__("ebx"),esi __asm__("esi"),edi __asm__("edi"),ebp __asm__("ebp");
    __asm__ volatile("" : "=a"(eax),"=c"(ecx),"=d"(edx),"=b"(ebx),"=S"(esi),"=D"(edi),"=r"(ebp));
    u8 too_large;
    __asm__ volatile("v_00944e58:\n cmpl $-32,4(%%esp)\n v_00944e5d:\n"
        : ALLOC_00944E58_STATE,"=@cca"(too_large) : : "memory","cc");
    if (!too_large) {
        do {
            ALLOC_00944E58_STEP("v_00944e5f:\n pushl 4(%%esp)\n"
                CM_ORIGINAL_CALL(00944e63,00944e68,00944e84));
            u8 nonnull;
            __asm__ volatile("testl %%eax,%%eax\n v_00944e6a:\n popl %%ecx\n v_00944e6b:\n"
                : ALLOC_00944E58_STATE,"=@ccnz"(nonnull) : : "memory","cc");
            if (nonnull) goto return_actual;
            u8 flag_zero;
            __asm__ volatile("v_00944e6d:\n cmpl %%eax,8(%%esp)\n v_00944e71:\n"
                : ALLOC_00944E58_STATE,"=@ccz"(flag_zero) : : "memory","cc");
            if (flag_zero) goto return_actual;
            ALLOC_00944E58_STEP("v_00944e73:\n pushl 4(%%esp)\n"
                CM_ORIGINAL_CALL(00944e77,00944e7c,0094895b));
            u8 retry;
            __asm__ volatile("testl %%eax,%%eax\n v_00944e7e:\n popl %%ecx\n v_00944e7f:\n"
                : ALLOC_00944E58_STATE,"=@ccnz"(retry) : : "memory","cc");
            if (!retry) break;
        } while (1);
    }
    ALLOC_00944E58_STEP("v_00944e81:\n xorl %%eax,%%eax\n");
return_actual:
    ALLOC_00944E58_STEP("v_00944e83:\n ret");
    __builtin_unreachable();
}
