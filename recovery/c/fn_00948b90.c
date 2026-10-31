/* Range lookup at preferred-image VA 0x00948b90, image base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Record/range meanings and ownership remain unknown. Arithmetic wraps at
   32 bits; malformed addresses and original loops are retained. EDX:EAX is
   physical validation state, not a semantic 64-bit return declaration. */
#include "cmre.h"

#define LOOKUP_00948B90_STATE "+a"(cursor), "+c"(end), "+d"(offset)
#define LOOKUP_00948B90_STEP(text) \
    __asm__ volatile(text : LOOKUP_00948B90_STATE : : "memory", "cc")

/* Only the three original working registers participate in the bridge.
   No scratch frame, saved registers, argument cache or EDX initialization is
   introduced. Inspect the complete emitted function under the pinned GCC. */
unsigned long long CM_CDECL
__attribute__((naked, noinline, used,
    optimize("O2", "no-tree-ch", "align-loops=1", "align-jumps=1", "align-labels=1")))
cm_fn_00948b90(u32 arg __attribute__((unused)))
{
    register u32 cursor __asm__("eax");
    register u32 end __asm__("ecx");
    register u32 offset __asm__("edx");
    __asm__ volatile("" : "=a"(cursor), "=c"(end), "=d"(offset));
    LOOKUP_00948B90_STEP("v_00948b90:\n");
    cursor = CM_GLOBAL(volatile u32, 0x00de43f4);
    LOOKUP_00948B90_STEP("v_00948b95:\n");
    end = cursor * 5u;
    LOOKUP_00948B90_STEP("v_00948b98:\n");
    cursor = CM_GLOBAL(volatile u32, 0x00de43f8);
    LOOKUP_00948B90_STEP("v_00948b9d:\n");
    end = cursor + end * 4u;
    for (;;) {
        u8 exhausted;
        __asm__ volatile("v_00948ba0:\n cmpl %%ecx, %%eax\n v_00948ba2:\n"
            : LOOKUP_00948B90_STATE, "=@ccae"(exhausted)
            : : "memory", "cc");
        if (exhausted) {
            LOOKUP_00948B90_STEP("v_00948bb8:\n xorl %%eax, %%eax");
            break;
        }
        /* The original reads the actual stack slot on every iteration. */
        /* Generic C subtraction uses unsaved EBX for the memory operand under
           pinned GCC. Keep the original single-instruction access/fault state. */
        LOOKUP_00948B90_STEP("v_00948ba4:\n movl 4(%%esp), %%edx\n"
                             "v_00948ba8:\n subl 12(%%eax), %%edx\n");
        u8 contains;
        __asm__ volatile("v_00948bab:\n cmpl $0x100000, %%edx\n v_00948bb1:\n"
            : LOOKUP_00948B90_STATE, "=@ccb"(contains)
            : : "memory", "cc");
        if (contains)
            break;
        LOOKUP_00948B90_STEP("v_00948bb3:\n");
        cursor += 20u;
        LOOKUP_00948B90_STEP("v_00948bb6:\n");
    }
    __asm__ volatile("v_00948bba:\n ret"
        : : "a"(cursor), "c"(end), "d"(offset) : "memory");
    __builtin_unreachable();
}
