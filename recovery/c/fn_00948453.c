/* Preferred-image VA 00948453, image base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Increment cursor by eight, reread into ECX, then load EAX and EDX in that order. Preserve aliases, wrapping, flags and faults; physical result is EDX:EAX.
   Pinned mixed naked GCC C/assembly, source-local O2 in both outer modes.
   Requires complete emission/raw review; no portable/native exception claim. */
#include "cmre.h"
#define R_00948453_STATE "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(ebx), "+S"(esi), "+D"(edi), "+r"(ebp)
#define R_00948453_STEP(text) __asm__ volatile(".intel_syntax noprefix\n" text "\n.att_syntax prefix\n" : R_00948453_STATE : : "memory", "cc")
#define R_00948453_TEST(text, cc) __asm__ volatile(".intel_syntax noprefix\n" text "\n.att_syntax prefix\n" : R_00948453_STATE, "=@cc" cc (branch) : : "memory", "cc")
unsigned long long CM_CDECL __attribute__((naked, noinline, used,
    optimize("O2", "no-tree-ch", "align-loops=1", "align-jumps=1", "align-labels=1")))
cm_fn_00948453(u32 cursor __attribute__((unused)))
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    register u32 ebx __asm__("ebx"), esi __asm__("esi"), edi __asm__("edi");
    register u32 ebp __asm__("ebp");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx), "=b"(ebx), "=S"(esi), "=D"(edi), "=r"(ebp));
    R_00948453_STEP(
        "v_00948453: mov eax, dword ptr [esp + 4]\n"
        "v_00948457: add dword ptr [eax], 8\n"
        "v_0094845a: mov ecx, dword ptr [eax]\n"
        "v_0094845c: mov eax, dword ptr [ecx - 8]\n"
        "v_0094845f: mov edx, dword ptr [ecx - 4]\n"
        "v_00948462: ret\n");
    __builtin_unreachable();
}
#undef R_00948453_STATE
#undef R_00948453_STEP
#undef R_00948453_TEST
