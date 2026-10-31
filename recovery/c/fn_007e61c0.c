/* Constructor at preferred-image VA 0x007e61c0, image base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Registry, index and field meanings remain unknown. Original allocation and
   initializer calls remain boundaries; no heap initialization is inferred.
   The exact FS-chain frame and handler 00960746 are retained without claiming
   exception/unwind behavior. EDX:EAX transports incidental register values. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_THISCALL cm_fn_0048c580(void *);

typedef u32 constructor_007e61c0_word __attribute__((aligned(1), may_alias));
#define CONSTRUCTOR_007E61C0_STATE \
    "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(ebx), "+S"(esi), "+D"(edi), "+r"(ebp)
#define CONSTRUCTOR_007E61C0_STEP(text) \
    __asm__ volatile(text : CONSTRUCTOR_007E61C0_STATE : : "memory", "cc")

/* Fixed-GPR adapters preserve all fault registers, original stack writes and
   actual callee cleanup. C performs ordered volatile reads, allocation routing
   and the flag-neutral LEA arithmetic. Stores are pinned because a generic C
   store can spill seven live registers into the original frame. Both emitted
   modes must be inspected from the function entry, before its first label. */
unsigned long long CM_THISCALL
__attribute__((naked, noinline, used, optimize("O2")))
cm_fn_007e61c0(void *self_arg __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    register u32 ebx __asm__("ebx"), esi __asm__("esi");
    register u32 edi __asm__("edi"), ebp __asm__("ebp");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx), "=b"(ebx),
                         "=S"(esi), "=D"(edi), "=r"(ebp));
    CONSTRUCTOR_007E61C0_STEP(
        "v_007e61c0:\n pushl $-1\n"
        "v_007e61c2:\n pushl $0x00960746\n"
        "v_007e61c7:\n movl %%fs:0, %%eax\n"
        "v_007e61cd:\n pushl %%eax\n"
        "v_007e61ce:\n movl %%esp, %%fs:0\n"
        "v_007e61d5:\n pushl %%ecx\n"
        "v_007e61d6:\n");
    eax = CM_GLOBAL(volatile u32, 0x00adadfc);
    CONSTRUCTOR_007E61C0_STEP(
        "v_007e61db:\n pushl %%esi\n"
        "v_007e61dc:\n pushl %%edi\n"
        "v_007e61dd:\n movl %%ecx, %%edi\n"
        "v_007e61df:\n");
    ecx = CM_GLOBAL(volatile u32, 0x009cf684);
    CONSTRUCTOR_007E61C0_STEP(
        "v_007e61e5:\n pushl $0xa3\n"
        "v_007e61ea:\n movl %%edi, (%%eax,%%ecx,4)\n"
        CM_ORIGINAL_CALL(007e61ed, 007e61f2, 00944cf1));
    esi = eax;
    u8 first_nonnull;
    __asm__ volatile(
        "v_007e61f4:\n addl $4, %%esp\n"
        "v_007e61f7:\n movl %%esi, 8(%%esp)\n"
        "v_007e61fb:\n testl %%esi, %%esi\n"
        "v_007e61fd:\n movl $0, 20(%%esp)\n"
        "v_007e6205:\n"
        : CONSTRUCTOR_007E61C0_STATE, "=@ccnz"(first_nonnull)
        : : "memory", "cc");
    if (first_nonnull) {
        CONSTRUCTOR_007E61C0_STEP(
            "v_007e6207:\n movl %%esi, %%ecx\n"
            CM_ORIGINAL_CALL(007e6209, 007e620e, 0048c580)
            "movl $0x0096f074, (%%esi)\n"
            "v_007e6214:\n");
        eax = CM_GLOBAL(volatile u32, 0x009cf794);
        CONSTRUCTOR_007E61C0_STEP("v_007e6219:\n");
        ecx = eax * 3u;
        CONSTRUCTOR_007E61C0_STEP("v_007e621c:\n");
        edx = ecx * 9u;
        CONSTRUCTOR_007E61C0_STEP(
            "v_007e621f:\n shll $2, %%edx\n"
            "v_007e6222:\n subl %%eax, %%edx\n"
            "v_007e6224:\n");
        /* Base is the dword stored at AE23D0, not its literal VA. */
        eax = CM_GLOBAL(volatile u32, 0x00ae23d0);
        CONSTRUCTOR_007E61C0_STEP(
            "v_007e6229:\n addl %%eax, %%edx\n"
            "v_007e622b:\n movl %%edx, 4(%%esi)\n"
            "v_007e622e:\n");
    } else {
        CONSTRUCTOR_007E61C0_STEP("v_007e6230:\n xorl %%esi, %%esi");
    }
    /* Both globals are reread after the self/member store. Aliases can change
       the registry base or the index between any of these accesses. */
    CONSTRUCTOR_007E61C0_STEP(
        "v_007e6232:\n movl %%esi, 163(%%edi)\n"
        "v_007e6238:\n");
    eax = CM_GLOBAL(volatile u32, 0x00adadfc);
    CONSTRUCTOR_007E61C0_STEP("v_007e623d:\n");
    ecx = CM_GLOBAL(volatile u32, 0x009cf794);
    CONSTRUCTOR_007E61C0_STEP(
        "v_007e6243:\n pushl $0xa3\n"
        "v_007e6248:\n movl $-1, 24(%%esp)\n"
        "v_007e6250:\n movl %%esi, (%%eax,%%ecx,4)\n"
        CM_ORIGINAL_CALL(007e6253, 007e6258, 00944cf1));
    esi = eax;
    u8 second_nonnull;
    __asm__ volatile(
        "v_007e625a:\n addl $4, %%esp\n"
        "v_007e625d:\n movl %%esi, 8(%%esp)\n"
        "v_007e6261:\n testl %%esi, %%esi\n"
        "v_007e6263:\n movl $1, 20(%%esp)\n"
        "v_007e626b:\n"
        : CONSTRUCTOR_007E61C0_STATE, "=@ccnz"(second_nonnull)
        : : "memory", "cc");
    if (second_nonnull) {
        CONSTRUCTOR_007E61C0_STEP(
            "v_007e626d:\n movl %%esi, %%ecx\n"
            CM_ORIGINAL_CALL(007e626f, 007e6274, 0048c580)
            "movl $0x0096efe4, (%%esi)\n"
            "v_007e627a:\n");
        eax = CM_GLOBAL(volatile u32, 0x009cf798);
        CONSTRUCTOR_007E61C0_STEP("v_007e627f:\n");
        ecx = eax * 3u;
        CONSTRUCTOR_007E61C0_STEP("v_007e6282:\n");
        edx = ecx * 9u;
        CONSTRUCTOR_007E61C0_STEP(
            "v_007e6285:\n shll $2, %%edx\n"
            "v_007e6288:\n subl %%eax, %%edx\n"
            "v_007e628a:\n");
        eax = CM_GLOBAL(volatile u32, 0x00ae23d0);
        CONSTRUCTOR_007E61C0_STEP(
            "v_007e628f:\n addl %%eax, %%edx\n"
            "v_007e6291:\n movl %%edx, 4(%%esi)\n"
            "v_007e6294:\n");
    } else {
        CONSTRUCTOR_007E61C0_STEP("v_007e6296:\n xorl %%esi, %%esi");
    }
    CONSTRUCTOR_007E61C0_STEP(
        "v_007e6298:\n movl %%esi, 167(%%edi)\n"
        "v_007e629e:\n");
    eax = CM_GLOBAL(volatile u32, 0x00adadfc);
    CONSTRUCTOR_007E61C0_STEP("v_007e62a3:\n");
    ecx = CM_GLOBAL(volatile u32, 0x009cf798);
    CONSTRUCTOR_007E61C0_STEP(
        "v_007e62a9:\n popl %%edi\n"
        "v_007e62aa:\n movl %%esi, (%%eax,%%ecx,4)\n"
        "v_007e62ad:\n");
    eax = CM_GLOBAL(volatile u32, 0x00adadfc);
    CONSTRUCTOR_007E61C0_STEP("v_007e62b2:\n");
    edx = CM_GLOBAL(volatile u32, 0x009cf5cc);
    CONSTRUCTOR_007E61C0_STEP(
        "v_007e62b8:\n popl %%esi\n"
        /* Generic C indexed reads split into LEA/MOV, changing fault GPRs. */
        "v_007e62b9:\n movl (%%eax,%%edx,4), %%ecx\n");
    u8 first_entry_nonnull;
    __asm__ volatile("v_007e62bc:\n testl %%ecx, %%ecx\n v_007e62be:\n"
        : CONSTRUCTOR_007E61C0_STATE, "=@ccnz"(first_entry_nonnull)
        : : "memory", "cc");
    if (first_entry_nonnull) {
        CONSTRUCTOR_007E61C0_STEP("v_007e62c0:\n");
        edx = CM_GLOBAL(volatile u32, 0x009cf5d8);
        CONSTRUCTOR_007E61C0_STEP("v_007e62c6:\n");
        ecx = *(volatile constructor_007e61c0_word *)(ecx + 0xeeu);
        CONSTRUCTOR_007E61C0_STEP(
            "v_007e62cc:\n movl %%ecx, (%%eax,%%edx,4)\n"
            "v_007e62cf:\n");
        eax = CM_GLOBAL(volatile u32, 0x00adadfc);
        CONSTRUCTOR_007E61C0_STEP("v_007e62d4:\n");
        ecx = CM_GLOBAL(volatile u32, 0x009cf5cc);
        CONSTRUCTOR_007E61C0_STEP("v_007e62da:\n movl (%%eax,%%ecx,4), %%edx");
        CONSTRUCTOR_007E61C0_STEP("v_007e62dd:\n");
        ecx = *(volatile constructor_007e61c0_word *)(edx + 0xf2u);
        CONSTRUCTOR_007E61C0_STEP("v_007e62e3:\n");
        edx = CM_GLOBAL(volatile u32, 0x009cf548);
        CONSTRUCTOR_007E61C0_STEP(
            "v_007e62e9:\n movl %%ecx, (%%eax,%%edx,4)\n"
            "v_007e62ec:\n");
        eax = CM_GLOBAL(volatile u32, 0x00adadfc);
    }
    CONSTRUCTOR_007E61C0_STEP("v_007e62f1:\n");
    ecx = CM_GLOBAL(volatile u32, 0x009cf624);
    CONSTRUCTOR_007E61C0_STEP("v_007e62f7:\n movl (%%eax,%%ecx,4), %%ecx");
    u8 final_entry_nonnull;
    __asm__ volatile("v_007e62fa:\n testl %%ecx, %%ecx\n v_007e62fc:\n"
        : CONSTRUCTOR_007E61C0_STATE, "=@ccnz"(final_entry_nonnull)
        : : "memory", "cc");
    if (final_entry_nonnull) {
        CONSTRUCTOR_007E61C0_STEP("v_007e62fe:\n");
        edx = *(volatile constructor_007e61c0_word *)(ecx + 0xeeu);
        CONSTRUCTOR_007E61C0_STEP("v_007e6304:\n");
        ecx = CM_GLOBAL(volatile u32, 0x009cf6b0);
        CONSTRUCTOR_007E61C0_STEP("v_007e630a:\n movl %%edx, (%%eax,%%ecx,4)");
    }
    CONSTRUCTOR_007E61C0_STEP(
        "v_007e630d:\n movl 4(%%esp), %%ecx\n"
        "v_007e6311:\n movl %%ecx, %%fs:0\n"
        "v_007e6318:\n addl $16, %%esp\n"
        "v_007e631b:\n ret");
    __builtin_unreachable();
}
