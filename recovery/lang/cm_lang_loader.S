/* Original incoming-ESP slots consumed by unchecked header/ID fread calls.
 * Capture defined machine bits before C changes the private frame.
 */
    .text
    .globl _cm_lang_load
_cm_lang_load:
    movl -0x4ec(%esp), %eax
    movl -0x4e0(%esp), %edx
    pushl %edx
    pushl %eax
    pushl 16(%esp)
    pushl 16(%esp)
    call _cm_lang_load_worker
    addl $16, %esp
    ret

    .globl _cm_lang_byte_order
_cm_lang_byte_order:
    leal 4(%esp), %eax
    pushl %eax
    call _cm_lang_byte_order_worker
    addl $4, %esp
    ret
