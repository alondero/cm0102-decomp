/* Preferred-image VA 007e56d0, image base 00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Field names and original declared return type remain unknown.
   EDX:EAX and the naked register body are validation adapters for pinned GCC.
   Declared VM transport is explicit; acceptance is recorded in the cmre ledger.
   Native standalone and non-RET unwinds remain unresolved. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void *, void *, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

unsigned long long CM_CDECL
__attribute__((naked, noinline, used, optimize("O2")))
cm_fn_007e56d0(void *record_arg __attribute__((unused)))
{
    register u32 eax __asm__("eax");
    register u32 edx __asm__("edx");
    register u32 ecx __asm__("ecx");
    register u32 ebx __asm__("ebx");
    register u32 ebp __asm__("ebp");
    register u32 esi __asm__("esi");
    register u32 edi __asm__("edi");
    __asm__ volatile("" : "=a"(eax), "=d"(edx), "=c"(ecx),
                         "=b"(ebx), "=r"(ebp), "=S"(esi), "=D"(edi));

    /* Load the argument before reserving the original uninitialized frame. */
    __asm__ volatile("v_007e56d0:\n\tmovl 4(%%esp), %%ecx\n"
                     "subl $0x200, %%esp"
                     : "+c"(ecx) : : "memory", "cc");
    if (ecx) {
        /* Partial loads preserve upper EAX and the pointer's upper ECX bits.
           The second address still uses the full pointer before CL changes. */
        __asm__ volatile("v_007e574a:\n\tmovb 0x19(%%ecx), %%al\n"
                         "v_007e574d:\n\tmovb 0x1a(%%ecx), %%cl"
                         : "+a"(eax), "+c"(ecx) : : "memory");
        /* Expose signed predicates from the original byte registers. Keeping
           separate C byte temporaries made pinned GCC spill ECX into the frame;
           the preserved raw failure rejects that otherwise passing draft. */
        u8 greater;
        __asm__ volatile("cmpb %b2, %b1" : "=@ccg"(greater)
                         : "a"(eax), "c"(ecx) : "cc");
        if (greater)
            __asm__ volatile("movb $3, %b0" : "+a"(eax));
        else {
            u8 equal;
            __asm__ volatile("cmpb %b2, %b1" : "=@cce"(equal)
                             : "a"(eax), "c"(ecx) : "cc");
            if (equal) {
                u8 positive;
                __asm__ volatile("testb %b1, %b1" : "=@ccg"(positive)
                                 : "a"(eax) : "cc");
                if (positive)
                    __asm__ volatile("movb $2, %b0" : "+a"(eax));
                else
                    eax = 1u;
            } else {
                /* SETNE AL; DEC EAX; AND EAX,4; ADD EAX,-3. */
                eax = 0xfffffffdu;
            }
        }
    } else {
        /* Original diagnostic buffers, call order, arguments and return words. */
        __asm__ volatile(
            "v_007e56de:\n\tleal (%%esp), %%eax\n"
            "leal 0x100(%%esp), %%ecx\n"
            "pushl %%eax\n\tpushl %%ecx\n\tpushl $0\n\tpushl $0\n"
            "pushl $0xa7a088\n"
            CM_ORIGINAL_CALL(007e56f4, 007e56f9, 00944cff)
            "addl $0x14, %%esp\n\tleal (%%esp), %%edx\n"
            "leal 0x100(%%esp), %%eax\n\tpushl $0x25a\n"
            "pushl %%edx\n\tpushl %%eax\n"
            CM_ORIGINAL_CALL(007e570e, 007e5713, 0090d130)
            "pushl %%eax\n\tpushl $0x9870e8\n\tpushl $0xae24d0\n"
            CM_ORIGINAL_CALL(007e571e, 007e5723, 00944c9f)
            "pushl $0\n\tpushl $0xae24d0\n\tpushl $0x9870e0\n"
            CM_ORIGINAL_CALL(007e572f, 007e5734, 005e8290)
            "addl $0x24, %%esp\n\tmovl $0, 0xb67a34\n\txorb %%al, %%al"
            : "+a"(eax), "+d"(edx), "+c"(ecx) : : "memory", "cc");
    }
    __asm__ volatile("addl $0x200, %%esp\n\tret"
                     : : "a"(eax), "d"(edx), "c"(ecx), "b"(ebx),
                         "r"(ebp), "S"(esi), "D"(edi) : "memory", "cc");
    __builtin_unreachable();
}
