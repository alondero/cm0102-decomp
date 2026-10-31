/* Eleven-field record reader at preferred-image VA 0x007e4fa0, image base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Field meanings and original semantic return types are unknown. EDX:EAX is
   a validation adapter. Original getter/date/copy/diagnostics remain boundaries;
   declared call transport requires owned VM continuation dispatch. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00785830(void *, u32);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(void *, void *, void *);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void *, void *, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Diagnostic tail adapters use the original frame without a helper CALL.
   Their 256-byte buffers are uninitialized, at entry ESP-200h and ESP-100h.
   The first 16 local bytes remain uninitialized separate temporaries. */
__asm__(
    ".text\n"
    "_reader_eleven_source_diagnostic:\n"
    "cmpl %esi, %edi\n"
    "v_007e4fb6:\n leal 28(%esp), %eax\n"
    "v_007e4fba:\n leal 284(%esp), %ecx\n"
    "v_007e4fc1:\n pushl %eax\n"
    "v_007e4fc2:\n pushl %ecx\n"
    "v_007e4fc3:\n pushl %esi\n"
    "v_007e4fc4:\n pushl %esi\n"
    "v_007e4fc5:\n pushl $0x00a7a088\n"
    CM_ORIGINAL_CALL(007e4fca, 007e4fcf, 00944cff)
    "addl $20, %esp\n"
    "v_007e4fd2:\n leal 28(%esp), %edx\n"
    "v_007e4fd6:\n leal 284(%esp), %eax\n"
    "v_007e4fdd:\n pushl $0x18e\n"
    "v_007e4fe2:\n pushl %edx\n"
    "v_007e4fe3:\n pushl %eax\n"
    "v_007e4fe4:\n jmp _reader_eleven_diagnostic_common\n"
    "_reader_eleven_destination_diagnostic:\n"
    "cmpl %esi, %ebx\n"
    "v_007e4ff1:\n leal 284(%esp), %ecx\n"
    "v_007e4ff8:\n leal 28(%esp), %edx\n"
    "v_007e4ffc:\n pushl %ecx\n"
    "v_007e4ffd:\n pushl %edx\n"
    "v_007e4ffe:\n pushl %esi\n"
    "v_007e4fff:\n pushl %esi\n"
    "v_007e5000:\n pushl $0x00a7a088\n"
    CM_ORIGINAL_CALL(007e5005, 007e500a, 00944cff)
    "addl $20, %esp\n"
    "v_007e500d:\n leal 284(%esp), %eax\n"
    "v_007e5014:\n leal 28(%esp), %ecx\n"
    "v_007e5018:\n pushl $0x196\n"
    "v_007e501d:\n pushl %eax\n"
    "v_007e501e:\n pushl %ecx\n"
    "_reader_eleven_diagnostic_common:\n"
    CM_ORIGINAL_CALL(007e501f, 007e5024, 0090d130)
    "pushl %eax\n"
    "v_007e5025:\n pushl $0x009870e8\n"
    "v_007e502a:\n pushl $0x00ae24d0\n"
    CM_ORIGINAL_CALL(007e502f, 007e5034, 00944c9f)
    "pushl %esi\n"
    "v_007e5035:\n pushl $0x00ae24d0\n"
    "v_007e503a:\n pushl $0x009870e0\n"
    CM_ORIGINAL_CALL(007e503f, 007e5044, 005e8290)
    "addl $36, %esp\n"
    "v_007e5047:\n movl %esi, 0x00b67a34\n"
    "v_007e504d:\n xorl %eax, %eax\n"
    "v_007e504f:\n popl %edi\n"
    "v_007e5050:\n popl %esi\n"
    "v_007e5051:\n popl %ebx\n"
    "v_007e5052:\n addl $528, %esp\n"
    "v_007e5058:\n ret\n"
);

#define READER_ELEVEN_LIVE_INPUTS "a" (raw_a), "d" (raw_d), "b" (destination), \
                    "c" (raw_c), "S" (counter), "D" (source), "r" (raw_bp)
#define READER_ELEVEN_LIVE_OUTPUTS "+a" (raw_a), "+d" (raw_d), "+b" (destination), \
                     "+c" (raw_c), "+S" (counter), "+D" (source), "+r" (raw_bp)

/* A C volatile store with all seven registers live makes pinned GCC spill
   into the original saved-register slots. Emit its original MOV directly;
   the immediate operand names the opaque destination offset. */
#define READER_ELEVEN_STORE_DWORD(offset) \
    __asm__ volatile ("movl %%eax, %c0(%%ebx)" \
        : : "i" (offset), READER_ELEVEN_LIVE_INPUTS : "memory")

#define READER_ELEVEN_STORE_WORD(offset) \
    __asm__ volatile ("movw %%ax, %c0(%%ebx)" \
        : : "i" (offset), READER_ELEVEN_LIVE_INPUTS : "memory")
#define READER_ELEVEN_STORE_BYTE(offset) \
    __asm__ volatile ("movb %%al, %c0(%%ebx)" \
        : : "i" (offset), READER_ELEVEN_LIVE_INPUTS : "memory")
#define READER_ELEVEN_STRING_(x) #x
#define READER_ELEVEN_STRING(x) READER_ELEVEN_STRING_(x)

/* The CL sequence stores the counter before setting the receiver; the DL
   sequence sets the receiver before the store. Preserve both original orders.
   Full EAX is pushed with only AL replaced. INC preserves incoming carry. */
#define READER_ELEVEN_GET_CL(load_va,copy_va,push_va,inc_va,store_va,self_va,call_va,return_va) \
    __asm__ volatile ( \
        "v_" READER_ELEVEN_STRING(load_va) ":\n movb (%%esi), %%al\n" \
        "v_" READER_ELEVEN_STRING(copy_va) ":\n movb %%al, %%cl\n" \
        "v_" READER_ELEVEN_STRING(push_va) ":\n pushl %%eax\n" \
        "v_" READER_ELEVEN_STRING(inc_va) ":\n incb %%cl\n" \
        "v_" READER_ELEVEN_STRING(store_va) ":\n movb %%cl, (%%esi)\n" \
        "v_" READER_ELEVEN_STRING(self_va) ":\n movl %%edi, %%ecx\n" \
        CM_ORIGINAL_CALL(call_va, return_va, 00785830) \
        : READER_ELEVEN_LIVE_OUTPUTS : : "memory", "cc")
#define READER_ELEVEN_GET_DL(load_va,copy_va,push_va,inc_va,self_va,store_va,call_va,return_va) \
    __asm__ volatile ( \
        "v_" READER_ELEVEN_STRING(load_va) ":\n movb (%%esi), %%al\n" \
        "v_" READER_ELEVEN_STRING(copy_va) ":\n movb %%al, %%dl\n" \
        "v_" READER_ELEVEN_STRING(push_va) ":\n pushl %%eax\n" \
        "v_" READER_ELEVEN_STRING(inc_va) ":\n incb %%dl\n" \
        "v_" READER_ELEVEN_STRING(self_va) ":\n movl %%edi, %%ecx\n" \
        "v_" READER_ELEVEN_STRING(store_va) ":\n movb %%dl, (%%esi)\n" \
        CM_ORIGINAL_CALL(call_va, return_va, 00785830) \
        : READER_ELEVEN_LIVE_OUTPUTS : : "memory", "cc")

/* The pinned GCC register bridge retains every physical register across
   original calls, including callee-saved registers changed by stack aliases.
   C performs null routing and names ordered dword-store offsets. The assembly
   retains partial AL/CL/DL, full EAX argument pushes, INC carry, precise fault
   labels, uninitialized stack bytes and original call return-word provenance.
   Both configured builds need emitted-code inspection for unwanted spills. */
unsigned long long CM_CDECL __attribute__((naked, optimize("O2")))
cm_fn_007e4fa0(void *source_arg __attribute__((unused)),
              void *counter_arg __attribute__((unused)),
              void *destination_arg __attribute__((unused)))
{
    register u32 raw_a __asm__("eax");
    register u32 raw_d __asm__("edx");
    register u32 destination __asm__("ebx");
    register u32 raw_c __asm__("ecx");
    register u32 counter __asm__("esi");
    register u32 source __asm__("edi");
    register u32 raw_bp __asm__("ebp");
    __asm__ volatile (
        "v_007e4fa0:\n subl $528, %%esp\n"
        "v_007e4fa6:\n pushl %%ebx\n"
        "v_007e4fa7:\n pushl %%esi\n"
        "v_007e4fa8:\n pushl %%edi\n"
        "v_007e4fa9:\n movl 544(%%esp), %%edi\n"
        "v_007e4fb0:\n xorl %%esi, %%esi\n"
        "v_007e4fb2:\n cmpl %%esi, %%edi\n"
        : "=a" (raw_a), "=d" (raw_d), "=b" (destination), "=c" (raw_c),
          "=S" (counter), "=D" (source), "=r" (raw_bp) : : "memory", "cc");
    if (source == 0) {
        __asm__ volatile ("jmp _reader_eleven_source_diagnostic" : : READER_ELEVEN_LIVE_INPUTS : "memory");
        __builtin_unreachable();
    }
    __asm__ volatile ("v_007e4fe6:\n movl 552(%%esp), %%ebx"
                      : READER_ELEVEN_LIVE_OUTPUTS : : "memory");
    if (destination == 0) {
        __asm__ volatile ("jmp _reader_eleven_destination_diagnostic" : : READER_ELEVEN_LIVE_INPUTS : "memory");
        __builtin_unreachable();
    }
    __asm__ volatile (
        "cmpl %%esi, %%ebx\n"
        "v_007e5059:\n movl 548(%%esp), %%esi\n"
        "v_007e5060:\n pushl %%ebp\n"
        "v_007e5061:\n movl %%edi, %%ecx\n"
        "v_007e5063:\n movb (%%esi), %%al\n"
        "v_007e5065:\n movb %%al, %%dl\n"
        "v_007e5067:\n pushl %%eax\n"
        "v_007e5068:\n incb %%dl\n"
        "v_007e506a:\n movb %%dl, (%%esi)\n"
        CM_ORIGINAL_CALL(007e506c, 007e5071, 00785830)
        : READER_ELEVEN_LIVE_OUTPUTS : : "memory", "cc");
    /* Save the first full getter result; original second-call index uses its
       upper EAX bytes. Both getter failures are ignored. */
    raw_bp = raw_a;
    READER_ELEVEN_GET_CL(007e5073,007e5075,007e5077,007e5078,007e507a,007e507c,007e507e,007e5083);

    /* Original two-argument date/copy calls use adjacent eight-byte windows
       at entry ESP-208h and ESP-210h. No initialization is added. The return
       label from each transport marks the following actual instruction. */
    __asm__ volatile (
        "pushl %%eax\n"
        "v_007e5084:\n pushl %%ebp\n"
        "v_007e5085:\n leal 32(%%esp), %%ecx\n"
        CM_ORIGINAL_CALL(007e5089, 007e508e, 00549ef0)
        "leal 16(%%esp), %%edx\n"
        "v_007e5092:\n pushl %%eax\n"
        "v_007e5093:\n pushl %%edx\n"
        "v_007e5094:\n movl %%ebx, %%ecx\n"
        CM_ORIGINAL_CALL(007e5096, 007e509b, 00417c10)
        "movb (%%esi), %%al\n"
        "v_007e509d:\n movb %%al, %%cl\n"
        "v_007e509f:\n pushl %%eax\n"
        "v_007e50a0:\n incb %%cl\n"
        "v_007e50a2:\n movb %%cl, (%%esi)\n"
        "v_007e50a4:\n movl %%edi, %%ecx\n"
        CM_ORIGINAL_CALL(007e50a6, 007e50ab, 00785830)
        : READER_ELEVEN_LIVE_OUTPUTS : : "memory", "cc");
    READER_ELEVEN_STORE_DWORD(8);

    READER_ELEVEN_GET_DL(007e50ae,007e50b0,007e50b2,007e50b3,007e50b5,007e50b7,007e50b9,007e50be);
    READER_ELEVEN_STORE_WORD(12);

    READER_ELEVEN_GET_CL(007e50c2,007e50c4,007e50c6,007e50c7,007e50c9,007e50cb,007e50cd,007e50d2);
    READER_ELEVEN_STORE_WORD(14);

    READER_ELEVEN_GET_DL(007e50d6,007e50d8,007e50da,007e50db,007e50dd,007e50df,007e50e1,007e50e6);
    READER_ELEVEN_STORE_BYTE(16);

    READER_ELEVEN_GET_CL(007e50e9,007e50eb,007e50ed,007e50ee,007e50f0,007e50f2,007e50f4,007e50f9);
    READER_ELEVEN_STORE_DWORD(17);

    READER_ELEVEN_GET_DL(007e50fc,007e50fe,007e5100,007e5101,007e5103,007e5105,007e5107,007e510c);
    READER_ELEVEN_STORE_DWORD(21);

    READER_ELEVEN_GET_CL(007e510f,007e5111,007e5113,007e5114,007e5116,007e5118,007e511a,007e511f);
    READER_ELEVEN_STORE_BYTE(25);

    READER_ELEVEN_GET_DL(007e5122,007e5124,007e5126,007e5127,007e5129,007e512b,007e512d,007e5132);
    READER_ELEVEN_STORE_BYTE(26);

    READER_ELEVEN_GET_CL(007e5135,007e5137,007e5139,007e513a,007e513c,007e513e,007e5140,007e5145);
    /* The last store faults after EBP/EDI restoration, before ESI restoration.
       Keep the original value in EAX until that unaligned dword store. */
    __asm__ volatile (
        "popl %%ebp\n"
        "v_007e5146:\n popl %%edi\n"
        "v_007e5147:\n movl %%eax, 27(%%ebx)\n"
        "v_007e514a:\n popl %%esi\n"
        "v_007e514b:\n movl $1, %%eax\n"
        "v_007e5150:\n popl %%ebx\n"
        "v_007e5151:\n addl $528, %%esp\n"
        "v_007e5157:\n ret\n"
        : : READER_ELEVEN_LIVE_INPUTS : "memory", "cc");
    __builtin_unreachable();
}
