/* Primary 00945610 compiler helper, 47 bytes/17 instructions.
 * Input EAX is unsigned allocation size. Preserve ECX and other registers,
 * read one DWORD after each 4096-byte decrement, then read the exact tail.
 * Move ESP beneath the allocation and transfer the original return address.
 * EAX returns that address; flags retain the last TEST result.
 * Original Windows guard-page/SEH handling is external to this body.
 */
    .text
    .globl _cm_stack_probe
_cm_stack_probe:
    pushl %ecx
    cmpl $0x1000, %eax
    leal 8(%esp), %ecx
    jb .Ltail
.Lpages:
    subl $0x1000, %ecx
    subl $0x1000, %eax
    testl %eax, (%ecx)
    cmpl $0x1000, %eax
    jae .Lpages
.Ltail:
    subl %eax, %ecx
    movl %esp, %eax
    testl %eax, (%ecx)
    movl %ecx, %esp
    movl (%eax), %ecx
    movl 4(%eax), %eax
    pushl %eax
    ret
