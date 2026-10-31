/* Preferred-image VA007e5780, base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Numeric value and format meanings remain opaque. Outside formatter,
   diagnostics, image data and import scaffolding remain explicit boundaries.
   EDX:EAX is a validation adapter, not a semantic64-bit return claim. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void *,void *,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(void *,const void *,...);

/* Frame/call/partial-register adapters retain original access labels and
   uninitialized512-byte buffers. Threshold routing below is recovered C.
   Magic signed-multiply/divide sequences retain intermediate register/flags
   because subsequent stack reads and outside calls observe those states.
   No helper CALL, compiler stack scratch or buffer initialization is allowed.
   Every direct CALL writes its original numeric return word via owned VM
   near-RET transport. This is not standalone native continuation binding. */
__asm__(
    ".text\n"
    ".intel_syntax noprefix\n"
    "_fmt5780_diagnostic:\n"
    "cmp eax, 12\n"
    "v_007e578f:\n"
    "lea eax, [esp]\n"
    "v_007e5793:\n"
    "lea ecx, [esp + 0x100]\n"
    "v_007e579a:\n"
    "push eax\n"
    "v_007e579b:\n"
    "push ecx\n"
    "v_007e579c:\n"
    "push 0\n"
    "v_007e579e:\n"
    "push 0\n"
    "v_007e57a0:\n"
    "push 0xa7a088\n"
    CM_ORIGINAL_CALL(007e57a5, 007e57aa, 00944cff)
    "add esp, 0x14\n"
    "v_007e57ad:\n"
    "lea edx, [esp]\n"
    "v_007e57b1:\n"
    "lea eax, [esp + 0x100]\n"
    "v_007e57b8:\n"
    "push 0x288\n"
    "v_007e57bd:\n"
    "push edx\n"
    "v_007e57be:\n"
    "push eax\n"
    CM_ORIGINAL_CALL(007e57bf, 007e57c4, 0090d130)
    "push eax\n"
    "v_007e57c5:\n"
    "push 0x9870e8\n"
    "v_007e57ca:\n"
    "push 0xae24d0\n"
    CM_ORIGINAL_CALL(007e57cf, 007e57d4, 00944c9f)
    "push 0\n"
    "v_007e57d6:\n"
    "push 0xae24d0\n"
    "v_007e57db:\n"
    "push 0x9870e0\n"
    CM_ORIGINAL_CALL(007e57e0, 007e57e5, 005e8290)
    "add esp, 0x24\n"
    "v_007e57e8:\n"
    "mov dword ptr [0xb67a34], 0\n"
    "v_007e57f2:\n"
    "xor eax, eax\n"
    "v_007e57f4:\n"
    "add esp, 0x200\n"
    "v_007e57fa:\n"
    "ret\n"
    "_fmt5780_empty_nonzero:\n"
    "cmp ecx, 1\n"
    "v_007e5816:\n"
    "mov ecx, dword ptr [esp + 0x208]\n"
    "v_007e581d:\n"
    "mov eax, 1\n"
    "v_007e5822:\n"
    "mov byte ptr [ecx], 0\n"
    "v_007e5825:\n"
    "add esp, 0x200\n"
    "v_007e582b:\n"
    "ret\n"
    "_fmt5780_nonzero_one:\n"
    "cmp ecx, 1\n"
    "v_007e582e:\n"
    "mov edx, dword ptr [esp + 0x208]\n"
    "v_007e5835:\n"
    "push 0xa7a1d0\n"
    "v_007e583a:\n"
    "push edx\n"
    CM_ORIGINAL_CALL(007e583b, 007e5840, 0066f4e0)
    "add esp, 8\n"
    "v_007e5843:\n"
    "mov eax, 1\n"
    "v_007e5848:\n"
    "add esp, 0x200\n"
    "v_007e584e:\n"
    "ret\n"
    "_fmt5780_nonzero_lt6:\n"
    "cmp ecx, 6\n"
    "v_007e5854:\n"
    "mov eax, dword ptr [esp + 0x208]\n"
    "v_007e585b:\n"
    "push 0xa7a1c4\n"
    "v_007e5860:\n"
    "push eax\n"
    CM_ORIGINAL_CALL(007e5861, 007e5866, 0066f4e0)
    "add esp, 8\n"
    "v_007e5869:\n"
    "mov eax, 1\n"
    "v_007e586e:\n"
    "add esp, 0x200\n"
    "v_007e5874:\n"
    "ret\n"
    "_fmt5780_nonzero_lt9:\n"
    "cmp ecx, 9\n"
    "v_007e587a:\n"
    "mov ecx, dword ptr [esp + 0x208]\n"
    "v_007e5881:\n"
    "push 0xa7a1b4\n"
    "v_007e5886:\n"
    "push ecx\n"
    CM_ORIGINAL_CALL(007e5887, 007e588c, 0066f4e0)
    "add esp, 8\n"
    "v_007e588f:\n"
    "mov eax, 1\n"
    "v_007e5894:\n"
    "add esp, 0x200\n"
    "v_007e589a:\n"
    "ret\n"
    "_fmt5780_nonzero_lt14:\n"
    "cmp ecx, 14\n"
    "v_007e58a0:\n"
    "mov edx, dword ptr [esp + 0x208]\n"
    "v_007e58a7:\n"
    "push 0xa7a1a4\n"
    "v_007e58ac:\n"
    "push edx\n"
    CM_ORIGINAL_CALL(007e58ad, 007e58b2, 0066f4e0)
    "add esp, 8\n"
    "v_007e58b5:\n"
    "mov eax, 1\n"
    "v_007e58ba:\n"
    "add esp, 0x200\n"
    "v_007e58c0:\n"
    "ret\n"
    "_fmt5780_nonzero_div7:\n"
    "cmp ecx, 28\n"
    "v_007e58c6:\n"
    "mov eax, 0x92492493\n"
    "v_007e58cb:\n"
    "imul ecx\n"
    "v_007e58cd:\n"
    "add edx, ecx\n"
    "v_007e58cf:\n"
    "mov ecx, dword ptr [esp + 0x208]\n"
    "v_007e58d6:\n"
    "sar edx, 2\n"
    "v_007e58d9:\n"
    "mov eax, edx\n"
    "v_007e58db:\n"
    "shr eax, 0x1f\n"
    "v_007e58de:\n"
    "add edx, eax\n"
    "v_007e58e0:\n"
    "push edx\n"
    "v_007e58e1:\n"
    "push 0xa7a188\n"
    "v_007e58e6:\n"
    "push ecx\n"
    CM_ORIGINAL_CALL(007e58e7, 007e58ec, 0066f4e0)
    "add esp, 0xc\n"
    "v_007e58ef:\n"
    "mov eax, 1\n"
    "v_007e58f4:\n"
    "add esp, 0x200\n"
    "v_007e58fa:\n"
    "ret\n"
    "_fmt5780_nonzero_lt40:\n"
    "cmp ecx, 40\n"
    "v_007e5900:\n"
    "mov edx, dword ptr [esp + 0x208]\n"
    "v_007e5907:\n"
    "push 0xa7a178\n"
    "v_007e590c:\n"
    "push edx\n"
    CM_ORIGINAL_CALL(007e590d, 007e5912, 0066f4e0)
    "add esp, 8\n"
    "v_007e5915:\n"
    "mov eax, 1\n"
    "v_007e591a:\n"
    "add esp, 0x200\n"
    "v_007e5920:\n"
    "ret\n"
    "_fmt5780_nonzero_div30:\n"
    "cmp ecx, 365\n"
    "v_007e5929:\n"
    "mov eax, 0x88888889\n"
    "v_007e592e:\n"
    "imul ecx\n"
    "v_007e5930:\n"
    "add edx, ecx\n"
    "v_007e5932:\n"
    "sar edx, 4\n"
    "v_007e5935:\n"
    "mov eax, edx\n"
    "v_007e5937:\n"
    "shr eax, 0x1f\n"
    "v_007e593a:\n"
    "lea ecx, [edx + eax + 1]\n"
    "v_007e593e:\n"
    "push ecx\n"
    "v_007e593f:\n"
    "push 0xa7a15c\n"
    "v_007e5944:\n"
    "jmp v_007e5aeb\n"
    "_fmt5780_nonzero_lt450:\n"
    "cmp ecx, 450\n"
    "v_007e5951:\n"
    "mov eax, dword ptr [esp + 0x208]\n"
    "v_007e5958:\n"
    "push 0xa7a14c\n"
    "v_007e595d:\n"
    "push eax\n"
    CM_ORIGINAL_CALL(007e595e, 007e5963, 0066f4e0)
    "add esp, 8\n"
    "v_007e5966:\n"
    "mov eax, 1\n"
    "v_007e596b:\n"
    "add esp, 0x200\n"
    "v_007e5971:\n"
    "ret\n"
    "_fmt5780_nonzero_lt630:\n"
    "cmp ecx, 630\n"
    "v_007e597a:\n"
    "mov ecx, dword ptr [esp + 0x208]\n"
    "v_007e5981:\n"
    "push 0xa7a13c\n"
    "v_007e5986:\n"
    "push ecx\n"
    CM_ORIGINAL_CALL(007e5987, 007e598c, 0066f4e0)
    "add esp, 8\n"
    "v_007e598f:\n"
    "mov eax, 1\n"
    "v_007e5994:\n"
    "add esp, 0x200\n"
    "v_007e599a:\n"
    "ret\n"
    "_fmt5780_nonzero_lt910:\n"
    "cmp ecx, 910\n"
    "v_007e59a3:\n"
    "mov edx, dword ptr [esp + 0x208]\n"
    "v_007e59aa:\n"
    "push 0xa7a12c\n"
    "v_007e59af:\n"
    "push edx\n"
    CM_ORIGINAL_CALL(007e59b0, 007e59b5, 0066f4e0)
    "add esp, 8\n"
    "v_007e59b8:\n"
    "mov eax, 1\n"
    "v_007e59bd:\n"
    "add esp, 0x200\n"
    "v_007e59c3:\n"
    "ret\n"
    "_fmt5780_nonzero_div365:\n"
    "cmp ecx, 910\n"
    "v_007e59c4:\n"
    "mov eax, 0xb38cf9b1\n"
    "v_007e59c9:\n"
    "imul ecx\n"
    "v_007e59cb:\n"
    "add edx, ecx\n"
    "v_007e59cd:\n"
    "mov ecx, dword ptr [esp + 0x208]\n"
    "v_007e59d4:\n"
    "sar edx, 8\n"
    "v_007e59d7:\n"
    "mov eax, edx\n"
    "v_007e59d9:\n"
    "shr eax, 0x1f\n"
    "v_007e59dc:\n"
    "add edx, eax\n"
    "v_007e59de:\n"
    "push edx\n"
    "v_007e59df:\n"
    "push 0xa7a110\n"
    "v_007e59e4:\n"
    "push ecx\n"
    CM_ORIGINAL_CALL(007e59e5, 007e59ea, 0066f4e0)
    "add esp, 0xc\n"
    "v_007e59ed:\n"
    "mov eax, 1\n"
    "v_007e59f2:\n"
    "add esp, 0x200\n"
    "v_007e59f8:\n"
    "ret\n"
    "_fmt5780_empty_zero:\n"
    "cmp ecx, 1\n"
    "v_007e59fe:\n"
    "mov edx, dword ptr [esp + 0x208]\n"
    "v_007e5a05:\n"
    "mov eax, 1\n"
    "v_007e5a0a:\n"
    "mov byte ptr [edx], 0\n"
    "v_007e5a0d:\n"
    "add esp, 0x200\n"
    "v_007e5a13:\n"
    "ret\n"
    "_fmt5780_zero_one:\n"
    "cmp ecx, 1\n"
    "v_007e5a1e:\n"
    "mov eax, dword ptr [esp + 0x208]\n"
    "v_007e5a25:\n"
    "push 0xa7a108\n"
    "v_007e5a2a:\n"
    "push eax\n"
    CM_ORIGINAL_CALL(007e5a2b, 007e5a30, 0066f4e0)
    "add esp, 8\n"
    "v_007e5a33:\n"
    "mov eax, 1\n"
    "v_007e5a38:\n"
    "add esp, 0x200\n"
    "v_007e5a3e:\n"
    "ret\n"
    "_fmt5780_zero_lt14:\n"
    "cmp ecx, 1\n"
    "v_007e5a3f:\n"
    "push ecx\n"
    "v_007e5a40:\n"
    "mov ecx, dword ptr [esp + 0x20c]\n"
    "v_007e5a47:\n"
    "push 0xa7a0f4\n"
    "v_007e5a4c:\n"
    "push ecx\n"
    CM_ORIGINAL_CALL(007e5a4d, 007e5a52, 0066f4e0)
    "add esp, 0xc\n"
    "v_007e5a55:\n"
    "mov eax, 1\n"
    "v_007e5a5a:\n"
    "add esp, 0x200\n"
    "v_007e5a60:\n"
    "ret\n"
    "_fmt5780_zero_div7:\n"
    "cmp ecx, 43\n"
    "v_007e5a66:\n"
    "mov eax, 0x92492493\n"
    "v_007e5a6b:\n"
    "imul ecx\n"
    "v_007e5a6d:\n"
    "add edx, ecx\n"
    "v_007e5a6f:\n"
    "sar edx, 2\n"
    "v_007e5a72:\n"
    "mov eax, edx\n"
    "v_007e5a74:\n"
    "shr eax, 0x1f\n"
    "v_007e5a77:\n"
    "add edx, eax\n"
    "v_007e5a79:\n"
    "cmp edx, 1\n"
    "v_007e5a7c:\n"
    "jne v_007e5a9f\n"
    "v_007e5a7e:\n"
    "mov ecx, dword ptr [esp + 0x208]\n"
    "v_007e5a85:\n"
    "push 0xa7a0ec\n"
    "v_007e5a8a:\n"
    "push ecx\n"
    CM_ORIGINAL_CALL(007e5a8b, 007e5a90, 0066f4e0)
    "add esp, 8\n"
    "v_007e5a93:\n"
    "mov eax, 1\n"
    "v_007e5a98:\n"
    "add esp, 0x200\n"
    "v_007e5a9e:\n"
    "ret\n"
    "v_007e5a9f:\n"
    "push edx\n"
    "v_007e5aa0:\n"
    "push 0xa7a0d8\n"
    "v_007e5aa5:\n"
    "jmp v_007e5aeb\n"
    "_fmt5780_zero_div30:\n"
    "cmp ecx, 43\n"
    "v_007e5aa7:\n"
    "add ecx, -0xf\n"
    "v_007e5aaa:\n"
    "mov eax, 0x88888889\n"
    "v_007e5aaf:\n"
    "imul ecx\n"
    "v_007e5ab1:\n"
    "add edx, ecx\n"
    "v_007e5ab3:\n"
    "sar edx, 4\n"
    "v_007e5ab6:\n"
    "mov eax, edx\n"
    "v_007e5ab8:\n"
    "shr eax, 0x1f\n"
    "v_007e5abb:\n"
    "lea eax, [edx + eax + 1]\n"
    "v_007e5abf:\n"
    "cmp eax, 1\n"
    "v_007e5ac2:\n"
    "jne v_007e5ae5\n"
    "v_007e5ac4:\n"
    "mov ecx, dword ptr [esp + 0x208]\n"
    "v_007e5acb:\n"
    "push 0xa7a0d0\n"
    "v_007e5ad0:\n"
    "push ecx\n"
    CM_ORIGINAL_CALL(007e5ad1, 007e5ad6, 0066f4e0)
    "add esp, 8\n"
    "v_007e5ad9:\n"
    "mov eax, 1\n"
    "v_007e5ade:\n"
    "add esp, 0x200\n"
    "v_007e5ae4:\n"
    "ret\n"
    "v_007e5ae5:\n"
    "push eax\n"
    "v_007e5ae6:\n"
    "push 0xa7a0b8\n"
    "v_007e5aeb:\n"
    "mov edx, dword ptr [esp + 0x210]\n"
    "v_007e5af2:\n"
    "push edx\n"
    CM_ORIGINAL_CALL(007e5af3, 007e5af8, 0066f4e0)
    "add esp, 0xc\n"
    "v_007e5afb:\n"
    "mov eax, 1\n"
    "v_007e5b00:\n"
    "add esp, 0x200\n"
    "v_007e5b06:\n"
    "ret\n"
    ".att_syntax prefix\n"
);

#define FMT5780_STATE "a"(raw_a),"c"(value),"d"(raw_d),"b"(raw_b),\
                      "S"(raw_s),"D"(raw_i),"r"(raw_p)
#define FMT5780_BARRIER() __asm__ volatile("" : "+a"(raw_a),"+c"(value),"+d"(raw_d),"+b"(raw_b), \
    "+S"(raw_s),"+D"(raw_i),"+r"(raw_p) : : "memory","cc")
#define FMT5780_ROUTE(name) do { \
    __asm__ volatile("jmp _fmt5780_" #name : : FMT5780_STATE : "memory"); \
    __builtin_unreachable(); \
} while (0)

/* All physical registers stay live through C routing. Pinned GCC emission
   must contain only register comparisons/branches here; O2 is explicit
   under both outer optimization modes. Arbitrary aliases and original
   signed comparisons remain part of the contract. */
unsigned long long CM_CDECL __attribute__((naked,optimize("O2")))
cm_fn_007e5780(s32 input_value __attribute__((unused)),
              void *buffer __attribute__((unused)),
              s32 capacity __attribute__((unused)),
              u32 mode __attribute__((unused)))
{
    register u32 raw_a __asm__("eax"), value __asm__("ecx"), raw_d __asm__("edx");
    register u32 raw_b __asm__("ebx"),raw_s __asm__("esi"),raw_i __asm__("edi");
    register u32 raw_p __asm__("ebp");
    __asm__ volatile(
        "v_007e5780:\n movl 12(%%esp), %%eax\n"
        "v_007e5784:\n subl $512, %%esp\n"
        : "=a"(raw_a),"=c"(value),"=d"(raw_d),"=b"(raw_b),
          "=S"(raw_s),"=D"(raw_i),"=r"(raw_p) : : "memory","cc");
    FMT5780_BARRIER();
    if ((s32)raw_a < 12)
        FMT5780_ROUTE(diagnostic);
    __asm__ volatile(
        "cmpl $12, %%eax\n"
        "v_007e57fb:\n movl 528(%%esp), %%eax\n"
        "v_007e5802:\n movl 516(%%esp), %%ecx\n"
        : "+a"(raw_a),"+c"(value),"+d"(raw_d),"+b"(raw_b),
          "+S"(raw_s),"+D"(raw_i),"+r"(raw_p) : : "memory");
    FMT5780_BARRIER();
    if (raw_a != 0) {
        FMT5780_BARRIER();
    if ((s32)value < 1) FMT5780_ROUTE(empty_nonzero);
        FMT5780_BARRIER();
    if (value == 1) FMT5780_ROUTE(nonzero_one);
        FMT5780_BARRIER();
    if ((s32)value < 6) FMT5780_ROUTE(nonzero_lt6);
        FMT5780_BARRIER();
    if ((s32)value < 9) FMT5780_ROUTE(nonzero_lt9);
        FMT5780_BARRIER();
    if ((s32)value < 14) FMT5780_ROUTE(nonzero_lt14);
        FMT5780_BARRIER();
    if ((s32)value < 28) FMT5780_ROUTE(nonzero_div7);
        FMT5780_BARRIER();
    if ((s32)value < 40) FMT5780_ROUTE(nonzero_lt40);
        FMT5780_BARRIER();
    if ((s32)value < 365) FMT5780_ROUTE(nonzero_div30);
        FMT5780_BARRIER();
    if ((s32)value < 450) FMT5780_ROUTE(nonzero_lt450);
        FMT5780_BARRIER();
    if ((s32)value < 630) FMT5780_ROUTE(nonzero_lt630);
        FMT5780_BARRIER();
    if ((s32)value < 910) FMT5780_ROUTE(nonzero_lt910);
        FMT5780_ROUTE(nonzero_div365);
    }
    FMT5780_BARRIER();
    if ((s32)value < 1) FMT5780_ROUTE(empty_zero);
    FMT5780_BARRIER();
    if ((s32)value < 14) {
        FMT5780_BARRIER();
    if (value == 1) FMT5780_ROUTE(zero_one);
        FMT5780_ROUTE(zero_lt14);
    }
    FMT5780_BARRIER();
    if ((s32)value < 43) FMT5780_ROUTE(zero_div7);
    FMT5780_ROUTE(zero_div30);
}
