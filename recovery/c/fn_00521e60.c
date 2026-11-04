/* Preferred-image VA 0x00521e60, image base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Seven cdecl dwords. With S = entry ESP, the original helper receives an
   eight-byte scratch object at S-8, followed by arg3, arg4, arg5+arg7 and
   arg6. Its original RET16 restores ESP to S-12. The wrapper rereads arg2
   as a signed word and arg1 as a dword after the call, then computes
   arg1 + sign_extend16(arg2)*104 modulo 2^32. It reads both scratch fields
   before restoring EBX, stores the word first and the adjusted byte second,
   and returns the full destination address in EAX.

   Preserve the exact scratch, saved-register and helper-descended stack
   locations: a conventional C frame would introduce unproved aliases.
   This basic-assembly adapter retains those obligations and the original
   numeric CALL return word. It remains a VM continuation boundary and
   does not establish a native binding or deepen the arithmetic into C. */
#include "cmre.h"

extern u8 *CM_THISCALL cm_fn_00549f70(u8 *self, u32 first, u32 second,
                                    u32 third, u32 fourth);

u32 CM_CDECL __attribute__((naked))
cm_fn_00521e60(u32 arg1 __attribute__((unused)),
              u32 arg2 __attribute__((unused)),
              u32 arg3 __attribute__((unused)),
              u32 arg4 __attribute__((unused)),
              u32 arg5 __attribute__((unused)),
              u32 arg6 __attribute__((unused)),
              u32 arg7 __attribute__((unused)))
{
    __asm__(
        "v_00521e60:\n"
        "movl 20(%esp), %ecx\n"
        "v_00521e64:\n"
        "movl 24(%esp), %eax\n"
        "subl $8, %esp\n"
        "pushl %ebx\n"
        "v_00521e6c:\n"
        "movl 40(%esp), %ebx\n"
        "pushl %eax\n"
        "v_00521e71:\n"
        "movl 32(%esp), %eax\n"
        "leal (%ebx,%ecx), %edx\n"
        "v_00521e78:\n"
        "movl 28(%esp), %ecx\n"
        "pushl %edx\n"
        "pushl %eax\n"
        "pushl %ecx\n"
        "leal 20(%esp), %ecx\n"
        CM_ORIGINAL_CALL(00521e83, 00521e88, 00549f70)
        "movswl 20(%esp), %eax\n"
        "v_00521e8d:\n"
        "movl 16(%esp), %ecx\n"
        "leal (%eax,%eax,2), %edx\n"
        "leal (%eax,%edx,4), %eax\n"
        "v_00521e97:\n"
        "movw 4(%esp), %dx\n"
        "leal (%ecx,%eax,8), %eax\n"
        "v_00521e9f:\n"
        "movb 6(%esp), %cl\n"
        "subb %bl, %cl\n"
        "popl %ebx\n"
        "v_00521ea6:\n"
        "movw %dx, (%eax)\n"
        "v_00521ea9:\n"
        "movb %cl, 2(%eax)\n"
        "addl $8, %esp\n"
        "ret\n"
    );
}
