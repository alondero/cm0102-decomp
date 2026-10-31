/* Primary9519D3 bounded length; always read the limit byte, including limit0. */
    .intel_syntax noprefix
    .text
    .globl _cm_crt_length
_cm_crt_length:
cm_crt_length_i_009519d3:
    mov edx, dword ptr [esp + 8]
cm_crt_length_i_009519d7:
    mov eax, dword ptr [esp + 4]
cm_crt_length_i_009519db:
    test edx, edx
cm_crt_length_i_009519dd:
    push esi
cm_crt_length_i_009519de:
    lea ecx, [edx - 1]
cm_crt_length_i_009519e1:
    je cm_crt_length_i_009519f0
cm_crt_length_i_009519e3:
    cmp byte ptr [eax], 0
cm_crt_length_i_009519e6:
    je cm_crt_length_i_009519f0
cm_crt_length_i_009519e8:
    inc eax
cm_crt_length_i_009519e9:
    mov esi, ecx
cm_crt_length_i_009519eb:
    dec ecx
cm_crt_length_i_009519ec:
    test esi, esi
cm_crt_length_i_009519ee:
    jne cm_crt_length_i_009519e3
cm_crt_length_i_009519f0:
    cmp byte ptr [eax], 0
cm_crt_length_i_009519f3:
    pop esi
cm_crt_length_i_009519f4:
    jne cm_crt_length_i_009519fb
cm_crt_length_i_009519f6:
    sub eax, dword ptr [esp + 4]
cm_crt_length_i_009519fa:
    ret
cm_crt_length_i_009519fb:
    mov eax, edx
cm_crt_length_i_009519fd:
    ret
