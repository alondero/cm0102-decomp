/* Preserve EBP-relative caller slots and initial saved ECX local. */
    .text
    .globl _cm_crt_classify
_cm_crt_classify:
    pushl %ebp
    movl %esp, %ebp
    pushl %ecx
    pushl %ebp
    call _cm_crt_classify_frame
    addl $4, %esp
    leave
    ret
