/* IGNORED DIAGNOSTIC DERIVATIVE. Production CM_ORIGINAL_CALL execution
   transport; historical candidate remains immutable and escalated. */
/* Ordered eleven-field serializer at preferred-image VA 007e4de0, base00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Opaque record offsets retain original signedness and unaligned widths.
   Counter/destination/source/stack may alias. Setter errors are ignored. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void *,void *,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

typedef s16 packed_s16 __attribute__((aligned(1),may_alias));
typedef u32 packed_u32 __attribute__((aligned(1),may_alias));
#define STATE "+a"(eax), "+c"(ecx), "+d"(edx), "+b"(source), "+S"(counter), "+D"(destination), "+r"(bp)
#define STEP(text) __asm__ volatile(text : STATE : : "memory", "cc")
#define LABEL(va) STEP("v_" #va ":")
#define FIELD(va, type, offset) do { \
    LABEL(va); edx = (u32)*(volatile type *)((u8 *)source+(offset)); \
    STEP(""); \
} while(0)
/* INC byte preserves carry and wraps modulo256; a C addition would alter flags.
   Narrow byte/register adapters retain incoming high halves at fault boundaries. */
#define ADVANCE(loadva, storeva) \
    STEP("v_" #loadva ":\n\tmovb (%%esi), %%al\n\tmovb %%al, %%cl\n\tincb %%cl\n" \
         "v_" #storeva ":\n\tmovb %%cl, (%%esi)")
#define RECEIVER() STEP("movl %%edi, %%ecx")
#define SETTER(callva, returnva, receiver_after_push) \
    STEP("pushl %%edx\n\tpushl %%eax\n\t" receiver_after_push \
         CM_ORIGINAL_CALL(callva, returnva, 00785790))

/* No helper CALL: the frame adapter tail-transfers here with original registers.
   C field reads must emit direct EBX-relative accesses, inspected in both modes. */
void __attribute__((naked,noinline,used,optimize("O2"))) cm_serialize_007e4de0(void)
{
    register u32 eax __asm__("eax"), ecx __asm__("ecx"), edx __asm__("edx");
    register void *source __asm__("ebx"), *counter __asm__("esi"), *destination __asm__("edi");
    register u32 bp __asm__("ebp");
    __asm__ volatile("" : "=a"(eax),"=c"(ecx),"=d"(edx),"=b"(source),"=S"(counter),"=D"(destination),"=r"(bp));
    STEP("movl 0x218(%%esp), %%esi\n\tpushl %%ebp");

    /* Signed word+0 is read before the counter. Partial BP and DL are observable. */
    STEP("v_007e4ea1:\n\tmovw (%%ebx), %%bp\n v_007e4ea4:\n\tmovb (%%esi), %%al");
    ecx = (u32)(s32)(s16)bp;
    STEP("movb %%al, %%dl\n\tpushl %%ecx\n\tincb %%dl\n\tpushl %%eax\n\tmovl %%edi, %%ecx\n"
         "v_007e4eb1:\n\tmovb %%dl, (%%esi)\n\t" CM_ORIGINAL_CALL(007e4eb3, 007e4eb8, 00785790) "");

    /* Unsigned word+2 is read before this counter and before argument pushes. */
    STEP("v_00007e4eb8:\n\tmovw 2(%%ebx), %%ax\n v_007e4ebc:\n\tmovb (%%esi), %%cl");
    eax &= 0xffffu;
    /* GCC may emit MOVZX for the C mask. Restore original AND flags before INC
       preserves carry and before either counter/argument alias can fault. */
    STEP("andl $0xffff, %%eax\n\tmovb %%cl, %%dl\n\tpushl %%eax\n\tincb %%dl\n\tpushl %%ecx\n\tmovl %%edi, %%ecx\n"
         "v_007e4ecb:\n\tmovb %%dl, (%%esi)\n\t" CM_ORIGINAL_CALL(007e4ecd, 007e4ed2, 00785790) "");

    /* Later fields increment/store the counter before reading the source. */
    ADVANCE(00007e4ed2,007e4ed8);
    FIELD(007e4eda,packed_u32,8);
    SETTER(007e4ee1,007e4ee6,"movl %%edi, %%ecx\n\t");

    ADVANCE(00007e4ee6,007e4eec);
    RECEIVER(); FIELD(007e4ef0,packed_s16,0x0c);
    SETTER(007e4ef6,007e4efb,"");

    ADVANCE(00007e4efb,007e4f01);
    RECEIVER(); FIELD(007e4f05,packed_s16,0x0e);
    SETTER(007e4f0b,007e4f10,"");

    ADVANCE(00007e4f10,007e4f16);
    RECEIVER(); FIELD(007e4f1a,s8,0x10);
    SETTER(007e4f20,007e4f25,"");

    ADVANCE(00007e4f25,007e4f2b);
    FIELD(007e4f2d,packed_u32,0x11);
    SETTER(007e4f34,007e4f39,"movl %%edi, %%ecx\n\t");

    ADVANCE(00007e4f39,007e4f3f);
    FIELD(007e4f41,packed_u32,0x15);
    SETTER(007e4f48,007e4f4d,"movl %%edi, %%ecx\n\t");

    ADVANCE(00007e4f4d,007e4f53);
    FIELD(007e4f55,s8,0x19);
    SETTER(007e4f5d,007e4f62,"movl %%edi, %%ecx\n\t");

    ADVANCE(00007e4f62,007e4f68);
    RECEIVER(); FIELD(007e4f6c,s8,0x1a);
    SETTER(007e4f72,007e4f77,"");

    ADVANCE(00007e4f77,007e4f7d);
    FIELD(007e4f7f,packed_u32,0x1b);
    SETTER(007e4f86,007e4f8b,"movl %%edi, %%ecx\n\t");
    STEP("popl %%ebp\n\tpopl %%edi\n\tpopl %%esi\n\tmovl $1, %%eax\n\tpopl %%ebx\n\taddl $0x200, %%esp\n\tret");
    __builtin_unreachable();
}

/* The frame/diagnostic adapter preserves uninitialized buffers, register saves,
   null-check order and the original inconsistent buffer argument order. */
unsigned long long CM_CDECL __attribute__((naked,noinline,used))
cm_fn_007e4de0(void *source __attribute__((unused)),void *destination __attribute__((unused)),void *counter __attribute__((unused)))
{
    __asm__ volatile(
        ".intel_syntax noprefix\n sub esp, 0x200\n push ebx\n mov ebx, dword ptr [esp+0x208]\n"
        "push esi\n xor esi, esi\n cmp ebx, esi\n push edi\n jne 1f\n"
        "lea eax, [esp+0xc]\n lea ecx, [esp+0x10c]\n push eax\n push ecx\n push esi\n push esi\n push 0xa7a088\n"
        "" CM_ORIGINAL_CALL(007e4e0a, 007e4e0f, 00944cff) "\n add esp, 0x14\n"
        "lea edx, [esp+0xc]\n lea eax, [esp+0x10c]\n push 0x160\n push edx\n push eax\n jmp 2f\n"
        "1:\n mov edi, dword ptr [esp+0x214]\n cmp edi, esi\n jne 3f\n"
        "lea ecx, [esp+0x10c]\n lea edx, [esp+0xc]\n push ecx\n push edx\n push esi\n push esi\n push 0xa7a088\n"
        "" CM_ORIGINAL_CALL(007e4e45, 007e4e4a, 00944cff) "\n add esp, 0x14\n"
        "lea eax, [esp+0x10c]\n lea ecx, [esp+0xc]\n push 0x168\n push eax\n push ecx\n"
        "2:\n " CM_ORIGINAL_CALL(007e4e5f, 007e4e64, 0090d130) "\n push eax\n push 0x9870e8\n push 0xae24d0\n"
        "" CM_ORIGINAL_CALL(007e4e6f, 007e4e74, 00944c9f) "\n push esi\n push 0xae24d0\n push 0x9870e0\n"
        "" CM_ORIGINAL_CALL(007e4e7f, 007e4e84, 005e8290) "\n add esp, 0x24\n mov dword ptr [0xb67a34], esi\n xor eax, eax\n"
        "pop edi\n pop esi\n pop ebx\n add esp, 0x200\n ret\n"
        "3:\n jmp _cm_serialize_007e4de0\n .att_syntax prefix\n");
}
