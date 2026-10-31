/* WIP: memory mismatch, case 595 (page 0x0301F000). When argument 2 is nonzero
   and the second 0x00785830 result is neither 0 nor 1, the buffer is never written
   and 0x004AE660 receives uninitialized stack. Two scratch variants (zero-filled
   buffer, fewer locals) did not clear the mismatch. Likely cause, not proven: the
   C frame does not match the original frame's stack contents or address.

   Preferred-image VA 0x005A04C0, image base 0x00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Stdcall with three stack words (RET 0xC); ECX is written before any read, so
   there is no thiscall receiver. The 0x1388-byte frame is a local buffer whose
   address is passed to the formatter (0x0066F4E0) and to 0x004AE660. Argument 2
   selects the no-object branch when zero. Otherwise argument 1 is the receiver
   for two 0x00785830 calls (index 1, then index 2 whose low byte is the mode).
   Mode 0 or 1 reads a 107-byte record at dword 0x00AE23D0 + index * 107 and
   formats its byte at +0x37 with the record field at +4. Any other mode leaves
   the buffer untouched. Argument 3 is the receiver for 0x004AE660. The stack
   probe at 0x00945610 reserves the frame and is not called from C, matching
   recovery/c/fn_005a0380.c. Global and string meanings remain unknown. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern char *CM_CDECL cm_fn_0066f4e0(char *dst, const char *fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

void CM_STDCALL cm_fn_005a04c0(u32 arg1, u32 arg2, u32 arg3)
{
    u8 buffer[5000];
    u32 index;
    u32 base;
    u32 entry;
    s32 selector;
    s32 value;

    if (arg2 == 0) {
        cm_fn_0066f4e0((char *)buffer, (const char *)0x009c9b40);
    } else {
        index = (u32)cm_fn_00785830((void *)arg1, 1);
        selector = (s8)cm_fn_00785830((void *)arg1, 2);
        if (selector == 0) {
            base = CM_GLOBAL(u32, 0x00ae23d0);
            entry = base + index * 107u;
            value = *(s8 *)(entry + 0x37u);
            cm_fn_0066f4e0((char *)buffer, (const char *)0x009c9db4,
                           value, value, entry + 4u);
        } else if (selector == 1) {
            base = CM_GLOBAL(u32, 0x00ae23d0);
            entry = base + index * 107u;
            value = *(s8 *)(entry + 0x37u);
            cm_fn_0066f4e0((char *)buffer, (const char *)0x009c9e34,
                           value, value, entry + 4u);
        }
    }
    cm_fn_004ae660((void *)arg3, (u32)buffer);
}
