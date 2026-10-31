/* Ignored production VM transport probe; no acceptance claim. */
/* Preferred-image VA 0x007e5420, image base 0x00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Field meanings are unknown. Offsets, widths and access order are recovered. */
#include "cmre.h"
#define FIVE_ORIGINAL_CALL(callva, returnva, calleeva) \
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
    "_serializer5_source_diagnostic:\n"
    "cmpl %esi, %ebx\n"
    "v_007e5436:\n leal 12(%esp), %eax\n"
    "v_007e543a:\n leal 268(%esp), %ecx\n"
    "v_007e5441:\n pushl %eax\n"
    "v_007e5442:\n pushl %ecx\n"
    "v_007e5443:\n pushl %esi\n"
    "v_007e5444:\n pushl %esi\n"
    "v_007e5445:\n pushl $0x00a7a088\n"
    FIVE_ORIGINAL_CALL(007e544a, 007e544f, 00944cff)
    "v_007e544f:\n addl $20, %esp\n"
    "v_007e5452:\n leal 12(%esp), %edx\n"
    "v_007e5456:\n leal 268(%esp), %eax\n"
    "v_007e545d:\n pushl $0x20c\n"
    "v_007e5462:\n pushl %edx\n"
    "v_007e5463:\n pushl %eax\n"
    "jmp _serializer5_diagnostic_common\n"
    "_serializer5_destination_diagnostic:\n"
    "cmpl %esi, %edi\n"
    "v_007e5471:\n leal 268(%esp), %ecx\n"
    "v_007e5478:\n leal 12(%esp), %edx\n"
    "v_007e547c:\n pushl %ecx\n"
    "v_007e547d:\n pushl %edx\n"
    "v_007e547e:\n pushl %esi\n"
    "v_007e547f:\n pushl %esi\n"
    "v_007e5480:\n pushl $0x00a7a088\n"
    FIVE_ORIGINAL_CALL(007e5485, 007e548a, 00944cff)
    "v_007e548a:\n addl $20, %esp\n"
    "v_007e548d:\n leal 268(%esp), %eax\n"
    "v_007e5494:\n leal 12(%esp), %ecx\n"
    "v_007e5498:\n pushl $0x214\n"
    "v_007e549d:\n pushl %eax\n"
    "v_007e549e:\n pushl %ecx\n"
    "_serializer5_diagnostic_common:\n"
    FIVE_ORIGINAL_CALL(007e549f, 007e54a4, 0090d130)
    "v_007e54a4:\n pushl %eax\n"
    "v_007e54a5:\n pushl $0x009870e8\n"
    "v_007e54aa:\n pushl $0x00ae24d0\n"
    FIVE_ORIGINAL_CALL(007e54af, 007e54b4, 00944c9f)
    "v_007e54b4:\n pushl %esi\n"
    "v_007e54b5:\n pushl $0x00ae24d0\n"
    "v_007e54ba:\n pushl $0x009870e0\n"
    FIVE_ORIGINAL_CALL(007e54bf, 007e54c4, 005e8290)
    "v_007e54c4:\n addl $36, %esp\n"
    "v_007e54c7:\n movl %esi, 0x00b67a34\n"
    "xorl %eax, %eax\n popl %edi\n popl %esi\n popl %ebx\n"
    "addl $512, %esp\n ret\n"
);

#define FIVE_STRING_(x) #x
#define FIVE_STRING(x) FIVE_STRING_(x)
#define FIVE_LIVE_INPUTS "a" (raw_a), "d" (raw_d), "b" (source), "c" (field), \
                    "S" (counter), "D" (destination), "r" (word_bp)
#define FIVE_LIVE_OUTPUTS "+a" (raw_a), "+d" (raw_d), "+b" (source), "+c" (field), \
                     "+S" (counter), "+D" (destination), "+r" (word_bp)

/* Counter/register adapter: preserve partial EAX and EDX, byte wrap and INC
   flags (including unchanged carry). The following C field load is deliberately
   sequenced after this store, so counter/source/destination aliases stay visible. */
#define FIVE_INCREMENT_BEFORE_FIELD(load_va,copy_va,inc_va,store_va) \
    __asm__ volatile ( \
        "v_" FIVE_STRING(load_va) ":\n movb (%%esi), %%al\n" \
        "v_" FIVE_STRING(copy_va) ":\n movb %%al, %%dl\n" \
        "v_" FIVE_STRING(inc_va) ":\n incb %%dl\n" \
        "v_" FIVE_STRING(store_va) ":\n movb %%dl, (%%esi)\n" \
        : FIVE_LIVE_OUTPUTS : : "memory", "cc")

/* Original thiscall emission: arguments are field then partial EAX; setter
   failure is ignored. All physical register values are captured after return,
   including any changes caused by aliases into a callee's saved stack slots. */
#define FIVE_EMIT_DWORD(offset,load_va,copy_va,inc_va,store_va,field_va,push_value_va,push_index_va,self_va,call_va,return_va) \
    do { \
        FIVE_INCREMENT_BEFORE_FIELD(load_va,copy_va,inc_va,store_va); \
        __asm__ volatile ("v_" FIVE_STRING(field_va) ":\n" : : FIVE_LIVE_INPUTS : "memory"); \
        field = *(volatile u32 *)(source + (offset)); \
        __asm__ volatile ( \
            "v_" FIVE_STRING(push_value_va) ":\n pushl %%ecx\n" \
            "v_" FIVE_STRING(push_index_va) ":\n pushl %%eax\n" \
            "v_" FIVE_STRING(self_va) ":\n movl %%edi, %%ecx\n" \
            FIVE_ORIGINAL_CALL(call_va, return_va, 00785790) \
            "r_" FIVE_STRING(return_va) ":\n" \
            : FIVE_LIVE_OUTPUTS : : "memory", "cc"); \
    } while (0)

/* Pinned-GCC register bridge: the C null routing, ordered dword field loads,
   signed conversion and final unsigned conversion are readable source logic.
   Access/counter/call adapters retain fault registers, original argument order,
   original stack footprint and uninitialized diagnostic buffers. Mixed naked
   C/extended asm needs emitted-code inspection at both configured build levels. */
unsigned long long CM_CDECL __attribute__((naked, optimize("O2")))
cm_fn_007e5420(void *source_arg __attribute__((unused)),
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
        "v_007e5420:\n subl $512, %%esp\n"
        "v_007e5426:\n pushl %%ebx\n"
        "v_007e5427:\n movl 520(%%esp), %%ebx\n"
        "v_007e542e:\n pushl %%esi\n"
        "v_007e542f:\n xorl %%esi, %%esi\n"
        "v_007e5431:\n cmpl %%esi, %%ebx\n"
        "v_007e5433:\n pushl %%edi\n"
        : "=a" (raw_a), "=d" (raw_d), "=b" (source), "=c" (field),
          "=S" (counter), "=D" (destination), "=r" (word_bp) : : "memory", "cc");
    if (source == 0) {
        __asm__ volatile ("jmp _serializer5_source_diagnostic" : : FIVE_LIVE_INPUTS : "memory");
        __builtin_unreachable();
    }
    __asm__ volatile ("v_007e5466:\n movl 532(%%esp), %%edi"
                      : FIVE_LIVE_OUTPUTS : : "memory");
    if (destination == 0) {
        __asm__ volatile ("jmp _serializer5_destination_diagnostic" : : FIVE_LIVE_INPUTS : "memory");
        __builtin_unreachable();
    }
    __asm__ volatile (
        /* Reproduce original comparison flags before the counter read. */
        "cmpl %%esi, %%edi\n"
        "v_007e54d9:\n movl 536(%%esp), %%esi\n"
        "v_007e54e0:\n pushl %%ebp\n"
        : FIVE_LIVE_OUTPUTS : : "memory", "cc");


    FIVE_EMIT_DWORD(0,007e54e1,007e54e3,007e54e5,007e54e7,007e54e9,007e54eb,007e54ec,007e54ed,007e54ef,007e54f4);

    /* Signed word +4: field load precedes counter; push value before INC. */
    __asm__ volatile(
        "v_007e54f4:\n movw 4(%%ebx), %%bp\n"
        "v_007e54f8:\n movb (%%esi), %%al\n"
        "v_007e54fa:\n"
        : FIVE_LIVE_OUTPUTS : : "memory");
    field = (s32)(s16)(u16)word_bp;
    __asm__ volatile(
        "v_007e54fd:\n movb %%al, %%dl\n"
        "v_007e54ff:\n pushl %%ecx\n"
        "v_007e5500:\n incb %%dl\n"
        "v_007e5502:\n pushl %%eax\n"
        "v_007e5503:\n movl %%edi, %%ecx\n"
        "v_007e5505:\n movb %%dl, (%%esi)\n"
        FIVE_ORIGINAL_CALL(007e5507,007e550c,00785790)
        : FIVE_LIVE_OUTPUTS : : "memory", "cc");

    /* Unsigned word +6 replaces AX, while index replaces only CL. */
    __asm__ volatile(
        "v_007e550c:\n movw 6(%%ebx), %%ax\n"
        "v_007e5510:\n movb (%%esi), %%cl\n"
        : FIVE_LIVE_OUTPUTS : : "memory");
    raw_a = (u16)raw_a;
    __asm__ volatile(
        "v_007e5512:\n andl $65535, %%eax\n"
        "v_007e5517:\n movb %%cl, %%dl\n"
        "v_007e5519:\n pushl %%eax\n"
        "v_007e551a:\n incb %%dl\n"
        "v_007e551c:\n pushl %%ecx\n"
        "v_007e551d:\n movl %%edi, %%ecx\n"
        "v_007e551f:\n movb %%dl, (%%esi)\n"
        FIVE_ORIGINAL_CALL(007e5521,007e5526,00785790)
        : FIVE_LIVE_OUTPUTS : : "memory", "cc");

    /* Signed word +C goes to EDX; increment CL, push value, then store counter. */
    __asm__ volatile(
        "v_007e5526:\n movw 12(%%ebx), %%bp\n"
        "v_007e552a:\n movb (%%esi), %%al\n"
        "v_007e552c:\n movb %%al, %%cl\n"
        "v_007e552e:\n"
        : FIVE_LIVE_OUTPUTS : : "memory");
    raw_d = (s32)(s16)(u16)word_bp;
    __asm__ volatile(
        "v_007e5531:\n incb %%cl\n"
        "v_007e5533:\n pushl %%edx\n"
        "v_007e5534:\n movb %%cl, (%%esi)\n"
        "v_007e5536:\n pushl %%eax\n"
        "v_007e5537:\n movl %%edi, %%ecx\n"
        FIVE_ORIGINAL_CALL(007e5539,007e553e,00785790)
        : FIVE_LIVE_OUTPUTS : : "memory", "cc");

    /* Counter read precedes +E word overwriting working BX. */
    __asm__ volatile(
        "v_007e553e:\n movb (%%esi), %%al\n"
        "v_007e5540:\n movw 14(%%ebx), %%bx\n"
        "v_007e5544:\n movb %%al, %%cl\n"
        : FIVE_LIVE_OUTPUTS : : "memory");
    source = (u16)source;
    __asm__ volatile(
        "v_007e5546:\n andl $65535, %%ebx\n"
        "v_007e554c:\n incb %%cl\n"
        "v_007e554e:\n pushl %%ebx\n"
        "v_007e554f:\n movb %%cl, (%%esi)\n"
        "v_007e5551:\n pushl %%eax\n"
        "v_007e5552:\n movl %%edi, %%ecx\n"
        FIVE_ORIGINAL_CALL(007e5554,007e5559,00785790)
        "v_007e5559:\n popl %%ebp\n"
        "v_007e555a:\n popl %%edi\n"
        "v_007e555b:\n popl %%esi\n"
        "v_007e555c:\n movl $1, %%eax\n"
        "v_007e5561:\n popl %%ebx\n"
        "v_007e5562:\n addl $512, %%esp\n"
        "v_007e5568:\n ret\n"
        : : FIVE_LIVE_INPUTS : "memory", "cc");
    __builtin_unreachable();
}
