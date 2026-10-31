/* Primary 00945A60 substring and 00946540 character search.
 * Full 321-byte closure includes shared 00946530 exit and 00946546 AL entry.
 * Retain aligned DWORD reads, unsigned detection arithmetic, flags and faults.
 * Native assembler labels preserve instruction provenance; no interpreter. */
    .intel_syntax noprefix
    .text
    .globl _cm_crt_strstr
    .globl _cm_crt_strchr
_cm_crt_strstr:
cm_crt_i_00945a60:
    mov ecx, dword ptr [esp + 8]
cm_crt_i_00945a64:
    push edi
cm_crt_i_00945a65:
    push ebx
cm_crt_i_00945a66:
    push esi
cm_crt_i_00945a67:
    mov dl, byte ptr [ecx]
cm_crt_i_00945a69:
    mov edi, dword ptr [esp + 0x10]
cm_crt_i_00945a6d:
    test dl, dl
cm_crt_i_00945a6f:
    je cm_crt_i_00945ada
cm_crt_i_00945a71:
    mov dh, byte ptr [ecx + 1]
cm_crt_i_00945a74:
    test dh, dh
cm_crt_i_00945a76:
    je cm_crt_i_00945ac7
cm_crt_i_00945a78:
    mov esi, edi
cm_crt_i_00945a7a:
    mov ecx, dword ptr [esp + 0x14]
cm_crt_i_00945a7e:
    mov al, byte ptr [edi]
cm_crt_i_00945a80:
    inc esi
cm_crt_i_00945a81:
    cmp al, dl
cm_crt_i_00945a83:
    je cm_crt_i_00945a9a
cm_crt_i_00945a85:
    test al, al
cm_crt_i_00945a87:
    je cm_crt_i_00945a94
cm_crt_i_00945a89:
    mov al, byte ptr [esi]
cm_crt_i_00945a8b:
    inc esi
cm_crt_i_00945a8c:
    cmp al, dl
cm_crt_i_00945a8e:
    je cm_crt_i_00945a9a
cm_crt_i_00945a90:
    test al, al
cm_crt_i_00945a92:
    jne cm_crt_i_00945a89
cm_crt_i_00945a94:
    pop esi
cm_crt_i_00945a95:
    pop ebx
cm_crt_i_00945a96:
    pop edi
cm_crt_i_00945a97:
    xor eax, eax
cm_crt_i_00945a99:
    ret
cm_crt_i_00945a9a:
    mov al, byte ptr [esi]
cm_crt_i_00945a9c:
    inc esi
cm_crt_i_00945a9d:
    cmp al, dh
cm_crt_i_00945a9f:
    jne cm_crt_i_00945a8c
cm_crt_i_00945aa1:
    lea edi, [esi - 1]
cm_crt_i_00945aa4:
    mov ah, byte ptr [ecx + 2]
cm_crt_i_00945aa7:
    test ah, ah
cm_crt_i_00945aa9:
    je cm_crt_i_00945ad3
cm_crt_i_00945aab:
    mov al, byte ptr [esi]
cm_crt_i_00945aad:
    add esi, 2
cm_crt_i_00945ab0:
    cmp al, ah
cm_crt_i_00945ab2:
    jne cm_crt_i_00945a78
cm_crt_i_00945ab4:
    mov al, byte ptr [ecx + 3]
cm_crt_i_00945ab7:
    test al, al
cm_crt_i_00945ab9:
    je cm_crt_i_00945ad3
cm_crt_i_00945abb:
    mov ah, byte ptr [esi - 1]
cm_crt_i_00945abe:
    add ecx, 2
cm_crt_i_00945ac1:
    cmp al, ah
cm_crt_i_00945ac3:
    je cm_crt_i_00945aa4
cm_crt_i_00945ac5:
    jmp cm_crt_i_00945a78
cm_crt_i_00945ac7:
    xor eax, eax
cm_crt_i_00945ac9:
    pop esi
cm_crt_i_00945aca:
    pop ebx
cm_crt_i_00945acb:
    pop edi
cm_crt_i_00945acc:
    mov al, dl
cm_crt_i_00945ace:
    jmp cm_crt_i_00946546
cm_crt_i_00945ad3:
    lea eax, [edi - 1]
cm_crt_i_00945ad6:
    pop esi
cm_crt_i_00945ad7:
    pop ebx
cm_crt_i_00945ad8:
    pop edi
cm_crt_i_00945ad9:
    ret
cm_crt_i_00945ada:
    mov eax, edi
cm_crt_i_00945adc:
    pop esi
cm_crt_i_00945add:
    pop ebx
cm_crt_i_00945ade:
    pop edi
cm_crt_i_00945adf:
    ret
cm_crt_i_00946530:
    lea eax, [edx - 1]
cm_crt_i_00946533:
    pop ebx
cm_crt_i_00946534:
    ret
_cm_crt_strchr:
cm_crt_i_00946540:
    xor eax, eax
cm_crt_i_00946542:
    mov al, byte ptr [esp + 8]
cm_crt_i_00946546:
    push ebx
cm_crt_i_00946547:
    mov ebx, eax
cm_crt_i_00946549:
    shl eax, 8
cm_crt_i_0094654c:
    mov edx, dword ptr [esp + 8]
cm_crt_i_00946550:
    test edx, 3
cm_crt_i_00946556:
    je cm_crt_i_0094656b
cm_crt_i_00946558:
    mov cl, byte ptr [edx]
cm_crt_i_0094655a:
    inc edx
cm_crt_i_0094655b:
    cmp cl, bl
cm_crt_i_0094655d:
    je cm_crt_i_00946530
cm_crt_i_0094655f:
    test cl, cl
cm_crt_i_00946561:
    je cm_crt_i_009465b4
cm_crt_i_00946563:
    test edx, 3
cm_crt_i_00946569:
    jne cm_crt_i_00946558
cm_crt_i_0094656b:
    or ebx, eax
cm_crt_i_0094656d:
    push edi
cm_crt_i_0094656e:
    mov eax, ebx
cm_crt_i_00946570:
    shl ebx, 0x10
cm_crt_i_00946573:
    push esi
cm_crt_i_00946574:
    or ebx, eax
cm_crt_i_00946576:
    mov ecx, dword ptr [edx]
cm_crt_i_00946578:
    mov edi, 0x7efefeff
cm_crt_i_0094657d:
    mov eax, ecx
cm_crt_i_0094657f:
    mov esi, edi
cm_crt_i_00946581:
    xor ecx, ebx
cm_crt_i_00946583:
    add esi, eax
cm_crt_i_00946585:
    add edi, ecx
cm_crt_i_00946587:
    xor ecx, 0xffffffff
cm_crt_i_0094658a:
    xor eax, 0xffffffff
cm_crt_i_0094658d:
    xor ecx, edi
cm_crt_i_0094658f:
    xor eax, esi
cm_crt_i_00946591:
    add edx, 4
cm_crt_i_00946594:
    and ecx, 0x81010100
cm_crt_i_0094659a:
    jne cm_crt_i_009465b8
cm_crt_i_0094659c:
    and eax, 0x81010100
cm_crt_i_009465a1:
    je cm_crt_i_00946576
cm_crt_i_009465a3:
    and eax, 0x1010100
cm_crt_i_009465a8:
    jne cm_crt_i_009465b2
cm_crt_i_009465aa:
    and esi, 0x80000000
cm_crt_i_009465b0:
    jne cm_crt_i_00946576
cm_crt_i_009465b2:
    pop esi
cm_crt_i_009465b3:
    pop edi
cm_crt_i_009465b4:
    pop ebx
cm_crt_i_009465b5:
    xor eax, eax
cm_crt_i_009465b7:
    ret
cm_crt_i_009465b8:
    mov eax, dword ptr [edx - 4]
cm_crt_i_009465bb:
    cmp al, bl
cm_crt_i_009465bd:
    je cm_crt_i_009465f5
cm_crt_i_009465bf:
    test al, al
cm_crt_i_009465c1:
    je cm_crt_i_009465b2
cm_crt_i_009465c3:
    cmp ah, bl
cm_crt_i_009465c5:
    je cm_crt_i_009465ee
cm_crt_i_009465c7:
    test ah, ah
cm_crt_i_009465c9:
    je cm_crt_i_009465b2
cm_crt_i_009465cb:
    shr eax, 0x10
cm_crt_i_009465ce:
    cmp al, bl
cm_crt_i_009465d0:
    je cm_crt_i_009465e7
cm_crt_i_009465d2:
    test al, al
cm_crt_i_009465d4:
    je cm_crt_i_009465b2
cm_crt_i_009465d6:
    cmp ah, bl
cm_crt_i_009465d8:
    je cm_crt_i_009465e0
cm_crt_i_009465da:
    test ah, ah
cm_crt_i_009465dc:
    je cm_crt_i_009465b2
cm_crt_i_009465de:
    jmp cm_crt_i_00946576
cm_crt_i_009465e0:
    pop esi
cm_crt_i_009465e1:
    pop edi
cm_crt_i_009465e2:
    lea eax, [edx - 1]
cm_crt_i_009465e5:
    pop ebx
cm_crt_i_009465e6:
    ret
cm_crt_i_009465e7:
    lea eax, [edx - 2]
cm_crt_i_009465ea:
    pop esi
cm_crt_i_009465eb:
    pop edi
cm_crt_i_009465ec:
    pop ebx
cm_crt_i_009465ed:
    ret
cm_crt_i_009465ee:
    lea eax, [edx - 3]
cm_crt_i_009465f1:
    pop esi
cm_crt_i_009465f2:
    pop edi
cm_crt_i_009465f3:
    pop ebx
cm_crt_i_009465f4:
    ret
cm_crt_i_009465f5:
    lea eax, [edx - 4]
cm_crt_i_009465f8:
    pop esi
cm_crt_i_009465f9:
    pop edi
cm_crt_i_009465fa:
    pop ebx
cm_crt_i_009465fb:
    ret
