/* Deallocator caller at preferred-image VA 0x009452ca, image base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Modes, ownership and API behavior remain unknown. Original helpers/imports
   are explicit boundaries. EDX:EAX transports incidental physical registers;
   the null path deliberately retains incoming EAX and EDX. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00948b90(u32);
extern unsigned long long CM_CDECL cm_fn_00948bbb(u32, u32);
extern unsigned long long CM_CDECL cm_fn_009498eb(u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00949942(u32, u32, u32);

#define DEALLOC_009452CA_STATE \
    "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(ebx), "+S"(esi), "+D"(edi), "+r"(ebp)
#define DEALLOC_009452CA_STEP(text) \
    __asm__ volatile(text : DEALLOC_009452CA_STATE : : "memory", "cc")

/* C routes the null, mode, lookup, fallback and success paths. Adapters retain
   the original frame, actual callee-returned registers/ESP and argument words.
   The public indirect-call return label is adjacent to the actual epilogue;
   other C paths tail-jump there without an extra CALL or stack write. */
unsigned long long CM_CDECL
__attribute__((naked, noinline, used,
    optimize("O2", "align-loops=1", "align-jumps=1", "align-labels=1")))
cm_fn_009452ca(u32 arg4 __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    register u32 ebx __asm__("ebx"), esi __asm__("esi");
    register u32 edi __asm__("edi"), ebp __asm__("ebp");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx), "=b"(ebx),
                         "=S"(esi), "=D"(edi), "=r"(ebp));
    u8 nonnull;
    __asm__ volatile(
        "v_009452ca:\n pushl %%ebp\n"
        "v_009452cb:\n movl %%esp, %%ebp\n"
        "v_009452cd:\n pushl %%ecx\n"
        "v_009452ce:\n pushl %%esi\n"
        "v_009452cf:\n movl 8(%%ebp), %%esi\n"
        "v_009452d2:\n testl %%esi, %%esi\n v_009452d4:\n"
        : DEALLOC_009452CA_STATE, "=@ccnz"(nonnull) : : "memory", "cc");
    if (!nonnull)
        goto done;
    DEALLOC_009452CA_STEP("v_009452d6:\n");
    eax = CM_GLOBAL(volatile u32, 0x00de4404);
    u8 mode3;
    __asm__ volatile("v_009452db:\n cmpl $3, %%eax\n v_009452de:\n"
        : DEALLOC_009452CA_STATE, "=@ccz"(mode3) : : "memory", "cc");
    if (mode3) {
        DEALLOC_009452CA_STEP("v_009452e0:\n pushl %%esi\n"
            CM_ORIGINAL_CALL(009452e1, 009452e6, 00948b90)
            "popl %%ecx\n");
        u8 found;
        /* PUSH precedes JE and preserves the TEST flags on both paths. */
        __asm__ volatile("v_009452e7:\n testl %%eax, %%eax\n"
                         "v_009452e9:\n pushl %%esi\n v_009452ea:\n"
            : DEALLOC_009452CA_STATE, "=@ccnz"(found) : : "memory", "cc");
        if (found) {
            DEALLOC_009452CA_STEP("v_009452ec:\n pushl %%eax\n"
                CM_ORIGINAL_CALL(009452ed, 009452f2, 00948bbb)
                "popl %%ecx\n v_009452f3:\n popl %%ecx\n v_009452f4:\n");
            goto done;
        }
        goto heap_tail;
    }
    u8 mode2;
    __asm__ volatile("v_009452f6:\n cmpl $2, %%eax\n v_009452f9:\n"
        : DEALLOC_009452CA_STATE, "=@ccz"(mode2) : : "memory", "cc");
    if (mode2) {
        DEALLOC_009452CA_STEP(
            "v_009452fb:\n leal 8(%%ebp), %%eax\n"
            "v_009452fe:\n pushl %%eax\n"
            "v_009452ff:\n leal -4(%%ebp), %%eax\n"
            "v_00945302:\n pushl %%eax\n"
            "v_00945303:\n pushl %%esi\n"
            CM_ORIGINAL_CALL(00945304, 00945309, 009498eb));
        u8 found;
        __asm__ volatile("addl $12, %%esp\n v_0094530c:\n testl %%eax, %%eax\n v_0094530e:\n"
            : DEALLOC_009452CA_STATE, "=@ccnz"(found) : : "memory", "cc");
        if (found) {
            /* Forward actual output slots after the original lookup, including
               its mutation of the caller's argument at EBP+8. */
            DEALLOC_009452CA_STEP(
                "v_00945310:\n pushl %%eax\n"
                "v_00945311:\n pushl 8(%%ebp)\n"
                "v_00945314:\n pushl -4(%%ebp)\n"
                CM_ORIGINAL_CALL(00945317, 0094531c, 00949942)
                "addl $12, %%esp\n v_0094531f:\n");
            goto done;
        }
    }
    DEALLOC_009452CA_STEP("v_00945321:\n pushl %%esi\n");
heap_tail:
    DEALLOC_009452CA_STEP(
        "v_00945322:\n pushl $0\n"
        "v_00945324:\n pushl 0x00de4400\n"
        CM_ORIGINAL_INDIRECT_CALL(0094532a, 00945330)
        "_dealloc_009452ca_epilogue:\n popl %%esi\n"
        "v_00945331:\n leave\n v_00945332:\n ret");
    __builtin_unreachable();
done:
    DEALLOC_009452CA_STEP("jmp _dealloc_009452ca_epilogue");
    __builtin_unreachable();
}
