/* Ignored production VM transport probe; no acceptance claim. */
/* Preferred-image VA 0x007e5160, image base 0x00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Field meanings are unknown. Offsets, widths and access order are recovered. */
#include "cmre.h"
#define SIX_ORIGINAL_CALL(callva, returnva, calleeva) \
    "v_" #callva ":\n" \
    "cmre_call_" #callva "_" #returnva "_" #calleeva ":\n" \
    ".byte 0x68\n.long 0x" #returnva "\n" \
    ".byte 0xe9\n.long _cm_fn_" #calleeva " - . - 4\n" \
    "cmre_return_" #callva "_" #returnva ":\n"


extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void *, void *, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Original diagnostic ABI adapter. Both branches use the original 512-byte
   uninitialized frame. Their buffer argument orders intentionally differ.
   There is no helper CALL and no extra stack save. */
__asm__(
    ".text\n"
    "_serializer_source_diagnostic:\n"
    "cmpl %esi, %ebx\n"
    "v_007e5176:\n leal 12(%esp), %eax\n"
    "v_007e517a:\n leal 268(%esp), %ecx\n"
    "v_007e5181:\n pushl %eax\n"
    "v_007e5182:\n pushl %ecx\n"
    "v_007e5183:\n pushl %esi\n"
    "v_007e5184:\n pushl %esi\n"
    "v_007e5185:\n pushl $0x00a7a088\n"
    SIX_ORIGINAL_CALL(007e518a, 007e518f, 00944cff)
    "v_007e518f:\n addl $20, %esp\n"
    "v_007e5192:\n leal 12(%esp), %edx\n"
    "v_007e5196:\n leal 268(%esp), %eax\n"
    "v_007e519d:\n pushl $0x1bb\n"
    "v_007e51a2:\n pushl %edx\n"
    "v_007e51a3:\n pushl %eax\n"
    "jmp _serializer_diagnostic_common\n"
    "_serializer_destination_diagnostic:\n"
    "cmpl %esi, %edi\n"
    "v_007e51b1:\n leal 268(%esp), %ecx\n"
    "v_007e51b8:\n leal 12(%esp), %edx\n"
    "v_007e51bc:\n pushl %ecx\n"
    "v_007e51bd:\n pushl %edx\n"
    "v_007e51be:\n pushl %esi\n"
    "v_007e51bf:\n pushl %esi\n"
    "v_007e51c0:\n pushl $0x00a7a088\n"
    SIX_ORIGINAL_CALL(007e51c5, 007e51ca, 00944cff)
    "v_007e51ca:\n addl $20, %esp\n"
    "v_007e51cd:\n leal 268(%esp), %eax\n"
    "v_007e51d4:\n leal 12(%esp), %ecx\n"
    "v_007e51d8:\n pushl $0x1c3\n"
    "v_007e51dd:\n pushl %eax\n"
    "v_007e51de:\n pushl %ecx\n"
    "_serializer_diagnostic_common:\n"
    SIX_ORIGINAL_CALL(007e51df, 007e51e4, 0090d130)
    "v_007e51e4:\n pushl %eax\n"
    "v_007e51e5:\n pushl $0x009870e8\n"
    "v_007e51ea:\n pushl $0x00ae24d0\n"
    SIX_ORIGINAL_CALL(007e51ef, 007e51f4, 00944c9f)
    "v_007e51f4:\n pushl %esi\n"
    "v_007e51f5:\n pushl $0x00ae24d0\n"
    "v_007e51fa:\n pushl $0x009870e0\n"
    SIX_ORIGINAL_CALL(007e51ff, 007e5204, 005e8290)
    "v_007e5204:\n addl $36, %esp\n"
    "v_007e5207:\n movl %esi, 0x00b67a34\n"
    "xorl %eax, %eax\n popl %edi\n popl %esi\n popl %ebx\n"
    "addl $512, %esp\n ret\n"
);

#define STRING_(x) #x
#define STRING(x) STRING_(x)
#define LIVE_INPUTS "a" (raw_a), "d" (raw_d), "b" (source), "c" (field), \
                    "S" (counter), "D" (destination), "r" (word_bp)
#define LIVE_OUTPUTS "+a" (raw_a), "+d" (raw_d), "+b" (source), "+c" (field), \
                     "+S" (counter), "+D" (destination), "+r" (word_bp)

/* Counter/register adapter: preserve partial EAX and EDX, byte wrap and INC
   flags (including unchanged carry). The following C field load is deliberately
   sequenced after this store, so counter/source/destination aliases stay visible. */
#define INCREMENT_BEFORE_FIELD(load_va,copy_va,inc_va,store_va) \
    __asm__ volatile ( \
        "v_" STRING(load_va) ":\n movb (%%esi), %%al\n" \
        "v_" STRING(copy_va) ":\n movb %%al, %%dl\n" \
        "v_" STRING(inc_va) ":\n incb %%dl\n" \
        "v_" STRING(store_va) ":\n movb %%dl, (%%esi)\n" \
        : LIVE_OUTPUTS : : "memory", "cc")

/* Original thiscall emission: arguments are field then partial EAX; setter
   failure is ignored. All physical register values are captured after return,
   including any changes caused by aliases into a callee's saved stack slots. */
#define EMIT_DWORD(offset,load_va,copy_va,inc_va,store_va,field_va,push_value_va,push_index_va,self_va,call_va,return_va) \
    do { \
        INCREMENT_BEFORE_FIELD(load_va,copy_va,inc_va,store_va); \
        __asm__ volatile ("v_" STRING(field_va) ":\n" : : LIVE_INPUTS : "memory"); \
        field = *(volatile u32 *)(source + (offset)); \
        __asm__ volatile ( \
            "v_" STRING(push_value_va) ":\n pushl %%ecx\n" \
            "v_" STRING(push_index_va) ":\n pushl %%eax\n" \
            "v_" STRING(self_va) ":\n movl %%edi, %%ecx\n" \
            SIX_ORIGINAL_CALL(call_va, return_va, 00785790) \
            "r_" STRING(return_va) ":\n" \
            : LIVE_OUTPUTS : : "memory", "cc"); \
    } while (0)

/* Pinned-GCC register bridge: the C null routing, ordered dword field loads,
   signed conversion and final unsigned conversion are readable source logic.
   Access/counter/call adapters retain fault registers, original argument order,
   original stack footprint and uninitialized diagnostic buffers. Mixed naked
   C/extended asm needs emitted-code inspection at both configured build levels. */
unsigned long long CM_CDECL __attribute__((naked, optimize("O2")))
cm_fn_007e5160(void *source_arg __attribute__((unused)),
              void *destination_arg __attribute__((unused)),
              void *counter_arg __attribute__((unused)))
{
    register u32 raw_a __asm__("eax");
    register u32 raw_d __asm__("edx");
    register u32 source __asm__("ebx");
    register u32 field __asm__("ecx");
    register u32 counter __asm__("esi");
    register u32 destination __asm__("edi");
    register u32 word_bp __asm__("ebp");
    __asm__ volatile (
        "v_007e5160:\n subl $512, %%esp\n"
        "v_007e5166:\n pushl %%ebx\n"
        "v_007e5167:\n movl 520(%%esp), %%ebx\n"
        "v_007e516e:\n pushl %%esi\n"
        "v_007e516f:\n xorl %%esi, %%esi\n"
        "v_007e5171:\n cmpl %%esi, %%ebx\n"
        "v_007e5173:\n pushl %%edi\n"
        : "=a" (raw_a), "=d" (raw_d), "=b" (source), "=c" (field),
          "=S" (counter), "=D" (destination), "=r" (word_bp) : : "memory", "cc");
    if (source == 0) {
        __asm__ volatile ("jmp _serializer_source_diagnostic" : : LIVE_INPUTS : "memory");
        __builtin_unreachable();
    }
    __asm__ volatile ("v_007e51a6:\n movl 532(%%esp), %%edi"
                      : LIVE_OUTPUTS : : "memory");
    if (destination == 0) {
        __asm__ volatile ("jmp _serializer_destination_diagnostic" : : LIVE_INPUTS : "memory");
        __builtin_unreachable();
    }
    __asm__ volatile (
        /* Reproduce original comparison flags before the counter read. */
        "cmpl %%esi, %%edi\n"
        "v_007e5219:\n movl 536(%%esp), %%esi\n"
        "v_007e5220:\n pushl %%ebp\n"
        : LIVE_OUTPUTS : : "memory", "cc");

    EMIT_DWORD(0,007e5221,007e5223,007e5225,007e5227,007e5229,007e522b,007e522c,007e522d,007e522f,007e5234);
    EMIT_DWORD(4,007e5234,007e5236,007e5238,007e523a,007e523c,007e523f,007e5240,007e5241,007e5243,007e5248);
    EMIT_DWORD(8,007e5248,007e524a,007e524c,007e524e,007e5250,007e5253,007e5254,007e5255,007e5257,007e525c);
    EMIT_DWORD(12,007e525c,007e525e,007e5260,007e5262,007e5264,007e5267,007e5268,007e5269,007e526b,007e5270);

    /* Fifth emission: signed word is loaded before the counter byte. The field
       push precedes INC; the counter store occurs after both argument pushes. */
    __asm__ volatile (
        "v_007e5270:\n movw 16(%%ebx), %%bp\n"
        "v_007e5274:\n movb (%%esi), %%al\n"
        "v_007e5276:\n"
        : LIVE_OUTPUTS : : "memory");
    field = (s32)(s16)(u16)word_bp;
    __asm__ volatile (
        "v_007e5279:\n movb %%al, %%dl\n"
        "v_007e527b:\n pushl %%ecx\n"
        "v_007e527c:\n incb %%dl\n"
        "v_007e527e:\n pushl %%eax\n"
        "v_007e527f:\n movl %%edi, %%ecx\n"
        "v_007e5281:\n movb %%dl, (%%esi)\n"
        SIX_ORIGINAL_CALL(007e5283, 007e5288, 00785790)
        "v_007e5288:\n"
        : LIVE_OUTPUTS : : "memory", "cc");

    /* Sixth emission: the word overwrites working BX before the counter read;
       the unsigned conversion destroys the working source pointer. */
    __asm__ volatile (
        "v_007e5288:\n movw 18(%%ebx), %%bx\n"
        "v_007e528c:\n movb (%%esi), %%al\n"
        "v_007e528e:\n movb %%al, %%dl\n"
        : LIVE_OUTPUTS : : "memory");
    source = (u16)source;
    __asm__ volatile (
        /* The compiler may use MOVZX for the C conversion. Recreate original
           AND flags before INC DL, which preserves carry. */
        "v_007e5290:\n andl $65535, %%ebx\n"
        "v_007e5296:\n incb %%dl\n"
        "v_007e5298:\n pushl %%ebx\n"
        "v_007e5299:\n pushl %%eax\n"
        "v_007e529a:\n movl %%edi, %%ecx\n"
        "v_007e529c:\n movb %%dl, (%%esi)\n"
        SIX_ORIGINAL_CALL(007e529e, 007e52a3, 00785790)
        "v_007e52a3:\n popl %%ebp\n"
        "v_007e52a4:\n popl %%edi\n"
        "v_007e52a5:\n popl %%esi\n"
        "v_007e52a6:\n movl $1, %%eax\n"
        "v_007e52ab:\n popl %%ebx\n"
        "v_007e52ac:\n addl $512, %%esp\n"
        "v_007e52b2:\n ret\n"
        : : LIVE_INPUTS : "memory", "cc");
    __builtin_unreachable();
}
