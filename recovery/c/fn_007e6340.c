/* Preferred-image VA007e6340, base00400000; targetSHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Instruction-backed C candidate. Table and ownership meanings remain opaque; EDX:EAX is incidental.
   No additional return word/frame: the original tail transfer enters base cleanup. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_0048c650(void *);
typedef u32 derived_007e6340_word __attribute__((aligned(1),may_alias));
#define DERIVED_007E6340_STATE "+c"(self)
#define DERIVED_007E6340_STEP(text) __asm__ volatile(text : DERIVED_007E6340_STATE : : "memory", "cc")
unsigned long long CM_THISCALL __attribute__((naked,noinline,used,optimize("O2")))
cm_fn_007e6340(void *arg __attribute__((unused))) {
    register u8 *self __asm__("ecx");
    __asm__ volatile("" : "=c"(self));
    DERIVED_007E6340_STEP("v_007e6340:\n");
    *(volatile derived_007e6340_word *)self=0x0096efe4;
    DERIVED_007E6340_STEP("v_007e6346:\n.byte 0xe9\n.long _cm_fn_0048c650 - . - 4\n");
    __builtin_unreachable();
}
