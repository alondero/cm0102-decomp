/* Record reader at preferred-image VA 0x007e52c0, image base 0x00400000.
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
    "_reader_six_source_diagnostic:\n"
    "cmpl %esi, %edi\n"
    "v_007e52d6:\n leal 28(%esp), %eax\n"
    "v_007e52da:\n leal 284(%esp), %ecx\n"
    "v_007e52e1:\n pushl %eax\n"
    "v_007e52e2:\n pushl %ecx\n"
    "v_007e52e3:\n pushl %esi\n"
    "v_007e52e4:\n pushl %esi\n"
    "v_007e52e5:\n pushl $0x00a7a088\n"
    CM_ORIGINAL_CALL(007e52ea, 007e52ef, 00944cff)
    "addl $20, %esp\n"
    "v_007e52f2:\n leal 28(%esp), %edx\n"
    "v_007e52f6:\n leal 284(%esp), %eax\n"
    "v_007e52fd:\n pushl $0x1e4\n"
    "v_007e5302:\n pushl %edx\n"
    "v_007e5303:\n pushl %eax\n"
    "v_007e5304:\n jmp _reader_six_diagnostic_common\n"
    "_reader_six_destination_diagnostic:\n"
    "cmpl %esi, %ebx\n"
    "v_007e5311:\n leal 284(%esp), %ecx\n"
    "v_007e5318:\n leal 28(%esp), %edx\n"
    "v_007e531c:\n pushl %ecx\n"
    "v_007e531d:\n pushl %edx\n"
    "v_007e531e:\n pushl %esi\n"
    "v_007e531f:\n pushl %esi\n"
    "v_007e5320:\n pushl $0x00a7a088\n"
    CM_ORIGINAL_CALL(007e5325, 007e532a, 00944cff)
    "addl $20, %esp\n"
    "v_007e532d:\n leal 284(%esp), %eax\n"
    "v_007e5334:\n leal 28(%esp), %ecx\n"
    "v_007e5338:\n pushl $0x1ec\n"
    "v_007e533d:\n pushl %eax\n"
    "v_007e533e:\n pushl %ecx\n"
    "_reader_six_diagnostic_common:\n"
    CM_ORIGINAL_CALL(007e533f, 007e5344, 0090d130)
    "pushl %eax\n"
    "v_007e5345:\n pushl $0x009870e8\n"
    "v_007e534a:\n pushl $0x00ae24d0\n"
    CM_ORIGINAL_CALL(007e534f, 007e5354, 00944c9f)
    "pushl %esi\n"
    "v_007e5355:\n pushl $0x00ae24d0\n"
    "v_007e535a:\n pushl $0x009870e0\n"
    CM_ORIGINAL_CALL(007e535f, 007e5364, 005e8290)
    "addl $36, %esp\n"
    "v_007e5367:\n movl %esi, 0x00b67a34\n"
    "v_007e536d:\n xorl %eax, %eax\n"
    "v_007e536f:\n popl %edi\n"
    "v_007e5370:\n popl %esi\n"
    "v_007e5371:\n popl %ebx\n"
    "v_007e5372:\n addl $528, %esp\n"
    "v_007e5378:\n ret\n"
);

#define READER_SIX_LIVE_INPUTS "a" (raw_a), "d" (raw_d), "b" (destination), \
                    "c" (raw_c), "S" (counter), "D" (source), "r" (raw_bp)
#define READER_SIX_LIVE_OUTPUTS "+a" (raw_a), "+d" (raw_d), "+b" (destination), \
                     "+c" (raw_c), "+S" (counter), "+D" (source), "+r" (raw_bp)

/* A C volatile store with all seven registers live makes pinned GCC spill
   into the original saved-register slots. Emit its original MOV directly;
   the immediate operand names the opaque destination offset. */
#define READER_SIX_STORE_DWORD(offset) \
    __asm__ volatile ("movl %%eax, %c0(%%ebx)" \
        : : "i" (offset), READER_SIX_LIVE_INPUTS : "memory")

/* The pinned GCC register bridge retains every physical register across
   original calls, including callee-saved registers changed by stack aliases.
   C performs null routing and names ordered dword-store offsets. The assembly
   retains partial AL/CL/DL, full EAX argument pushes, INC carry, precise fault
   labels, uninitialized stack bytes and original call return-word provenance.
   Both configured builds need emitted-code inspection for unwanted spills. */
unsigned long long CM_CDECL __attribute__((naked, optimize("O2")))
cm_fn_007e52c0(void *source_arg __attribute__((unused)),
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
        "v_007e52c0:\n subl $528, %%esp\n"
        "v_007e52c6:\n pushl %%ebx\n"
        "v_007e52c7:\n pushl %%esi\n"
        "v_007e52c8:\n pushl %%edi\n"
        "v_007e52c9:\n movl 544(%%esp), %%edi\n"
        "v_007e52d0:\n xorl %%esi, %%esi\n"
        "v_007e52d2:\n cmpl %%esi, %%edi\n"
        : "=a" (raw_a), "=d" (raw_d), "=b" (destination), "=c" (raw_c),
          "=S" (counter), "=D" (source), "=r" (raw_bp) : : "memory", "cc");
    if (source == 0) {
        __asm__ volatile ("jmp _reader_six_source_diagnostic" : : READER_SIX_LIVE_INPUTS : "memory");
        __builtin_unreachable();
    }
    __asm__ volatile ("v_007e5306:\n movl 552(%%esp), %%ebx"
                      : READER_SIX_LIVE_OUTPUTS : : "memory");
    if (destination == 0) {
        __asm__ volatile ("jmp _reader_six_destination_diagnostic" : : READER_SIX_LIVE_INPUTS : "memory");
        __builtin_unreachable();
    }
    __asm__ volatile (
        "cmpl %%esi, %%ebx\n"
        "v_007e5379:\n movl 548(%%esp), %%esi\n"
        "v_007e5380:\n pushl %%ebp\n"
        "v_007e5381:\n movl %%edi, %%ecx\n"
        "v_007e5383:\n movb (%%esi), %%al\n"
        "v_007e5385:\n movb %%al, %%dl\n"
        "v_007e5387:\n pushl %%eax\n"
        "v_007e5388:\n incb %%dl\n"
        "v_007e538a:\n movb %%dl, (%%esi)\n"
        CM_ORIGINAL_CALL(007e538c, 007e5391, 00785830)
        : READER_SIX_LIVE_OUTPUTS : : "memory", "cc");
    READER_SIX_STORE_DWORD(0);

    __asm__ volatile (
        "v_007e5393:\n movb (%%esi), %%al\n"
        "v_007e5395:\n movb %%al, %%cl\n"
        "v_007e5397:\n pushl %%eax\n"
        "v_007e5398:\n incb %%cl\n"
        "v_007e539a:\n movb %%cl, (%%esi)\n"
        "v_007e539c:\n movl %%edi, %%ecx\n"
        CM_ORIGINAL_CALL(007e539e, 007e53a3, 00785830)
        : READER_SIX_LIVE_OUTPUTS : : "memory", "cc");
    READER_SIX_STORE_DWORD(4);

    __asm__ volatile (
        "v_007e53a6:\n movb (%%esi), %%al\n"
        "v_007e53a8:\n movb %%al, %%dl\n"
        "v_007e53aa:\n pushl %%eax\n"
        "v_007e53ab:\n incb %%dl\n"
        "v_007e53ad:\n movl %%edi, %%ecx\n"
        "v_007e53af:\n movb %%dl, (%%esi)\n"
        CM_ORIGINAL_CALL(007e53b1, 007e53b6, 00785830)
        : READER_SIX_LIVE_OUTPUTS : : "memory", "cc");
    READER_SIX_STORE_DWORD(8);

    __asm__ volatile (
        "v_007e53b9:\n movb (%%esi), %%al\n"
        "v_007e53bb:\n movb %%al, %%cl\n"
        "v_007e53bd:\n pushl %%eax\n"
        "v_007e53be:\n incb %%cl\n"
        "v_007e53c0:\n movb %%cl, (%%esi)\n"
        "v_007e53c2:\n movl %%edi, %%ecx\n"
        CM_ORIGINAL_CALL(007e53c4, 007e53c9, 00785830)
        : READER_SIX_LIVE_OUTPUTS : : "memory", "cc");
    READER_SIX_STORE_DWORD(12);

    __asm__ volatile (
        "v_007e53cc:\n movb (%%esi), %%al\n"
        "v_007e53ce:\n movb %%al, %%dl\n"
        "v_007e53d0:\n pushl %%eax\n"
        "v_007e53d1:\n incb %%dl\n"
        "v_007e53d3:\n movl %%edi, %%ecx\n"
        "v_007e53d5:\n movb %%dl, (%%esi)\n"
        CM_ORIGINAL_CALL(007e53d7, 007e53dc, 00785830)
        : READER_SIX_LIVE_OUTPUTS : : "memory", "cc");
    raw_bp = raw_a;
    __asm__ volatile (
        "v_007e53de:\n movb (%%esi), %%al\n"
        "v_007e53e0:\n movb %%al, %%cl\n"
        "v_007e53e2:\n pushl %%eax\n"
        "v_007e53e3:\n incb %%cl\n"
        "v_007e53e5:\n movb %%cl, (%%esi)\n"
        "v_007e53e7:\n movl %%edi, %%ecx\n"
        CM_ORIGINAL_CALL(007e53e9, 007e53ee, 00785830)
        "pushl %%eax\n"
        "v_007e53ef:\n pushl %%ebp\n"
        /* ESP includes both arguments: self is entry ESP-208h. */
        "v_007e53f0:\n leal 32(%%esp), %%ecx\n"
        CM_ORIGINAL_CALL(007e53f4, 007e53f9, 00549ef0)
        /* After RET 8, the secondary temporary is entry ESP-210h. */
        "leal 16(%%esp), %%edx\n"
        "v_007e53fd:\n pushl %%eax\n"
        "v_007e53fe:\n pushl %%edx\n"
        "v_007e53ff:\n leal 16(%%ebx), %%ecx\n"
        CM_ORIGINAL_CALL(007e5402, 007e5407, 00417c10)
        "popl %%ebp\n"
        "v_007e5408:\n popl %%edi\n"
        "v_007e5409:\n popl %%esi\n"
        "v_007e540a:\n movl $1, %%eax\n"
        "v_007e540f:\n popl %%ebx\n"
        "v_007e5410:\n addl $528, %%esp\n"
        "v_007e5416:\n ret\n"
        : : READER_SIX_LIVE_INPUTS : "memory", "cc");
    __builtin_unreachable();
}
