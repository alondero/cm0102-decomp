/* Preferred-image VA 0x0094ab27, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve the original stack frame, saved-register writes, call order and fault
   state. The two CALL bridges retain original numeric return words; continuation
   dispatch uses the owned VM binding, not a standalone native implementation.
   GCC basic assembly in a naked entry; inspect emission at both build modes.
   The scanner may be composed; the copy helper remains an original boundary. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_009488e0(u32 source);
extern unsigned long long CM_CDECL cm_fn_0094f0f0(u32 destination, u32 source, u32 size);
unsigned long long CM_CDECL __attribute__((naked, regparm(2)))
cm_fn_0094ab27(u32 eax __attribute__((unused)), u32 edx __attribute__((unused)),
               u32 source __attribute__((unused)), u32 offset __attribute__((unused)))
{
    __asm__(
        "v_0094ab27: pushl %edi\n"
        "v_0094ab28: movl 12(%esp), %edi\n"
        "v_0094ab2c: testl %edi, %edi\n"
        "v_0094ab2e: jz v_0094ab4a\n"
        "v_0094ab30: pushl %esi\n"
        "v_0094ab31: movl 12(%esp), %esi\n"
        "v_0094ab35: pushl %esi\n"
        CM_ORIGINAL_CALL(0094ab36, 0094ab3b, 009488e0)
        "incl %eax\n"
        "v_0094ab3c: pushl %eax\n"
        "v_0094ab3d: pushl %esi\n"
        "v_0094ab3e: addl %edi, %esi\n"
        "v_0094ab40: pushl %esi\n"
        CM_ORIGINAL_CALL(0094ab41, 0094ab46, 0094f0f0)
        "addl $16, %esp\n"
        "v_0094ab49: popl %esi\n"
        "v_0094ab4a: popl %edi\n"
        "v_0094ab4b: ret\n"
    );
}
