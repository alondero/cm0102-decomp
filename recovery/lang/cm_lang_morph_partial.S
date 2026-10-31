/* Recreate the measured original frame. Unfinished language bodies remain
 * explicit original code; this is an emulator hybrid slice, not integration.
 */
    .text
    .globl _cm_lang_morph_partial
_cm_lang_morph_partial:
    movl $0x2808, %eax
    call 0x00945610
    pushl %ebx
    pushl %ebp
    pushl %esi
    pushl %edi
    pushl %esp
    call _cm_lang_morph_initialize
    addl $4, %esp
    movl 0x281c(%esp), %ebx
    leal 0x1018(%esp), %edx
    movl %ebx, %esi
    addl %eax, %esi
    leal (%edx,%eax), %edi
    xorl %ecx, %ecx
    testl %ebx, %ebx
    jz .Lnull
    movl 0x009eb2dc, %eax
    testl %eax, %eax
    jz .Lzero
    cmpl $7, %eax
    je .Lseven
    cmpl $10, %eax
    je .Lten
    cmpl $8, %eax
    je .Leight
    cmpl $6, %eax
    je .Lsix
    cmpl $2, %eax
    je .Ltwo
    cmpl $4, %eax
    je .Lfour
    cmpl $3, %eax
    je .Lthree
    /* No duplicate initialization or global read. Keep every register and
     * the exact original ESP when entering the original comparison ladder. */
    cmpl $9, %eax
    je .Lnine
    jmp 0x0067c2e5
.Lzero:
    pushl %esp
    call _cm_lang_morph_language_zero
    addl $4, %esp
    jmp .Lreturn
.Lseven:
    pushl %esp
    call _cm_lang_morph_language_seven
    addl $4, %esp
    jmp .Lreturn
.Lten:
    pushl %esp
    call _cm_lang_morph_language_ten
    addl $4, %esp
    jmp .Lreturn
.Leight:
    pushl %esp
    call _cm_lang_morph_language_eight
    addl $4, %esp
    jmp .Lreturn
.Lsix:
    pushl %esp
    call _cm_lang_morph_language_six
    addl $4, %esp
    jmp .Lreturn
.Ltwo:
    pushl %esp
    call _cm_lang_morph_language_two
    addl $4, %esp
    jmp .Lreturn
.Lfour:
    pushl %esp
    call _cm_lang_morph_language_four
    addl $4, %esp
    jmp .Lreturn
.Lthree:
    pushl %esp
    call _cm_lang_morph_language_three
    addl $4, %esp
    jmp .Lreturn
.Lnine:
    pushl %esp
    call _cm_lang_morph_language_nine
    addl $4, %esp
    jmp .Lreturn
.Lnull:
    movl $0x00ad9c64, %eax
.Lreturn:
    popl %edi
    popl %esi
    popl %ebp
    popl %ebx
    addl $0x2808, %esp
    ret
