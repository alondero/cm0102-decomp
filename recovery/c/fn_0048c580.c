/* Instruction-backed C candidate: preferred-image VA0048c580, base00400000.
   TargetSHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Only the original ordered fields are initialized; all holes stay untouched.
   Field meanings and ownership remain unknown. EDX:EAX captures actual registers. */
#include "cmre.h"
typedef u32 init_0048c580_dword __attribute__((aligned(1),may_alias));
typedef u16 init_0048c580_word __attribute__((aligned(1),may_alias));
#define INIT_0048C580_STATE "+a"(self), "+c"(ecx), "+d"(edx)
#define INIT_0048C580_STEP(text) __asm__ volatile(text : INIT_0048C580_STATE : : "memory", "cc")
unsigned long long CM_THISCALL __attribute__((naked,noinline,used,optimize("O2")))
cm_fn_0048c580(void *arg __attribute__((unused))) {
    register u8 *self __asm__("eax");
    register u32 ecx __asm__("ecx"),edx __asm__("edx");
    __asm__ volatile("" : "=a"(self),"=c"(ecx),"=d"(edx));
    INIT_0048C580_STEP("v_0048c580:\n movl %%ecx, %%eax\n"
                      "v_0048c582:\n orl $-1, %%edx\n"
                      "v_0048c585:\n xorl %%ecx, %%ecx\n");
    INIT_0048C580_STEP("v_0048c587:\n");
    *(volatile init_0048c580_dword *)(self + 0x0) = 0x9689e8;
    INIT_0048C580_STEP("v_0048c58d:\n");
    *(volatile init_0048c580_dword *)(self + 0x18) = (init_0048c580_dword)ecx;
    INIT_0048C580_STEP("v_0048c590:\n");
    *(volatile init_0048c580_word *)(self + 0x34) = (init_0048c580_word)ecx;
    INIT_0048C580_STEP("v_0048c594:\n");
    *(volatile u8 *)(self + 0x42) = 0x1;
    INIT_0048C580_STEP("v_0048c598:\n");
    *(volatile init_0048c580_dword *)(self + 0x20) = (init_0048c580_dword)edx;
    INIT_0048C580_STEP("v_0048c59b:\n");
    *(volatile init_0048c580_dword *)(self + 0x1c) = (init_0048c580_dword)edx;
    INIT_0048C580_STEP("v_0048c59e:\n");
    *(volatile init_0048c580_word *)(self + 0x3a) = (init_0048c580_word)ecx;
    INIT_0048C580_STEP("v_0048c5a2:\n");
    *(volatile u8 *)(self + 0x51) = 0x14;
    INIT_0048C580_STEP("v_0048c5a6:\n");
    *(volatile init_0048c580_dword *)(self + 0x28) = (init_0048c580_dword)ecx;
    INIT_0048C580_STEP("v_0048c5a9:\n");
    *(volatile init_0048c580_dword *)(self + 0x4c) = (init_0048c580_dword)ecx;
    INIT_0048C580_STEP("v_0048c5ac:\n");
    *(volatile init_0048c580_dword *)(self + 0x10) = (init_0048c580_dword)ecx;
    INIT_0048C580_STEP("v_0048c5af:\n");
    *(volatile init_0048c580_dword *)(self + 0x8) = (init_0048c580_dword)ecx;
    INIT_0048C580_STEP("v_0048c5b2:\n");
    *(volatile init_0048c580_dword *)(self + 0xc) = (init_0048c580_dword)ecx;
    INIT_0048C580_STEP("v_0048c5b5:\n");
    *(volatile init_0048c580_dword *)(self + 0x30) = (init_0048c580_dword)edx;
    INIT_0048C580_STEP("v_0048c5b8:\n");
    *(volatile init_0048c580_dword *)(self + 0x2c) = (init_0048c580_dword)ecx;
    INIT_0048C580_STEP("v_0048c5bb:\n");
    *(volatile init_0048c580_dword *)(self + 0x45) = 0x1;
    INIT_0048C580_STEP("v_0048c5c2:\n");
    *(volatile init_0048c580_word *)(self + 0x3e) = (init_0048c580_word)ecx;
    INIT_0048C580_STEP("v_0048c5c6:\n");
    *(volatile u8 *)(self + 0x52) = (u8)ecx;
    INIT_0048C580_STEP("v_0048c5c9:\n");
    *(volatile u8 *)(self + 0x49) = 0x5;
    INIT_0048C580_STEP("v_0048c5cd:\n");
    *(volatile u8 *)(self + 0x4a) = 0x3;
    INIT_0048C580_STEP("v_0048c5d1:\n");
    *(volatile init_0048c580_dword *)(self + 0x14) = (init_0048c580_dword)ecx;
    INIT_0048C580_STEP("v_0048c5d4:\n");
    *(volatile init_0048c580_word *)(self + 0x38) = (init_0048c580_word)ecx;
    INIT_0048C580_STEP("v_0048c5d8:\n");
    *(volatile init_0048c580_word *)(self + 0x36) = (init_0048c580_word)ecx;
    INIT_0048C580_STEP("v_0048c5dc:\n");
    *(volatile u8 *)(self + 0x4b) = (u8)ecx;
    INIT_0048C580_STEP("v_0048c5df:\n");
    *(volatile init_0048c580_dword *)(self + 0x24) = (init_0048c580_dword)edx;
    INIT_0048C580_STEP("v_0048c5e2:\n");
    *(volatile u8 *)(self + 0x50) = (u8)edx;
    INIT_0048C580_STEP("v_0048c5e5:\n ret");
    __builtin_unreachable();
}
