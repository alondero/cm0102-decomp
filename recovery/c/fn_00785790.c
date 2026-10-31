/* Isolated setter return-word transport diagnostic. */
/* News-slot setter, preferred-image VA 00785790, image base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Index is the signed low byte of the first slot. Other semantic types remain
   opaque; diagnostics are explicit original-code boundaries. EDX:EAX is a
   validation adapter, preserving incidental EDX rather than claiming a 64-bit
   semantic return. No diagnostic-buffer initialization is invented. */
#include "cmre.h"
#define SETTER_ORIGINAL_CALL(callva, returnva, calleeva) \
    "v_" #callva ":\n" \
    "cmre_call_" #callva "_" #returnva "_" #calleeva ":\n" \
    ".byte 0x68\n.long 0x" #returnva "\n" \
    ".byte 0xe9\n.long _cm_fn_" #calleeva " - . - 4\n" \
    "cmre_return_" #callva "_" #returnva ":\n"


extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void *, void *, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* The naked register adapter keeps all stack aliases and fault contexts intact.
   Both fixed build modes inspect this pinned optimized register-only body. */
unsigned long long CM_THISCALL
__attribute__((naked, noinline, used, optimize("O2")))
cm_fn_00785790(void *self_arg __attribute__((unused)),
               u32 index_arg __attribute__((unused)),
               u32 value_arg __attribute__((unused)))
{
    register u32 eax __asm__("eax");
    register u32 edx __asm__("edx");
    register void *self __asm__("ecx");
    register u32 ebx __asm__("ebx");
    register u32 esi __asm__("esi");
    register u32 edi __asm__("edi");
    __asm__ volatile("" : "=a"(eax), "=d"(edx), "=c"(self),
                         "=b"(ebx), "=S"(esi), "=D"(edi));

    /* MOV AL precedes the original 512-byte reservation. The comparisons expose
       their original flags as C conditions, including invalid-path flags. */
    __asm__ volatile("v_00785790:\n\tmovb 4(%%esp), %b0"
                     : "+a"(eax) : : "memory");
    __asm__ volatile("subl $0x200, %%esp" : : : "memory", "cc");
    u8 negative;
    __asm__ volatile("testb %b1, %b1" : "=@ccs"(negative) : "a"(eax) : "cc");
    if (!negative) {
        u8 at_least_50;
        __asm__ volatile("cmpb $50, %b1" : "=@ccge"(at_least_50) : "a"(eax) : "cc");
        if (!at_least_50) {
            /* Valid range is signed byte [0,50); high argument bits are ignored.
               Load the value after reserving the original frame, then perform
               the original unaligned dword store at self + 5 + 4*index. */
            __asm__ volatile("v_007857a2:\n\tmovl 0x208(%%esp), %0"
                             : "=d"(edx) : : "memory");
            eax = (u32)(s32)(s8)eax;
            __asm__ volatile("v_007857ac:\n\tmovl %0, 5(%1,%2,4)"
                             : : "d"(edx), "c"(self), "a"(eax) : "memory");
            eax = 1;
            __asm__ volatile("" : "+b"(ebx), "+S"(esi), "+D"(edi));
            __asm__ volatile("addl $0x200, %%esp\n\tret $8"
                             : : "a"(eax), "d"(edx), "c"(self) : "memory", "cc");
            __builtin_unreachable();
        }
    }

    /* Exact diagnostic ABI: no helper CALL, no scratch frame, unchanged buffers
       at entry ESP-200h and entry ESP-100h, and original call-return provenance. */
    __asm__ volatile("" : "+b"(ebx), "+S"(esi), "+D"(edi));
    __asm__ volatile(
        "v_007857be:\n\tleal (%%esp), %%eax\n\tleal 0x100(%%esp), %%ecx\n"
        "pushl %%eax\n\tpushl %%ecx\n\tpushl $0\n\tpushl $0\n\tpushl $0xa693ec\n"
        SETTER_ORIGINAL_CALL(007857d4, 007857d9, 00944cff)
        "v_007857d9:\n\taddl $0x14, %%esp\n"
        "leal (%%esp), %%edx\n\tleal 0x100(%%esp), %%eax\n"
        "pushl $0x1e5d\n\tpushl %%edx\n\tpushl %%eax\n"
        SETTER_ORIGINAL_CALL(007857ee, 007857f3, 0090d130)
        "v_007857f3:\n"
        "pushl %%eax\n\tpushl $0x9870e8\n\tpushl $0xae24d0\n"
        SETTER_ORIGINAL_CALL(007857fe, 00785803, 00944c9f)
        "v_00785803:\n"
        "pushl $0\n\tpushl $0xae24d0\n\tpushl $0x9870e0\n"
        SETTER_ORIGINAL_CALL(0078580f, 00785814, 005e8290)
        "v_00785814:\n\taddl $0x24, %%esp\n"
        "movl $0, 0xb67a34\n\txorl %%eax, %%eax\n\taddl $0x200, %%esp\n\tret $8"
        : : "a"(eax), "d"(edx), "c"(self) : "memory", "cc");
    __builtin_unreachable();
}
