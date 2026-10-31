/* Five-slot reader at preferred-image VA 007e5570, image base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Record fields remain opaque. Counter/source/destination/stack may alias.
   Original getter/date/copy/diagnostic routines remain explicit boundaries.
   EDX:EAX is a validation adapter; no semantic 64-bit return is inferred. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(void *, void *, void *);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void *, void *, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

#define READER_FIVE_STATE "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(destination), "+S"(counter), "+D"(source), "+r"(retained)
#define READER_FIVE_STEP(text) __asm__ volatile(text : READER_FIVE_STATE : : "memory", "cc")

/* Tail-entered with the original 0x210 frame and EBX/ESI/EDI saves intact.
   Register-only C assignments must emit no spills or helper calls. The getter
   reads a signed low-byte index; the caller still pushes the full partial EAX. */
void __attribute__((naked, noinline, used, optimize("O2"))) cm_read_007e5570(void)
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    register void *destination __asm__("ebx"), *counter __asm__("esi"), *source __asm__("edi");
    register u32 retained __asm__("ebp");
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx), "=b"(destination),
                         "=S"(counter), "=D"(source), "=r"(retained));
    READER_FIVE_STEP("v_007e5629:\n\tmovl 0x224(%%esp), %%esi\n v_007e5630:\n\tpushl %%ebp\n"
         "v_007e5631:\n\tmovl %%edi, %%ecx\n"
         "v_007e5633:\n\tmovb (%%esi), %%al\n v_007e5635:\n\tmovb %%al, %%dl\n"
         "v_007e5637:\n\tpushl %%eax\n v_007e5638:\n\tincb %%dl\n"
         "v_007e563a:\n\tmovb %%dl, (%%esi)\n"
         CM_ORIGINAL_CALL(007e563c, 007e5641, 00785830));

    /* This unaligned dword store precedes the next counter read. Pin it to
       EAX/EBX: GCC's generic C store spills live state into original saves. */
    READER_FIVE_STEP("movl %%eax, (%%ebx)");
    READER_FIVE_STEP("v_007e5643:\n\tmovb (%%esi), %%al\n v_007e5645:\n\tmovb %%al, %%cl\n"
         "v_007e5647:\n\tpushl %%eax\n v_007e5648:\n\tincb %%cl\n"
         "v_007e564a:\n\tmovb %%cl, (%%esi)\n v_007e564c:\n\tmovl %%edi, %%ecx\n"
         CM_ORIGINAL_CALL(007e564e, 007e5653, 00785830));

    /* Retain the first pair's getter result across the third getter. */
    retained = eax;
    READER_FIVE_STEP("");
    READER_FIVE_STEP("v_007e5655:\n\tmovb (%%esi), %%al\n v_007e5657:\n\tmovb %%al, %%dl\n"
         "v_007e5659:\n\tpushl %%eax\n v_007e565a:\n\tincb %%dl\n"
         "v_007e565c:\n\tmovl %%edi, %%ecx\n v_007e565e:\n\tmovb %%dl, (%%esi)\n"
         CM_ORIGINAL_CALL(007e5660, 007e5665, 00785830));

    /* First date self is entryESP-208h. Copy's other window is entryESP-210h.
       The two windows are eight bytes each; neither is initialized here. */
    READER_FIVE_STEP("pushl %%eax\n v_007e5666:\n\tpushl %%ebp\n"
         "v_007e5667:\n\tleal 0x20(%%esp), %%ecx\n"
         CM_ORIGINAL_CALL(007e566b, 007e5670, 00549ef0)
         "pushl %%eax\n v_007e5671:\n\tleal 0x14(%%esp), %%eax\n"
         "v_007e5675:\n\tpushl %%eax\n v_007e5676:\n\tleal 4(%%ebx), %%ecx\n"
         CM_ORIGINAL_CALL(007e5679, 007e567e, 00417c10));

    READER_FIVE_STEP("movb (%%esi), %%al\n v_007e5680:\n\tmovb %%al, %%cl\n"
         "v_007e5682:\n\tpushl %%eax\n v_007e5683:\n\tincb %%cl\n"
         "v_007e5685:\n\tmovb %%cl, (%%esi)\n v_007e5687:\n\tmovl %%edi, %%ecx\n"
         CM_ORIGINAL_CALL(007e5689, 007e568e, 00785830));
    retained = eax;
    READER_FIVE_STEP("");
    READER_FIVE_STEP("v_007e5690:\n\tmovb (%%esi), %%al\n v_007e5692:\n\tmovb %%al, %%dl\n"
         "v_007e5694:\n\tpushl %%eax\n v_007e5695:\n\tincb %%dl\n"
         "v_007e5697:\n\tmovl %%edi, %%ecx\n v_007e5699:\n\tmovb %%dl, (%%esi)\n"
         CM_ORIGINAL_CALL(007e569b, 007e56a0, 00785830));

    /* Second pair swaps window roles: date at entryESP-210h, copy's other
       window at entryESP-208h. Keep LEA after the first argument PUSH. */
    READER_FIVE_STEP("pushl %%eax\n v_007e56a1:\n\tpushl %%ebp\n"
         "v_007e56a2:\n\tleal 0x18(%%esp), %%ecx\n"
         CM_ORIGINAL_CALL(007e56a6, 007e56ab, 00549ef0)
         "pushl %%eax\n v_007e56ac:\n\tleal 0x1c(%%esp), %%eax\n"
         "v_007e56b0:\n\tpushl %%eax\n v_007e56b1:\n\tleal 0xc(%%ebx), %%ecx\n"
         CM_ORIGINAL_CALL(007e56b4, 007e56b9, 00417c10)
         "popl %%ebp\n v_007e56ba:\n\tpopl %%edi\n v_007e56bb:\n\tpopl %%esi\n"
         "v_007e56bc:\n\tmovl $1, %%eax\n v_007e56c1:\n\tpopl %%ebx\n"
         "v_007e56c2:\n\taddl $0x210, %%esp\n v_007e56c8:\n\tret");
    __builtin_unreachable();
}

/* Original frame/null-diagnostic ABI. Buffers are entryESP-200h/-100h; the
   extra sixteen frame bytes hold the disjoint uninitialized temporaries.
   Buffer argument orders differ on the two diagnostic paths. */
unsigned long long CM_CDECL __attribute__((naked, noinline, used))
cm_fn_007e5570(void *source __attribute__((unused)),
               void *counter __attribute__((unused)),
               void *destination __attribute__((unused)))
{
    __asm__ volatile(
        ".intel_syntax noprefix\n v_007e5570:\n sub esp, 0x210\n"
        "v_007e5576:\n push ebx\n v_007e5577:\n push esi\n v_007e5578:\n push edi\n"
        "v_007e5579:\n mov edi, dword ptr [esp+0x220]\n v_007e5580:\n xor esi, esi\n"
        "v_007e5582:\n cmp edi, esi\n v_007e5584:\n jne v_007e55b6\n"
        "v_007e5586:\n lea eax, [esp+0x1c]\n v_007e558a:\n lea ecx, [esp+0x11c]\n"
        "v_007e5591:\n push eax\n v_007e5592:\n push ecx\n v_007e5593:\n push esi\n"
        "v_007e5594:\n push esi\n v_007e5595:\n push 0xa7a088\n"
        CM_ORIGINAL_CALL(007e559a, 007e559f, 00944cff)
        "add esp, 0x14\n v_007e55a2:\n lea edx, [esp+0x1c]\n"
        "v_007e55a6:\n lea eax, [esp+0x11c]\n v_007e55ad:\n push 0x234\n"
        "v_007e55b2:\n push edx\n v_007e55b3:\n push eax\n v_007e55b4:\n jmp v_007e55ef\n"
        "v_007e55b6:\n mov ebx, dword ptr [esp+0x228]\n v_007e55bd:\n cmp ebx, esi\n"
        "v_007e55bf:\n jne 1f\n v_007e55c1:\n lea ecx, [esp+0x11c]\n"
        "v_007e55c8:\n lea edx, [esp+0x1c]\n v_007e55cc:\n push ecx\n"
        "v_007e55cd:\n push edx\n v_007e55ce:\n push esi\n v_007e55cf:\n push esi\n"
        "v_007e55d0:\n push 0xa7a088\n"
        CM_ORIGINAL_CALL(007e55d5, 007e55da, 00944cff)
        "add esp, 0x14\n v_007e55dd:\n lea eax, [esp+0x11c]\n"
        "v_007e55e4:\n lea ecx, [esp+0x1c]\n v_007e55e8:\n push 0x23c\n"
        "v_007e55ed:\n push eax\n v_007e55ee:\n push ecx\n"
        CM_ORIGINAL_CALL(007e55ef, 007e55f4, 0090d130)
        "push eax\n v_007e55f5:\n push 0x9870e8\n v_007e55fa:\n push 0xae24d0\n"
        CM_ORIGINAL_CALL(007e55ff, 007e5604, 00944c9f)
        "push esi\n v_007e5605:\n push 0xae24d0\n v_007e560a:\n push 0x9870e0\n"
        CM_ORIGINAL_CALL(007e560f, 007e5614, 005e8290)
        "add esp, 0x24\n v_007e5617:\n mov dword ptr [0xb67a34], esi\n"
        "v_007e561d:\n xor eax, eax\n v_007e561f:\n pop edi\n v_007e5620:\n pop esi\n"
        "v_007e5621:\n pop ebx\n v_007e5622:\n add esp, 0x210\n v_007e5628:\n ret\n"
        "1:\n jmp _cm_read_007e5570\n .att_syntax prefix\n");
}
