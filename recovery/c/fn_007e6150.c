/* Candidate cleanup at preferred-image VA007e6150, image base00400000.
   Target SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Opaque members and vtable meanings remain unknown. Original FS-chain and
   handler00960718 bytes are preserved; no exception/unwind behavior inferred.
   EDX:EAX retains incidental registers, not a semantic 64-bit return.
   Packet candidate derived from the reviewed draft; acceptance is pending. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_0048c650(void *);

typedef u32 cleanup_007e6150_word __attribute__((aligned(1), may_alias));
#define CLEANUP_007E6150_STATE \
    "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(ebx), "+S"(working_self), "+D"(edi), "+r"(ebp)
#define CLEANUP_007E6150_STEP(text) \
    __asm__ volatile(text : CLEANUP_007E6150_STATE : : "memory", "cc")

/* Keep the VM continuation marker adjacent to its native JMP. Access labels
   live at the C joins after each conditional block, so fault locations describe
   actual accesses even if GCC reorders the conditional body. */
#define CLEANUP_007E6150_INDIRECT(callva, returnva) \
    "v_" #callva ":\n" \
    "cmre_indirect_" #callva "_" #returnva ":\n" \
    ".byte 0xe9\n.long 0x" #callva " - . - 4\n" \
    "cmre_return_" #callva "_" #returnva ":\n"

unsigned long long CM_THISCALL
__attribute__((naked, noinline, used, optimize("O2")))
cm_fn_007e6150(void *self_arg __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    register u32 ebx __asm__("ebx"), edi __asm__("edi"), ebp __asm__("ebp");
    register u8 *working_self __asm__("esi");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx), "=b"(ebx),
                         "=S"(working_self), "=D"(edi), "=r"(ebp));
    CLEANUP_007E6150_STEP(
        "v_007e6150:\n\tpushl $-1\n"
        "v_007e6152:\n\tpushl $0x00960718\n"
        "v_007e6157:\n\tmovl %%fs:0, %%eax\n"
        "v_007e615d:\n\tpushl %%eax\n"
        "v_007e615e:\n\tmovl %%esp, %%fs:0\n"
        "v_007e6165:\n\tpushl %%ecx\n"
        "v_007e6166:\n\tpushl %%esi\n"
        "v_007e6167:\n\tmovl %%ecx, %%esi\n"
        "v_007e6169:\n\tmovl %%esi, 4(%%esp)\n"
        "v_007e616d:\n\tmovl $0x0096ef54, (%%esi)\n"
        "v_007e6173:\n");

    /* Do not cache the second member before the first dynamic call. */
    ecx = *(volatile cleanup_007e6150_word *)(working_self + 0xa3);
    u8 first_nonnull;
    __asm__ volatile("v_007e6179:\n\tmovl $0, 16(%%esp)\n"
                     "v_007e6181:\n\ttestl %%ecx, %%ecx\n v_007e6183:\n"
                     : CLEANUP_007E6150_STATE, "=@ccnz"(first_nonnull)
                     : : "memory", "cc");
    if (first_nonnull) {
        CLEANUP_007E6150_STEP(
            "v_007e6185:\n\tmovl (%%ecx), %%eax\n"
            "v_007e6187:\n\tpushl $1\n"
            CLEANUP_007E6150_INDIRECT(007e6189, 007e618b));
    }
    CLEANUP_007E6150_STEP("v_007e618b:\n");
    ecx = *(volatile cleanup_007e6150_word *)(working_self + 0xa7);
    u8 second_nonnull;
    __asm__ volatile("v_007e6191:\n\ttestl %%ecx, %%ecx\n v_007e6193:\n"
                     : CLEANUP_007E6150_STATE, "=@ccnz"(second_nonnull)
                     : : "memory", "cc");
    if (second_nonnull) {
        CLEANUP_007E6150_STEP(
            "v_007e6195:\n\tmovl (%%ecx), %%edx\n"
            "v_007e6197:\n\tpushl $1\n"
            CLEANUP_007E6150_INDIRECT(007e6199, 007e619b));
    }
    /* All following offsets use actual returned ESP. Dynamic cleanup is not
       presumed; ESI and saved-stack aliases keep their original consequences. */
    CLEANUP_007E6150_STEP(
        "v_007e619b:\n\tmovl %%esi, %%ecx\n"
        "v_007e619d:\n\tmovl $-1, 16(%%esp)\n"
        CM_ORIGINAL_CALL(007e61a5, 007e61aa, 0048c650)
        "movl 8(%%esp), %%ecx\n"
        "v_007e61ae:\n\tpopl %%esi\n"
        "v_007e61af:\n\tmovl %%ecx, %%fs:0\n"
        "v_007e61b6:\n\taddl $16, %%esp\n"
        "v_007e61b9:\n\tret");
    __builtin_unreachable();
}
