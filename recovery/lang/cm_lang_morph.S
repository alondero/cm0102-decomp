/* Recreate the measured original frame. Application body is native source;
 * original stack probe, immutable data and explicit CRT boundaries remain. */
    .text
    .globl _cm_lang_morph
_cm_lang_morph:
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
    cmpl $9, %eax
    je .Lnine
    cmpl $5, %eax
    je .Lfive
    pushl %esp
    call _cm_lang_morph_default
    addl $4, %esp
    jmp .Lreturn
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
.Lfive:
    pushl %esp
    call _cm_lang_morph_language_five
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
