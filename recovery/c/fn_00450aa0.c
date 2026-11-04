/* VA 0x00450aa0, original 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Incoming ECX and three caller stack words are forwarded in the exact
   original order. The first callee returns EDX:EAX and pops one word; the
   second receives those words and the surviving duplicate as its last arg.
   Later stack reads occur after the first call, retaining alias behavior.
   Owned CALL transport preserves original return words and is a VM boundary,
   not standalone native binding. This is an instruction-level assembly bridge,
   not a readable C reconstruction of the underlying game behavior. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0043f460(const int *, u32);
extern u32 CM_CDECL cm_fn_00441c10(u32, u32, u32, u32, u32);

u32 CM_THISCALL __attribute__((naked))
cm_fn_00450aa0(const int *self __attribute__((unused)),
                u32 arg1 __attribute__((unused)),
                u32 arg2 __attribute__((unused)),
                u32 arg3 __attribute__((unused)))
{
    __asm__(
        "v_00450aa0:\n"
        "movl 12(%esp), %eax\n"
        "v_00450aa4:\n"
        "pushl %eax\n"
        "v_00450aa5:\n"
        "pushl %eax\n"
        CM_ORIGINAL_CALL(00450aa6, 00450aab, 0043f460)
        "movl 8(%esp), %ecx\n"
        "v_00450aaf:\n"
        "pushl %edx\n"
        "v_00450ab0:\n"
        "pushl %eax\n"
        "v_00450ab1:\n"
        "movl 20(%esp), %eax\n"
        "v_00450ab5:\n"
        "pushl %eax\n"
        "v_00450ab6:\n"
        "pushl %ecx\n"
        CM_ORIGINAL_CALL(00450ab7, 00450abc, 00441c10)
        "addl $20, %esp\n"
        "v_00450abf:\n"
        "ret $12\n");
}
