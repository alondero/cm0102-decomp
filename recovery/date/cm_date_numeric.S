/* Primary 3.9.68 preferred VAs 0054A470/0054A650. Capture prior stack byte
 * before calling C: original four-digit seed is entry ESP-0x205, two-digit
 * seed is entry ESP-0x209. No synthetic default or C uninitialized read.
 * Reserve original frames; private frame contents otherwise are not parity.
 */
.text
.globl _cm_date_numeric4
_cm_date_numeric4:
    subl $0x208, %esp
    pushl %esi
    movl %ecx, %esi
    movzbl 7(%esp), %eax
    pushl %eax
    pushl 0x21c(%esp)
    pushl 0x21c(%esp)
    pushl 0x21c(%esp)
    pushl %esi
    pushl $0
    call _cm_date_numeric_worker
    addl $24, %esp
    popl %esi
    addl $0x208, %esp
    ret $12

.globl _cm_date_numeric2
_cm_date_numeric2:
    subl $0x20c, %esp
    pushl %esi
    pushl %edi
    movl %ecx, %esi
    movzbl 11(%esp), %eax
    pushl %eax
    pushl 0x224(%esp)
    pushl 0x224(%esp)
    pushl 0x224(%esp)
    pushl %esi
    pushl $1
    call _cm_date_numeric_worker
    addl $24, %esp
    popl %edi
    popl %esi
    addl $0x20c, %esp
    ret $12
