/* Opaque table/byte update at preferred-image VA 00949942, base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   No heap, page, ownership or import semantics are inferred. Original 00949829
   remains an explicit boundary. EDX:EAX transports actual physical registers;
   the original semantic return type is unknown. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_00949829(u32);
#define PAGE_RELEASE_00949942_STATE "+a"(eax), "+c"(ecx), "+d"(edx)
#define PAGE_RELEASE_00949942_STEP(text) \
    __asm__ volatile(text : PAGE_RELEASE_00949942_STATE : : "memory", "cc")

/* C controls the two exact-equality branches. Physical adapters retain memory
   widths, unsigned wrap, arithmetic shift, flag/fault order and caller aliases.
   Only the three used registers are constrained; no extra frame or scratch. */
unsigned long long CM_CDECL
__attribute__((naked, noinline, used, optimize("O2", "align-loops=1", "align-jumps=1", "align-labels=1")))
cm_fn_00949942(u32 arg4 __attribute__((unused)),
               u32 arg8 __attribute__((unused)),
               u32 argc __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx));
    PAGE_RELEASE_00949942_STEP("v_00949942:\n\tmovl 4(%%esp), %%eax\n"
                               "v_00949946:\n\tmovl 8(%%esp), %%ecx\n"
                               "v_0094994a:\n\tsubl 0x10(%%eax), %%ecx\n"
                               "v_0094994d:\n\tsarl $0xc, %%ecx\n"
                               "v_00949950:\n\tleal 0x18(%%eax,%%ecx,8), %%eax\n"
                               "v_00949954:\n\tmovl 0xc(%%esp), %%ecx\n"
                               "v_00949958:\n\tmovzbl (%%ecx), %%edx\n"
                               "v_0094995b:\n\taddl %%edx, (%%eax)\n"
                               "v_0094995d:\n\tandb $0, (%%ecx)");

    /* CMP precedes the potentially faulting MOV. MOV must retain CMP flags.
       The dword ADD and byte AND above must stay ordered even when aliased. */
    u8 updated_word_is_f0;
    __asm__ volatile("v_00949960:\n\tcmpl $0xf0, (%%eax)\n"
                     "v_00949966:\n\tmovl $0xf1, 4(%%eax)\n v_0094996d:\n"
                     : PAGE_RELEASE_00949942_STATE, "=@ccz"(updated_word_is_f0)
                     : : "memory", "cc");
    if (updated_word_is_f0) {
        u8 counter_is_20;
        /* Re-read the actual dword after INC; overflow and aliases are kept. */
        __asm__ volatile("v_0094996f:\n\tincl 0x00de3010\n"
                         "v_00949975:\n\tcmpl $0x20, 0x00de3010\n v_0094997c:\n"
                         : PAGE_RELEASE_00949942_STATE, "=@ccz"(counter_is_20)
                         : : "memory", "cc");
        if (counter_is_20) {
            PAGE_RELEASE_00949942_STEP("v_0094997e:\n\tpushl $0x10\n"
                                       CM_ORIGINAL_CALL(00949980, 00949985, 00949829)
                                       "popl %%ecx");
        }
    }
    PAGE_RELEASE_00949942_STEP("v_00949986:\n\tret");
    __builtin_unreachable();
}
