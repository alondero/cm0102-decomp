/* Readable record recovery at preferred-image VA 007e4750, base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Fields are opaque. The assembly adapter retains original SEH registration,
   uninitialized diagnostic buffers, date temporaries and partial-register ABI.
   All seven original callees, and handler 009606db, remain external boundaries. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(void *, void *, void *);
extern unsigned long long CM_CDECL cm_fn_004afa80(void *, void *);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void *, void *, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Memory primitives preserve original widths, partial registers and precise
   fault provenance. Record selection/control flow is C. A tail transfer keeps
   the original stack slots visible to aliases and to original SEH handling. */
#define LOAD_EAX(va, offset, base, constraint) \
    __asm__ volatile("v_" #va ":\n\tmovl " #offset "(%1), %0" \
                     : "=a"(eax) : constraint(base) : "memory")
#define LOAD_ECX(va, offset) \
    __asm__ volatile("v_" #va ":\n\tmovl " #offset "(%1), %0" \
                     : "=c"(ecx) : "S"(first) : "memory")
#define LOAD_EDX(va, offset) \
    __asm__ volatile("v_" #va ":\n\tmovl " #offset "(%1), %0" \
                     : "=d"(edx) : "S"(first) : "memory")
#define LOAD_AL(va, offset) \
    __asm__ volatile("v_" #va ":\n\tmovb " #offset "(%1), %b0" \
                     : "+a"(eax) : "S"(first) : "memory")
#define LOAD_CL(va, offset) \
    __asm__ volatile("v_" #va ":\n\tmovb " #offset "(%1), %b0" \
                     : "+c"(ecx) : "S"(first) : "memory")
#define LOAD_DL(va, offset) \
    __asm__ volatile("v_" #va ":\n\tmovb " #offset "(%1), %b0" \
                     : "+d"(edx) : "S"(first) : "memory")
#define STORE_DWORD(va, offset, value, constraint) \
    __asm__ volatile("v_" #va ":\n\tmovl %0, " #offset "(%1)" \
                     : : constraint(value), "D"(third) : "memory")
#define STORE_BYTE(va, offset, value, constraint) \
    __asm__ volatile("v_" #va ":\n\tmovb %b0, " #offset "(%1)" \
                     : : constraint(value), "D"(third) : "memory")
/* Expose the original signed comparison as a C condition without changing
   its flags: exception contexts may observe flags even though ABI tests omit them. */
#define SIGNED_BYTE_ABOVE_MINUS_ONE(va, positive) \
    __asm__ volatile("v_" #va ":\n\tcmpb $-1, %b1" \
                     : "=@ccg"(positive) : "a"(eax) : "cc")
#define KEEP_REGISTER_STATE() \
    __asm__ volatile("" : "+a"(eax), "+c"(ecx), "+d"(edx), \
                         "+S"(first), "+b"(second), "+D"(third))

void __attribute__((naked, noinline, used, optimize("O2")))
cm_record_suffix_007e4750(void)
{
    register void *first __asm__("esi");
    register void *second __asm__("ebx");
    register void *third __asm__("edi");
    register u32 eax __asm__("eax");
    register u32 ecx __asm__("ecx");
    register u32 edx __asm__("edx");
    /* Inputs arrive from the adapter; this emits no initialization or writes. */
    __asm__ volatile("" : "=a"(eax), "=c"(ecx), "=d"(edx),
                         "=S"(first), "=b"(second), "=D"(third));
    STORE_BYTE(007e489a, 0x10, eax, "a");
    LOAD_EAX(007e489d, 0, second, "b");
    STORE_DWORD(007e489f, 0x11, eax, "a");
    LOAD_ECX(007e48a2, 8);
    STORE_DWORD(007e48a5, 0x1b, ecx, "c");
    LOAD_EAX(007e48a8, 0x1c, first, "S");
    __asm__ volatile("addl $8, %%esp" : : : "memory");
    if ((u32)second == eax) {
        LOAD_EDX(007e48b2, 0x10);
        STORE_DWORD(007e48b5, 0x15, edx, "d");
        LOAD_AL(007e48b8, 0x47);
        u8 positive;
        SIGNED_BYTE_ABOVE_MINUS_ONE(007e48bb, positive);
        if (positive) {
            STORE_BYTE(007e48bf, 0x19, eax, "a");
            LOAD_AL(007e48c2, 0x48);
            goto store_last_al;
        }
        LOAD_CL(007e48c7, 0x43);
        STORE_BYTE(007e48ca, 0x19, ecx, "c");
        LOAD_DL(007e48cd, 0x44);
        STORE_BYTE(007e48d0, 0x1a, edx, "d");
    } else {
        LOAD_EAX(007e48d5, 0x0c, first, "S");
        STORE_DWORD(007e48d8, 0x15, eax, "a");
        LOAD_AL(007e48db, 0x48);
        u8 positive;
        SIGNED_BYTE_ABOVE_MINUS_ONE(007e48de, positive);
        if (positive) {
            STORE_BYTE(007e48e2, 0x19, eax, "a");
            LOAD_CL(007e48e5, 0x47);
            STORE_BYTE(007e48e8, 0x1a, ecx, "c");
        } else {
            LOAD_DL(007e48ed, 0x44);
            STORE_BYTE(007e48f0, 0x19, edx, "d");
            LOAD_AL(007e48f3, 0x43);
            goto store_last_al;
        }
    }
    goto finish;
store_last_al:
    STORE_BYTE(007e48f6, 0x1a, eax, "a");
finish:
    KEEP_REGISTER_STATE();
    __asm__ volatile("movl $1, %%eax\n\tjmp cm_record_epilogue_007e4750"
                     : : : "memory");
    __builtin_unreachable();
}

/* Naked adapter uses the original frame geometry. Its diagnostics must keep
   three distinct buffer orders and the original callee stack positions.
   Date object = entry ESP-214h; copy temporary = entry ESP-21ch.
   No stack bytes receive invented initialization. */
unsigned long long CM_CDECL __attribute__((naked))
cm_fn_007e4750(void *first __attribute__((unused)),
               void *second __attribute__((unused)),
               void *third __attribute__((unused)))
{
    __asm__ volatile(
        ".intel_syntax noprefix\n"
        "mov eax, dword ptr fs:[0]\n"
        "push -1\n push 0x9606db\n push eax\n"
        "mov dword ptr fs:[0], esp\n sub esp, 0x210\n"
        "push ebx\n push ebp\n push esi\n"
        "mov esi, dword ptr [esp+0x22c]\n xor ebp, ebp\n push edi\n"
        "test esi, esi\n jne 1f\n"
        "lea eax, [esp+0x20]\n lea ecx, [esp+0x120]\n"
        "push eax\n push ecx\n push ebp\n push ebp\n push 0xa7a088\n"
        "call _cm_fn_00944cff\n v_007e4795:\n add esp, 0x14\n"
        "lea edx, [esp+0x20]\n lea eax, [esp+0x120]\n"
        "push 0x29\n push edx\n push eax\n jmp 3f\n"
        "1:\n mov ebx, dword ptr [esp+0x234]\n test ebx, ebx\n jne 2f\n"
        "lea ecx, [esp+0x120]\n lea edx, [esp+0x20]\n"
        "push ecx\n push edx\n push ebp\n push ebp\n push 0xa7a088\n"
        "call _cm_fn_00944cff\n v_007e47cd:\n add esp, 0x14\n"
        "lea eax, [esp+0x120]\n lea ecx, [esp+0x20]\n"
        "push 0x31\n push eax\n push ecx\n jmp 3f\n"
        "2:\n mov edi, dword ptr [esp+0x238]\n test edi, edi\n jne 4f\n"
        "lea edx, [esp+0x120]\n lea eax, [esp+0x20]\n"
        "push edx\n push eax\n push ebp\n push ebp\n push 0xa7a088\n"
        "call _cm_fn_00944cff\n v_007e4805:\n add esp, 0x14\n"
        "lea ecx, [esp+0x120]\n lea edx, [esp+0x20]\n"
        "push 0x39\n push ecx\n push edx\n"
        "3:\n call _cm_fn_0090d130\n v_007e481c:\n"
        "push eax\n push 0x9870e8\n push 0xae24d0\n call _cm_fn_00944c9f\n v_007e482c:\n"
        "push ebp\n push 0xae24d0\n push 0x9870e0\n call _cm_fn_005e8290\n v_007e483c:\n"
        "add esp, 0x24\n mov dword ptr [0xb67a34], ebp\n xor eax, eax\n jmp 5f\n"
        /* Partial words passed to date retain entry upper ECX / previous FS
           upper EAX. Both callee cleanups and their exact ESP are preserved. */
        "4:\n v_007e484c:\n mov ax, word ptr [esi+0x28]\n v_007e4850:\n mov cx, word ptr [esi+0x2a]\n"
        "push eax\n push ecx\n lea ecx, [esp+0x20]\n call _cm_fn_00549ef0\n v_007e485f:\n"
        "lea edx, [esp+0x10]\n push eax\n push edx\n mov ecx, edi\n"
        "mov dword ptr [esp+0x230], ebp\n call _cm_fn_00417c10\n"
        /* These three copies are coupled to partial CX/DX and the precise
           exception state immediately preceding the original external call.
           Keeping them in the ABI adapter avoids dirtying callee stack bytes. */
        "v_007e4873:\n mov eax, dword ptr [esi]\n push esi\n"
        "v_007e4876:\n mov dword ptr [edi+8], eax\n"
        "v_007e4879:\n mov cx, word ptr [esi+0x32]\n"
        "v_007e487d:\n mov word ptr [edi+0xc], cx\n"
        "v_007e4881:\n mov dx, word ptr [esi+0x30]\n push ebx\n"
        "mov dword ptr [esp+0x230], -1\n"
        "v_007e4891:\n mov word ptr [edi+0xe], dx\n"
        /* The extra leading zero denotes the same original VA. This exact
           call-return address and the suffix's faulting store both name 489ah. */
        "call _cm_fn_004afa80\n v_00007e489a:\n"
        "jmp _cm_record_suffix_007e4750\n"
        "5:\n cm_record_epilogue_007e4750:\n"
        "mov ecx, dword ptr [esp+0x220]\n"
        "pop edi\n pop esi\n pop ebp\n pop ebx\n"
        "mov dword ptr fs:[0], ecx\n add esp, 0x21c\n ret\n"
        ".att_syntax prefix\n"
    );
}
