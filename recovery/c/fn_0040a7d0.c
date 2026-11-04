/* Preferred-image VA 0x0040a7d0, image base 0x00400000.
   Target SHA-256:
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00549f70(void *self, u32 first, u32 second,
                                     u32 third, u32 fourth);

/* Thiscall: ECX is the byte source; two stack dwords are destination and value.
   The original packs its call's arguments with partial register writes:
     fourth = (entry EAX & 0xffff0000) | sign16(source[2]);
     second = (fourth & 0xffffff00) | source[4];
     first  = (entry ECX & 0xffff0000) | sign16(source[3]);
     third  = value; callee ECX = destination.
   The byte reads occur in the order +2, +4, +3, interleaved with stack writes.
   Original 00549f70 forwards the full fourth dword to 0054a2d0, so entry EAX's
   high word cannot be discarded based on the narrow field loads.

   This basic-assembly adapter preserves entry registers, the exact original
   stack slots and all alias/fault order. Saved ECX at entryESP-4 is set to zero
   before the call, then read back after it; saved ESI is also read back from
   its actual slot. The returned EAX is the actual callee-return ESI, rather
   than a reloaded destination argument. The original callee pops four dwords.
   CM_ORIGINAL_CALL retains the numeric CALL return word at its original slot;
   execution uses the reviewed VM continuation seam, not native integration.
   The body is compiler-specific and requires emitted-code inspection at O0/O2. */
u32 CM_THISCALL __attribute__((naked))
cm_fn_0040a7d0(void *self __attribute__((unused)),
              u32 destination __attribute__((unused)),
              u32 value __attribute__((unused)))
{
    __asm__(
        "v_0040a7d0:\n"
        "pushl %ecx\n"
        "v_0040a7d1:\n"
        "movsbw 2(%ecx), %ax\n"
        "v_0040a7d6:\n"
        "movl 12(%esp), %edx\n"
        "v_0040a7da:\n"
        "pushl %esi\n"
        "v_0040a7db:\n"
        "movl 12(%esp), %esi\n"
        "v_0040a7df:\n"
        "pushl %eax\n"
        "v_0040a7e0:\n"
        "movb 4(%ecx), %al\n"
        "v_0040a7e3:\n"
        "pushl %edx\n"
        "v_0040a7e4:\n"
        "movsbw 3(%ecx), %cx\n"
        "v_0040a7e9:\n"
        "pushl %eax\n"
        "v_0040a7ea:\n"
        "pushl %ecx\n"
        "v_0040a7eb:\n"
        "movl %esi, %ecx\n"
        "v_0040a7ed:\n"
        "movl $0, 20(%esp)\n"
        CM_ORIGINAL_CALL(0040a7f5, 0040a7fa, 00549f70)
        "movl %esi, %eax\n"
        "v_0040a7fc:\n"
        "popl %esi\n"
        "v_0040a7fd:\n"
        "popl %ecx\n"
        "v_0040a7fe:\n"
        "ret $8\n"
    );
}
