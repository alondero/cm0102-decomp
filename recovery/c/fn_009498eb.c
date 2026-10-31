/* Linked lookup at preferred-image VA 009498eb, image base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Record/range/output meanings remain opaque. Traversal has no cycle limit.
   EDX:EAX transports incidental registers, not a semantic 64-bit return. */
#include "cmre.h"
#define LOOKUP_009498EB_STATE "+a"(eax), "+c"(ecx), "+d"(edx), "+S"(esi)
#define LOOKUP_009498EB_STEP(text) \
    __asm__ volatile(text : LOOKUP_009498EB_STATE : : "memory", "cc")
#define LOOKUP_009498EB_TEST(text, condition, result) \
    __asm__ volatile(text : LOOKUP_009498EB_STATE, "=@cc" condition(result) : : "memory", "cc")

/* C controls strict unsigned range tests, sentinel traversal and rejection.
   Narrow physical adapters preserve memory/fault order and original flags.
   Only the four used registers are constrained; EBX/EDI/EBP remain untouched. */
unsigned long long CM_CDECL
__attribute__((naked, noinline, used, optimize("O2", "no-tree-ch", "align-loops=1", "align-jumps=1", "align-labels=1")))
cm_fn_009498eb(u32 arg4 __attribute__((unused)),
               u32 arg8 __attribute__((unused)),
               u32 argC __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx"), esi __asm__("esi");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx), "=S"(esi));
    LOOKUP_009498EB_STEP("v_009498eb:\n\tmovl 4(%%esp), %%eax\n"
                         "v_009498ef:\n\tmovl $0x00ad6d58, %%edx\n"
                         "v_009498f4:\n\tpushl %%esi\n v_009498f5:\n\tmovl %%edx, %%ecx");
    for (;;) {
        u8 above_lower;
        LOOKUP_009498EB_TEST("v_009498f7:\n\tcmpl 0x10(%%ecx), %%eax\n v_009498fa:\n", "a", above_lower);
        if (above_lower) {
            u8 below_upper;
            LOOKUP_009498EB_TEST("v_009498fc:\n\tcmpl 0x14(%%ecx), %%eax\n v_009498ff:\n", "b", below_upper);
            if (below_upper) break;
        }
        u8 reached_sentinel;
        LOOKUP_009498EB_TEST("v_00949901:\n\tmovl (%%ecx), %%ecx\n"
                             "v_00949903:\n\tcmpl %%edx, %%ecx\n v_00949905:\n", "z", reached_sentinel);
        if (reached_sentinel) goto unavailable;
    }

    u8 low_nibble_nonzero;
    LOOKUP_009498EB_TEST("v_00949909:\n\ttestb $0x0f, %%al\n v_0094990b:\n", "nz", low_nibble_nonzero);
    if (low_nibble_nonzero) goto unavailable;
    u8 low_twelve_below_100;
    LOOKUP_009498EB_TEST("v_0094990d:\n\tmovl %%eax, %%esi\n"
                         "v_0094990f:\n\tmovl $0x100, %%edx\n"
                         "v_00949914:\n\tandl $0x0fff, %%esi\n"
                         "v_0094991a:\n\tcmpl %%edx, %%esi\n v_0094991c:\n", "b", low_twelve_below_100);
    if (low_twelve_below_100) goto unavailable;

    /* First output store precedes loading the second pointer from the caller's
       actual argument slot. It may overwrite that slot or the saved ESI word.
       AND CX is 16-bit: upper ECX survives, including at the second-store fault. */
    LOOKUP_009498EB_STEP("v_0094991e:\n\tmovl 0xc(%%esp), %%esi\n"
                         "v_00949922:\n\tmovl %%ecx, (%%esi)\n"
                         "v_00949924:\n\tmovl 0x10(%%esp), %%esi\n"
                         "v_00949928:\n\tmovl %%eax, %%ecx\n"
                         "v_0094992a:\n\tandw $0xf000, %%cx\n"
                         "v_0094992f:\n\tsubl %%ecx, %%eax\n"
                         "v_00949931:\n\tmovl %%ecx, (%%esi)\n"
                         "v_00949933:\n\tsubl %%edx, %%eax\n"
                         "v_00949935:\n\tpopl %%esi\n"
                         "v_00949936:\n\tsarl $4, %%eax\n"
                         "v_00949939:\n\tleal 8(%%eax,%%ecx), %%eax\n v_0094993d:\n\tret");
    __builtin_unreachable();
unavailable:
    LOOKUP_009498EB_STEP("v_0094993e:\n\txorl %%eax, %%eax\n"
                         "v_00949940:\n\tpopl %%esi\n v_00949941:\n\tret");
    __builtin_unreachable();
}
