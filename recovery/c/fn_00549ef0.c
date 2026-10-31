/* Preferred-image VA 0x00549ef0, image base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Object field meanings and original source return type remain unknown. */
#include "cmre.h"

/* Entered by a tail jump with a zero-extended 16-bit multiple of four in EAX.
   ESI carries self; ESP still names the original saved ESI. No scratch stack
   is permitted: object storage can alias bytes below the entry ESP.
   If value=100*quotient, its remainder on division by 400 is
   100*(quotient mod 4). Both original signed divisions have nonnegative inputs.
   These attributes and the terminal register constraints form a compiler ABI
   adapter; emitted O0/O2 code must be inspected for register/stack effects. */
static void __attribute__((used, noinline, naked, regparm(1), optimize("O2")))
date_arithmetic(u32 value)
{
    /* Keep every non-scratch register live; a naked function has no prologue.
       Terminal constraints retain their entry values without stack saves. */
    register u32 saved_bx __asm__("ebx");
    register u32 saved_si __asm__("esi");
    register u32 saved_di __asm__("edi");
    register u32 saved_bp __asm__("ebp");
    __asm__ volatile ("" : "=b" (saved_bx), "=S" (saved_si),
                      "=D" (saved_di), "=r" (saved_bp));
    u32 quotient = value / 100u;
    /* Commit the quotient to EAX before subtraction. Without this boundary,
       GCC retains a division intermediate by spilling over saved ESI. */
    __asm__ volatile ("" : "+a" (quotient));
    u32 remainder = value - quotient * 100u;
    if (remainder != 0) {
        __asm__ volatile ("jmp _date_finish" : : "a" (1u), "d" (remainder),
                          "b" (saved_bx), "S" (saved_si),
                          "D" (saved_di), "r" (saved_bp) : "memory");
        __builtin_unreachable();
    }
    remainder = (quotient & 3u) * 100u;
    __asm__ volatile ("jmp _date_finish" : : "a" (remainder == 0), "d" (remainder),
                      "b" (saved_bx), "S" (saved_si),
                      "D" (saved_di), "r" (saved_bp) : "memory");
    __builtin_unreachable();
}

/* Initial word stores retain partial EAX, saved ESI, untouched EDX and order
   at their fault boundaries. Only the original PUSH ESI and conditional
   PUSH EDI write stack bytes. The terminal store restores the original ESP
   and callee-saved registers first. EDX:EAX is an adapter for observable
   register values, not a claim about the original semantic return type. */
unsigned long long CM_THISCALL __attribute__((naked))
cm_fn_00549ef0(void *self __attribute__((unused)),
              u32 first __attribute__((unused)),
              u32 second __attribute__((unused)))
{
    __asm__(
        "v_00549ef0:\n"
        "movw 4(%esp), %ax\n"
        "v_00549ef5:\n"
        "pushl %esi\n"
        "v_00549ef6:\n"
        "movl 12(%esp), %esi\n"
        "v_00549efa:\n"
        "movw %ax, (%ecx)\n"
        "v_00549efd:\n"
        "movw %si, 2(%ecx)\n"
        "andl $65535, %esi\n"
        "movl %esi, %edx\n"
        "andl $1, %edx\n"
        "cmpl $1, %edx\n"
        "je 1f\n"
        "testl $3, %esi\n"
        "jne 1f\n"
        "movl %esi, %eax\n"
        "v_00549f2d:\n"
        "pushl %edi\n"
        "v_00549f36:\n"
        "popl %edi\n"
        "movl %ecx, %esi\n"
        "jmp _date_arithmetic\n"
        "_date_finish:\n"
        "movl %esi, %ecx\n"
        "popl %esi\n"
        "testl %eax, %eax\n"
        "jz 2f\n"
        "testl %edx, %edx\n"
        "v_00549f4f:\n"
        "movl %eax, 4(%ecx)\n"
        "movl %ecx, %eax\n"
        "ret $8\n"
        "1:\n"
        "xorl %eax, %eax\n"
        "popl %esi\n"
        "2:\n"
        "xorl %eax, %eax\n"
        "v_00549f5a:\n"
        "movl %eax, 4(%ecx)\n"
        "movl %ecx, %eax\n"
        "ret $8\n"
    );
}